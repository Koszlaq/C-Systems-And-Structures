# 📚 Array-Based Stack Implementation

> A fundamental Last-In-First-Out (LIFO) data structure implemented in C, showcasing manual memory allocation and strict pointer arithmetic.

---

## ✨ Key Features & Technical Highlights

This project demonstrates a core understanding of how standard data structures operate under the hood in C.

* **Array-Based Architecture:** The stack is built on top of a contiguous memory block, ensuring $O(1)$ time complexity for core operations like `push` and `pop`.
* **Strict Pointer Arithmetic:** To demonstrate low-level memory control, the array index operator (`[]`) is strictly forbidden in this codebase. All memory traversals and assignments are executed using raw pointer math (e.g., `*(stack->data + stack->head)`).
* **Robust Error Handling:** The program includes a continuous Command Line Interface (CLI) that gracefully handles edge cases:
    * Attempts to pop from an empty stack.
    * Invalid, non-numeric user inputs.
    * Memory allocation tracking and failure reporting.
* **Clean Memory Management:** Ensures no memory leaks by properly freeing the allocated stack structure and its internal data array upon program termination.

---

## 🛠️ Project Structure

* `stack.h` - Contains the `stack_t` struct definition and function prototypes.
* `stack.c` - Core stack logic (`init`, `push`, `pop`, `display`, `free`).
* `main.c` - Interactive CLI loop for testing stack operations.

---

## 🚀 How to Compile and Run

Navigate to this directory in your terminal and compile the program using `gcc`:

```bash
gcc main.c stack.c -o static_stack