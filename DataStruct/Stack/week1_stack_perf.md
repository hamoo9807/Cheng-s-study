# C Language: Array-based vs Linked Stack Performance Benchmark
## Test Overview
| Metric               | Details                                                                 |
|----------------------|-------------------------------------------------------------------------|
| Programming Language | C (Compiled with GCC; Command: `gcc [filename].c -o [executable].exe`)  |
| Test Environment     | Windows 11, Intel Core i7 Processor, 16GB RAM                           |
| Test Workload        | 100,000 push operations + 100,000 pop operations                        |
| Memory Validation    | No memory leaks detected (explicit stack memory release confirmed)     |

## Core Performance Results (Exact Values)
| Stack Type          | 100k Push + 100k Pop Total Time (s) | Single Operation Avg Time (s) | Post-Push Top Value | Validity  |
|---------------------|--------------------------------------|--------------------------------|---------------------|-----------|
| Array-based Stack   | 0.000000                             | 0.000000                       | 99999               | Valid     |
| Linked Stack        | 5.096 (Avg of 5.185/4.957/5.146)     | 0.000026 (Avg of 0.000026/0.000025/0.000026) | 99999               | Valid     |

## Raw Test Data (Exact Log Values)
### Array-based Stack
| Run # | Total Execution Time (s) | Single Op Avg Time (s) | Memory Release State |
|-------|--------------------------|------------------------|----------------------|
| 1     | 0.000000                 | 0.000000               | Freed (no leaks)     |
| 2     | 0.000000                 | 0.000000               | Freed (no leaks)     |
| 3     | 0.000000                 | 0.000000               | Freed (no leaks)     |
| 4     | 0.000000                 | 0.000000               | Freed (no leaks)     |
| 5     | 0.000000                 | 0.000000               | Freed (no leaks)     |

### Linked Stack
| Run # | Total Execution Time (s) | Single Op Avg Time (s) | Post-Push Top Value | Memory Release State |
|-------|--------------------------|------------------------|---------------------|----------------------|
| 1     | 5.185                    | 0.000026               | 99999               | Freed (no leaks)     |
| 2     | 4.957                    | 0.000025               | 99999               | Freed (no leaks)     |
| 3     | 5.146                    | 0.000026               | 99999               | Freed (no leaks)     |

## Key Observations
1. **Array-based Stack Performance**: The total time for 200,000 operations is 0.000000 seconds (below the test environment's time measurement precision), indicating ultra-low latency from direct array index manipulation (no dynamic memory overhead).
2. **Linked Stack Performance**: The average total time for 200,000 operations is 5.096 seconds, with an average single operation time of 0.000026 seconds. The overhead comes from dynamic node malloc/free and pointer operations.
3. **Validity**: Both stacks confirm correct post-push top value (99999) and complete memory release with no leaks, verifying functional correctness.

## Optimization Insights
- For Linked Stacks: Implement a node memory pool to reduce frequent malloc/free calls, which will significantly lower execution time.
- For Array-based Stacks: Retain direct index access; add dynamic resizing (realloc) to avoid fixed-size limitations while preserving performance.