# lan-scanner

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)
![License](https://img.shields.io/badge/license-MIT-green)
![Platform](https://img.shields.io/badge/platform-linux%20%7C%20macos-lightgrey)

A simple command-line LAN scanner written in C++17.
It discovers live hosts and open ports in your local subnet.

## Features
- Scans an IP range given in CIDR notation (`192.168.1.0/24`)
- Checks common ports: 22, 80, 443, 445
- Parallel host probing via `std::async`
- Non-blocking `connect` with timeout — never hangs on dead hosts

## Requirements
- Linux or macOS
- CMake 3.10+
- A C++17 compiler (g++ 9+, clang 10+)

## Build and run (available once the source is added)
```bash
cmake -B build
cmake --build build
./build/lan-scanner 192.168.1.0/24
