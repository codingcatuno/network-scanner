## Concurrent Scanner Benchmark

### Sequential Baseline

| Port Range | Time |
|------------|------|
| 1–1,000 | 31 ms |
| 1–10,000 | 236 ms |

### Concurrent Results

| Port Range | Run 1 | Run 2 | Run 3 | Average |
|------------|------:|------:|------:|--------:|
| 1–1,000 | 6 ms | 6 ms | 9 ms | 7 ms |
| 1–10,000 | 65 ms | 76 ms | 72 ms | 71 ms |

### Approximate Speedup

- 1–1,000 ports: ~4.4x faster
- 1–10,000 ports: ~3.3x faster

### Observation

Using a bounded worker-thread pool significantly reduced scan time compared with the sequential implementation. The scanner distributes independent port-scan jobs through a shared queue while mutexes protect the work queue and results vector.

**PERSONAL notes**

set up benchmarking code

make sure you reformat the scanner results if you havent already.
