# 📚 Dynamic Stack Implementation in C

> A robust, dynamically resizing Stack data structure implemented from scratch in C, focusing on manual memory management and raw pointer arithmetic.

---

## ✨ Key Features & Technical Highlights

This project is not just a standard stack; it was built with strict low-level constraints to demonstrate a deep understanding of computer memory:

* **Dynamic Auto-Resizing:** The stack starts with a base capacity (10 elements) and automatically **doubles its size** using `realloc()` whenever it reaches its limit, preventing stack overflow.
* **Strict Pointer Arithmetic:** The entire codebase strictly avoids the array index operator (`[]`). All memory access and array traversals are handled via pure pointer arithmetic (e.g., `*(stack->data + stack->head++)`).
* **Robust Error Handling:** The application gracefully handles and reports edge cases:
    * Memory allocation failures (`malloc`/`realloc` returning `NULL`).
    * Attempts to pop from an empty stack.
    * Invalid user inputs in the CLI (immediately terminating with a specific error code).
* **Memory Leak Prevention:** Includes a dedicated `stack_free` function to ensure all dynamically allocated memory is properly released before program termination.

---

## 🎮 How to Use the Program (Menu Guide)

Once compiled and executed, the program runs in a continuous loop. It will ask `Co chcesz zrobic?` (What do you want to do?). Enter a number from the list below and press Enter:

* **`0`** - **Exit:** Safely frees all memory and terminates the program.
* **`1`** - **Push:** Add a new element to the top of the stack. (The program will prompt you for an integer value).
* **`2`** - **Pop:** Remove the top element from the stack and display its value. If the stack is empty, it prints `Stack is empty`.
* **`3`** - **Display All:** Prints all elements currently on the stack. If the stack is empty, it prints `Stack is empty`.

The program includes strict input validation and memory checking:
* **Invalid Menu Choice:** Entering a number outside the menu options prints `Incorrect input data` and continues execution.
* **Memory Allocation Failure:** If `malloc` or `realloc` fails to secure memory, the program prints `Failed to allocate memory` and immediately exits with error code `8`.
* **Invalid Characters (Letters/Symbols):** If the user inputs non-numeric characters when a number is expected, the program instantly terminates, printing `Incorrect input`, and exits with error code `1`.

---

## 🛠️ Project Structure

* `stack.h` - Header file containing the `stack_t` structure definition and function prototypes.
* `stack.c` - Core implementation of the stack logic (init, push, pop, free, display).
* `main.c` - Interactive Command Line Interface (CLI) for user testing.

---

## 🚀 How to Compile and Run

### Prerequisites
You need a C compiler (like `gcc` or `clang`) installed on your system.

### Compilation
Navigate to the project directory in your terminal and compile the source files:

```bash
gcc main.c stack.c -o dynamic_stack