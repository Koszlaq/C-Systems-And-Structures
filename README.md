# ⚙️ C Systems Programming & Data Structures

> A comprehensive collection of advanced C programming projects, focusing on operating system internals, low-level memory management, and fundamental data structures.

This repository serves as my technical portfolio for C programming. It demonstrates my ability to work close to the hardware, manage raw memory, interact with the OS kernel (signals, file systems), and implement efficient data structures from scratch.

---

## 🚀 Part 1: Advanced Systems Programming (OS Level)

These projects replicate core operating system functionalities and standard library utilities, demonstrating a deep understanding of computer architecture and OS internals.

| # | Project Name | Description | Key Concepts |
| :---: | :--- | :--- | :--- |
| **01** | [Custom Memory Allocator](./01-Custom-Memory-Allocator) | A custom implementation of `malloc`, `calloc`, `realloc`, and `free`. | Heap management, memory fragmentation, pointer arithmetic. |
| **02** | [FAT12 File System Reader](./02-FAT12-File-System-Reader) | A low-level reader for the FAT12 file system, including custom `open`, `read`, `seek`, and `close` functions. | Boot sector parsing, clusters, raw binary file I/O. |
| **03** | [Cron Daemon Clone](./03-Cron-Daemon-Clone) | A custom background service mimicking the Linux `cron` utility for task scheduling. | Daemons, process creation (`fork`), time management. |
| **04** | [Signal-Driven Logger](./04-Signal-Driven-Logger) | A robust logging library controlled entirely via UNIX signals. | Inter-process communication (IPC), signal handlers, thread safety. |
| **05** | [Custom Standard I/O](./05-Custom-Standard-IO) | A from-scratch implementation of formatted input/output functions (`printf`, `scanf`). | Variadic functions (`stdarg.h`), string parsing, type casting. |

---

## 🧱 Part 2: Data Structures & Algorithms

Solid implementations of classic data structures, built strictly with manual memory management and optimized pointer operations.

| # | Project Name | Description | Key Concepts |
| :---: | :--- | :--- | :--- |
| **06** | [Dynamic Stack](./06-Dynamic-Stack) | Auto-resizing stack implemented via pure pointer arithmetic (no `[]` operators). | `realloc`, dynamic arrays, pointer math. |
| **07** | [Static Stack](./07-Static-Stack) | Fixed-size stack implementation. | LIFO principles, edge case handling. |
| **08** | [Singly Linked List](./08-Singly-Linked-List) | Standard forward-navigable linked list. | Node linking, memory allocation. |
| **09** | [Doubly Linked List](./09-Doubly-Linked-List) | Bi-directional linked list for complex iterations. | Prev/Next pointers, complex node deletion. |
| **10** | [Dynamic Vector](./10-Dynamic-Vector) | A generic, contiguous, resizable array (similar to `std::vector` in C++). | Capacity vs. Size, amortized time complexity. |
| **11** | [Vector Mathematics](./11-Vector-Mathematics) | A library for mathematical operations on vectors (dot product, cross product, scaling). | Linear algebra, optimized iterations. |
| **12** | [Circular Buffer](./12-Circular-Buffer) | A ring buffer implementation, highly useful in embedded systems and stream processing. | Modulo arithmetic, overwrite policies, FIFO. |

---

## 🛠️ General Build Instructions

Each project is contained within its own directory and can be compiled using a standard C compiler (e.g., `gcc` or `clang`). Detailed instructions, specific edge cases, and architectural decisions are documented in the `README.md` file located inside each specific project folder.

```bash
# Example: Compiling and running the Dynamic Stack
cd 06-Dynamic-Stack
gcc main.c stack.c -o stack
./stack
