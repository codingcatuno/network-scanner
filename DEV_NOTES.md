# Network Scanner — Development Notes

## Sequential Scanner Baseline

### Current Implementation
- TCP connect scanner
- IPv4
- Target: localhost (127.0.0.1)
- Scans ports sequentially
- Open ports stored in a vector

### Performance

| Port Range | Ports Scanned | Time |
|------------|---------------|------|
| 1–1,000    | 1,000         | 31 ms |
| 1–10,000   | 10,000        | 236 ms |

### Observations
- Localhost scanning is very fast.
- Each port is currently scanned one at a time.
- Connection attempts are independent of each other.
- Closed localhost ports appear to reject connections quickly.
- Port 631 was unexpectedly found open.

### Ideas / Next Steps
- Stop displaying closed ports by default.
- Investigate concurrent scanning.
- Compare concurrent performance against this sequential baseline.
