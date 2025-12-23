# C Linked List Performance Benchmark
A performance comparison between singly and doubly linked lists implemented in C, including 100k insert/delete operations, latency metrics, and optimization insights.


## Project Overview
This repository contains:
- C implementations of singly/doubly linked lists (core logic)
- Performance test scripts (100k insert/delete operations)
- A detailed benchmark report (this document)


## Key Files
| File Name                          | Description                                  |
|------------------------------------|----------------------------------------------|
| `c_ds_singly_linked_list_core.c`   | Singly linked list implementation (C)        |
| `c_ds_doubly_linked_list_core.c`   | Doubly linked list implementation (C)        |
| `c_ds_linked_list_performance_comparison.md` | Benchmark report (metrics + analysis) |


## Benchmark Summary
| Metric                  | Singly Linked List | Doubly Linked List |
|-------------------------|--------------------|--------------------|
| 100k Insert Time (s)    | 0.0368             | 0.0364             |
| 100k Delete Time (s)    | 13.8602            | 6.0638             |
| Total Execution Time (s)| 13.8970            | 6.1002             |


## Insights
1. **Insert Efficiency**: Head/tail inserts have similar latency (no traversal needed).
2. **Delete Efficiency**: Doubly linked lists are ~2.28x faster (uses `prev` pointer).
3. **Optimization**: Singly lists can be optimized with tail pointers; doubly lists with node pools.


## Compilation
Use GCC to compile:
```bash
gcc c_ds_singly_linked_list_core.c -o singly_list.exe
gcc c_ds_doubly_linked_list_core.c -o doubly_list.exe
```