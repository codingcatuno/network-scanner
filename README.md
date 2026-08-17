# C++ Network Scanner

A TCP network scanner built in C++ to explore socket programming, TCP/IP networking, and systems programming.

The scanner performs TCP connection attempts against a specified range of ports on a target host and reports which ports are accepting connections.

> **Status:** In active development.

## Current Features

- IPv4 TCP socket creation
- User-defined port ranges
- TCP connection testing
- Open-port detection
- Storage of discovered open ports
- Scan execution timing
- Input validation for port ranges
- Proper socket cleanup

## How It Works

The scanner creates a TCP socket and attempts to establish a connection to each port within the selected range.

For each port:

1. A TCP socket is created.
2. The destination IPv4 address and port are configured.
3. A TCP connection is attempted.
4. Successful connections are recorded as open.
5. The socket is closed before continuing.

Each port is currently scanned sequentially.

## Current Architecture

Target IPv4 Address
        |
        v
Selected Port Range
        |
        v
Create TCP Socket
        |
        v
Attempt Connection
        |
     +--+--+
     |     |
   OPEN   FAILED
     |
     v
Store Result
        |
        v
Close Socket
        |
        v
Next Port

## Performance Baseline

Initial testing was performed against localhost (`127.0.0.1`) using the sequential implementation.

| Port Range | Ports Scanned | Execution Time |
|------------|--------------:|---------------:|
| 1–1,000 | 1,000 | 31 ms |
| 1–10,000 | 10,000 | 236 ms |

These results represent localhost testing and should not be interpreted as representative of remote network performance. Local connection failures can be returned extremely quickly.

This baseline will be used to compare future concurrent implementations.

## Technologies & Concepts

- C++
- Linux socket API
- TCP/IP
- IPv4
- Socket programming
- `sockaddr_in`
- Network byte order
- C++ STL containers
- `std::chrono`

## Planned Improvements

- Concurrent port scanning
- Configurable target IPv4 addresses
- Improved connection error classification
- Service identification
- Cleaner command-line interface
- Optional verbose output
- JSON result export
- Performance comparisons between sequential and concurrent scanning

## What I'm Learning

This project is being built to develop a deeper understanding of networking and systems programming rather than relying on an existing scanning library.

Topics explored so far include:

- How TCP sockets are created and managed
- The difference between sockets, ports, and file descriptors
- How IPv4 destinations are represented using `sockaddr_in`
- Host versus network byte order
- Converting human-readable IP addresses into network representations
- Establishing TCP connections using the Linux socket API
- Managing socket resources
- Measuring program execution time
- Selecting data structures for storing scan results

## Responsible Use

This project is intended for educational purposes and authorized network testing only.

Only scan systems you own or have explicit permission to test.

*Just a fun little project*
