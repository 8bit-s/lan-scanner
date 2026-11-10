#ifdef _WIN32
    #define _WIN32_WINNT 0x0601
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    using socket_t = SOCKET;
    #define CLOSE_SOCKET closesocket
#else
    #include <arpa/inet.h>
    #include <fcntl.h>
    #include <netinet/in.h>
    #include <sys/select.h>
    #include <sys/socket.h>
    #include <unistd.h>
    using socket_t = int;
    #define CLOSE_SOCKET close
    #define INVALID_SOCKET (-1)
#endif

#include <cerrno>
#include <cstring>
#include <future>
#include <iostream>
#include <string>
#include <vector>

static bool port_open(const std::string& ip, int port, int timeout_ms) {
    socket_t s = ::socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) return false;

#ifndef _WIN32
    int flags = ::fcntl(s, F_GETFL, 0);
    ::fcntl(s, F_SETFL, flags | O_NONBLOCK);
#else
    u_long mode = 1;
    ::ioctlsocket(s, FIONBIO, &mode);
#endif

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(static_cast<unsigned short>(port));
    if (::inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) != 1) {
        CLOSE_SOCKET(s);
        return false;
    }

    int res = ::connect(s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
#ifndef _WIN32
    if (res < 0 && errno != EINPROGRESS) { CLOSE_SOCKET(s); return false; }
#else
    if (res == SOCKET_ERROR && ::WSAGetLastError() != WSAEWOULDBLOCK) {
        CLOSE_SOCKET(s);
        return false;
    }
#endif

    fd_set wfds;
    FD_ZERO(&wfds);
    FD_SET(s, &wfds);
    timeval tv{timeout_ms / 1000, (timeout_ms % 1000) * 1000};

#ifndef _WIN32
    res = ::select(s + 1, nullptr, &wfds, nullptr, &tv);
#else
    res = ::select(0, nullptr, &wfds, nullptr, &tv);
#endif
    if (res <= 0) { CLOSE_SOCKET(s); return false; }

    int err = 0;
    socklen_t len = sizeof(err);
    ::getsockopt(s, SOL_SOCKET, SO_ERROR,
                 reinterpret_cast<char*>(&err), &len);
    CLOSE_SOCKET(s);
    return err == 0;
}

int main(int argc, char** argv) {
#ifdef _WIN32
    WSADATA wsa{};
    if (::WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }
#endif

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <CIDR>  e.g. 192.168.1.0/24\n";
#ifdef _WIN32
        ::WSACleanup();
#endif
        return 1;
    }

    std::string cidr = argv[1];
    auto slash = cidr.find('/');
    if (slash == std::string::npos) {
        std::cerr << "Invalid CIDR: missing prefix\n";
        return 1;
    }

    in_addr base{};
    if (::inet_pton(AF_INET, cidr.substr(0, slash).c_str(), &base) != 1) {
        std::cerr << "Invalid IP address\n";
        return 1;
    }

    int prefix = std::stoi(cidr.substr(slash + 1));
    if (prefix < 1 || prefix > 30) {
        std::cerr << "Prefix must be between 1 and 30\n";
        return 1;
    }

    unsigned int mask = (prefix == 0) ? 0u : (~0u << (32 - prefix));
    unsigned int net = ntohl(base.s_addr) & mask;
    unsigned int bcast = net | ~mask;

    const std::vector<int> ports = {22, 80, 443, 445};
    std::vector<std::future<std::pair<std::string, std::vector<int>>>> jobs;

    for (unsigned int h = net + 1; h < bcast; ++h) {
        in_addr x{};
        x.s_addr = htonl(h);
        char buf[INET_ADDRSTRLEN];
        if (!::inet_ntop(AF_INET, &x, buf, sizeof(buf))) continue;
        std::string ip = buf;

        jobs.push_back(std::async(std::launch::async, [ip, ports]() {
            std::vector<int> open;
            for (int p : ports) {
                if (port_open(ip, p, 100)) open.push_back(p);
            }
            return std::make_pair(ip, open);
        }));
    }

    int found = 0;
    for (auto& f : jobs) {
        auto [ip, open] = f.get();
        if (open.empty()) continue;
        ++found;
        std::cout << "[+] " << ip << " -> ";
        for (std::size_t i = 0; i < open.size(); ++i) {
            std::cout << (i ? ", " : "") << open[i];
        }
        std::cout << "\n";
    }

    std::cout << "\nFound " << found << " host(s).\n";

#ifdef _WIN32
    ::WSACleanup();
#endif
    return 0;
}
