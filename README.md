# C++ TCP Port Scanner

A multithreaded TCP port scanner built from scratch in C++ to explore low-level network programming, Linux sockets, concurrency, synchronization, and systems programming.

The scanner performs TCP connection attempts across a user-defined range of ports and reports which ports are accepting connections.

> **Status:** Active development — multithreaded TCP scanning implemented; performance benchmarking and additional features in progress.

---

## Features

### Implemented

- IPv4 TCP socket creation using the Linux socket API
- User-defined port ranges
- Port range validation
- TCP connect scanning
- Open-port detection
- Multithreaded concurrent scanning
- Bounded worker-thread pool
- Shared work queue for port distribution
- Mutex-protected queue access
- Thread-safe result collection
- Storage of discovered open ports
- Scan execution timing using `std::chrono`
- Proper socket cleanup
- Thread synchronization using `join()`
- Clean scan summary
- Localhost testing and validation

### In Development

- Worker-count benchmarking
- Performance comparison across different thread-pool sizes
- Improved program structure and refactoring

### Planned

- User-defined target IPv4 addresses
- Configurable worker-thread count
- Improved connection error classification
- Service identification
- Command-line arguments
- Optional verbose output
- JSON result export
- Remote-host performance testing

---

## How It Works

The scanner performs a TCP connect scan against a selected range of ports.

Ports are placed into a shared work queue. A fixed number of worker threads retrieve ports from the queue and independently attempt TCP connections.

For each port, a worker:

1. Safely retrieves the next port from the shared work queue.
2. Creates an IPv4 TCP socket.
3. Configures the target address and port.
4. Converts the target IPv4 address into its network representation.
5. Attempts to establish a TCP connection using `connect()`.
6. Records the port if the connection succeeds.
7. Closes the socket.
8. Retrieves another port until no work remains.

After all workers finish, the main thread joins them and reports the discovered open ports and total scan time.

---

## Architecture

```text
                    Target IPv4 Address
                            |
                            v
                    User-Defined Range
                            |
                            v
                     Port Work Queue
                   [1, 2, 3 ... N]
                            |
                       Queue Mutex
                            |
              +-------------+-------------+
              |             |             |
              v             v             v
           Worker 1      Worker 2      Worker 3   ...
              |             |             |
              |         TCP connect()     |
              |             |             |
              +-------------+-------------+
                            |
                      Open Port?
                            |
                            v
                      Results Mutex
                            |
                            v
                    Open Ports Vector
                            |
                            v
                       Join Workers
                            |
                            v
                       Scan Summary
```

---

## Concurrency Design

TCP connection attempts are independent and primarily I/O-bound.

The original sequential implementation had to wait for each connection attempt to finish before beginning the next:

```text
Port 1 -> wait -> Port 2 -> wait -> Port 3 -> wait -> ...
```

The concurrent implementation allows multiple connection attempts to remain in progress during the same period:

```text
Worker 1 -> Port 1 ---------->
Worker 2 -> Port 2 ----->
Worker 3 -> Port 3 -------------->
Worker 4 -> Port 4 ---> next job
```

Rather than creating one thread for every port, the scanner uses a limited worker pool. Workers continuously retrieve jobs from a shared queue until no ports remain.

This avoids creating potentially thousands of threads while still allowing network wait time to overlap.

---

## Thread Safety

The concurrent scanner contains two primary shared resources:

### Port Work Queue

Multiple workers retrieve ports from the same queue.

A mutex protects the queue to prevent multiple threads from modifying it simultaneously.

The lock is held only long enough to:

1. Check whether work remains.
2. Retrieve the next port.
3. Remove it from the queue.

The lock is released **before the TCP connection attempt**, preventing slow network operations from blocking other workers from retrieving work.

### Open-Port Results

Open ports are stored in a shared results container.

A separate mutex protects result insertion so multiple workers cannot modify the container simultaneously.

`std::lock_guard` is used for automatic mutex management through RAII, ensuring locks are released when their scope ends.

---

## Performance

Initial benchmarks were performed against localhost (`127.0.0.1`).

