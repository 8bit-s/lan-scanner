# lan-scanner

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)
![License](https://img.shields.io/badge/license-MIT-green)
![Platform](https://img.shields.io/badge/platform-linux%20%7C%20macos%20%7C%20windows-lightgrey)

A simple command-line LAN scanner written in C++17.
It discovers live hosts and open ports in your local subnet.

## Features
- Scans an IP range in CIDR notation (`192.168.1.0/24`)
- Checks common ports: 22, 80, 443, 445
- Parallel host probing via `std::async`
- Non-blocking `connect` with timeout
- Cross-platform: Linux, macOS, Windows

## Requirements
- CMake 3.10+
- A C++17 compiler (g++ 9+, clang 10+, MSVC 2019+)

## Build and run
```bash
# Linux / macOS
cmake -B build
cmake --build build
./build/lan-scanner 192.168.1.0/24

# Windows (Developer Command Prompt)
cmake -B build -A x64
cmake --build build --config Release
build\Release\lan-scanner.exe 192.168.1.0/24
