# lan-scanner
> **For authorized security testing and self-hosted network diagnostics only.**

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)
![License](https://img.shields.io/badge/license-MIT-green)
![Platform](https://img.shields.io/badge/platform-linux%20%7C%20macos%20%7C%20windows-lightgrey)

A simple command-line LAN scanner written in C++17.
It discovers live hosts and open ports in your local subnet.

## ⚠️ Ethics & Legal Notice

`lan-scanner` is a **diagnostic tool for networks you own or are explicitly
authorized to test**. It is intended for home users, system administrators,
and students working in a lab they control.

### ✅ Allowed
- Your own home or office LAN
- An isolated lab or virtual network
- Authorized security testing **with written permission** from the network owner

### ❌ Forbidden
- Scanning third-party networks without permission
- Scanning ISP, university, employer, or any infrastructure you do not own
- Using scan results to gain unauthorized access to any system

### ⚖️ Legal context
Laws in most jurisdictions criminalize unauthorized access to computer
systems and networks. Port scanning by itself does not modify data, but
**intent and context matter**: if a scan is a step toward an attack, it is
treated as part of the offense. Scanning infrastructure you do not own may
also violate the provider's Terms of Service and local telecommunications
law. **You are responsible for knowing and following the laws that apply
to you.**

### 🧭 Rule of thumb
> **If you don't own the network and don't have written permission — don't scan it.**

By using this tool you accept full responsibility for how you use it.
The author(s) of `lan-scanner` are not liable for any misuse or for any
damage caused by unauthorized scanning.

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