### Sequential Baseline

| Port Range | Execution Time |
|------------|---------------:|
| 1–1,000 | 31 ms |
| 1–10,000 | 236 ms |

### Concurrent Scanner — 4 Workers

Each concurrent test was performed three times.

| Port Range | Run 1 | Run 2 | Run 3 | Average |
|------------|------:|------:|------:|--------:|
| 1–1,000 | 6 ms | 6 ms | 9 ms | 7 ms |
| 1–10,000 | 65 ms | 76 ms | 72 ms | 71 ms |

### Performance Improvement

Compared with the sequential baseline:

- **1–1,000 ports:** approximately **4.4x faster**
- **1–10,000 ports:** approximately **3.3x faster**

The 1–10,000 port scan decreased from approximately:

```text
236 ms -> 71 ms
```

using four worker threads.

> These measurements were performed against localhost, where rejected TCP connections typically return very quickly. They should not be interpreted as representative of remote network performance.

Additional benchmarking across different worker counts is planned.

---

## Benchmarking Roadmap

The next performance experiment will compare worker-pool sizes while keeping the target and port range constant.

Planned worker counts include:

```text
1
2
4
8
16
32
```

Each configuration will be tested multiple times and compared using average execution time.

This will help determine when additional concurrency stops producing meaningful performance improvements due to factors such as:

- Thread-management overhead
- Context switching
- Mutex contention
- Socket/network overhead
- System resource limits

The VM hardware and resource configuration used for these benchmarks will also be documented to make the results reproducible and easier to interpret.

---

## Technical Concepts

### Networking

- TCP/IP
- IPv4
- TCP connect scanning
- Linux socket API
- `socket()`
- `connect()`
- `sockaddr_in`
- `inet_pton()`
- Network byte order
- `htons()`
- File descriptors
- Socket resource management

### C++

- C++ STL containers
- `std::vector`
- `std::queue`
- `std::chrono`
- Lambda expressions
- Reference captures
- RAII

### Concurrency

- `std::thread`
- Worker threads
- Thread pools
- Shared work queues
- `std::mutex`
- `std::lock_guard`
- Race-condition prevention
- Critical sections
- Thread synchronization
- `join()`

---

## Development Progress

The scanner has been developed incrementally:

1. Created a single IPv4 TCP socket.
2. Learned how socket file descriptors work.
3. Represented IPv4 destinations using `sockaddr_in`.
4. Converted port numbers using network byte order.
5. Converted human-readable IPv4 addresses using `inet_pton()`.
6. Established TCP connections using `connect()`.
7. Detected whether individual ports accepted connections.
8. Added user-defined port selection.
9. Expanded scanning to user-defined port ranges.
10. Stored discovered open ports.
11. Added execution-time measurement.
12. Established sequential performance baselines.
13. Identified port scanning as an I/O-bound workload.
14. Designed a worker-thread architecture.
15. Added a shared port work queue.
16. Added mutex protection for shared resources.
17. Implemented worker threads using C++ lambdas.
18. Added thread synchronization using `join()`.
19. Benchmarked the concurrent implementation against the sequential baseline.

---

## What I've Learned

Building this project has helped develop my understanding of:

- The difference between sockets, ports, and file descriptors
- How programs identify network destinations using IP addresses and ports
- How TCP clients establish connections
- How Linux represents IPv4 socket addresses
- Why host and network byte order differ
- How system calls communicate success and failure
- Why socket resources must be properly closed
- How to select data structures based on their intended operations
- Why network operations are commonly I/O-bound
- The difference between concurrency and parallelism
- How worker pools distribute independent tasks
- How race conditions arise from shared mutable state
- How mutexes protect critical sections
- Why locks should be held for the shortest practical duration
- How `std::lock_guard` uses RAII for safer synchronization
- How lambda capture by reference enables workers to operate on shared state
- Why worker threads must be joined before shared resources leave scope
- How benchmarking can quantify the effect of architectural changes

---

## Responsible Use

This project is intended for educational purposes and authorized security testing.

Only scan systems you own or systems for which you have explicit permission to perform network testing.