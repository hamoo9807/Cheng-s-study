# Week1: Array-based & Linked Stack Implementation
## Project Overview
This project implements two stack structures (array-based and linked) in C, with performance tests for 100,000 push/pop operations and memory safety verification. The core goal is to compare the performance differences between static (array) and dynamic (linked list) stack implementations.

## File Structure
| File Name                          | Description                                                                 |
|------------------------------------|-----------------------------------------------------------------------------|
| `week1_arraystack_core.c`          | Core implementation of array-based stack (push/pop/init/free)               |
| `week1_linkedstack_core.c`         | Core implementation of linked stack (node-based push/pop/init/free)         |
| `week1_stack_performance_benchmark.md` | Exact performance test report for both stack types                      |

## Performance Summary (Exact Values)
| Metric                                  | Array-based Stack | Linked Stack                  |
|-----------------------------------------|-------------------|-------------------------------|
| Total Operations (Push + Pop)           | 200,000           | 200,000                       |
| Total Execution Time (s)                | 0.000000          | 5.185 / 4.957 / 5.146 (Avg 5.096) |
| Single Operation Average Time (s)       | 0.000000          | 0.000026 / 0.000025 / 0.000026 (Avg 0.000026) |
| Post-Push Top Value                     | 99999             | 99999                         |
| Memory Safety                           | No leaks          | No leaks                      |

## Compilation & Execution
### Compile
```bash
# Compile array-based stack
gcc week1_arraystack_core.c -o arraystack.exe

# Compile linked stack
gcc week1_linkedstack_core.c -o linkedstack.exe