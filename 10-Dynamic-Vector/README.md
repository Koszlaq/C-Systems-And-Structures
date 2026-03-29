# 📈 Dynamic Vector Implementation

> A complete C implementation of a dynamic array, mirroring the behavior of C++'s `std::vector`. It automatically manages memory allocation, growing and shrinking seamlessly based on the data volume.

---

## ✨ Key Features & Technical Highlights

This project demonstrates a deep understanding of amortized time complexity and contiguous memory management.

* **Dynamic Auto-Scaling (Growth):** When the vector reaches its maximum capacity during a `push_back` operation, it automatically allocates a new memory block **twice** the size of the current one using `realloc()`. This guarantees an amortized $O(1)$ insertion time.
* **Aggressive Memory Optimization (Shrinkage):** The `vector_erase` function doesn't just logically remove elements; it actively monitors the `size`-to-`capacity` ratio. If the vector becomes less than **25% full**, it halves its allocated memory footprint. This prevents memory bloat in long-running applications.
* **Contiguous Memory:** Unlike linked lists, the vector guarantees contiguous memory layout (`int *ptr`), ensuring high cache locality and $O(1)$ random access speeds.
* **Robust Error & Pointer Handling:** Deep memory cleanups are handled through pointer-to-pointer functions (`vector_create_struct`, `vector_destroy_struct`) preventing any dangling pointers or memory leaks upon program termination.

---

## 🎮 How to Use the Program (Menu Guide)

Upon execution, the program will first ask: `Podaj pojemnosc tablicy:` (Enter initial vector capacity). Provide a positive integer.

After successful allocation, it will prompt: `Co chesz zrobic?` (What do you want to do?). Enter a number:

* **`0`** - **Exit:** Safely destroys the vector, frees all memory, and ends the program.
* **`1`** - **Push Back (Add):** Adds elements to the end of the vector. 
    * *Instructions:* Enter numbers separated by spaces. Type `0` to stop adding and return to the main menu.
* **`2`** - **Erase (Remove):** Removes ALL occurrences of specified values from the vector, shifting subsequent elements left to fill the gaps and maintaining the original order.
    * *Instructions:* Enter numbers separated by spaces. Type `0` to stop erasing and return to the main menu.

The program includes strict input validation and memory checking:
* If the vector runs out of system memory during expansion (`realloc` fails), it safely stops adding elements, prints `Failed to allocate memory`, and displays the vector in its last valid state.
* If all elements are erased, it outputs `Buffer is empty`.
* Non-numeric inputs immediately terminate the program safely (Exit Code 1).

---

## 🛠️ Project Structure

* `vector.h` - Contains the `vector_t` struct (`ptr`, `size`, `capacity`) and all function declarations.
* `vector.c` - Core implementation of allocation, reallocation, and the shifting logic for the `erase` function.
* `main.c` - An interactive loop for testing multiple push and erase operations with continuous memory monitoring.

---

## 🚀 Compilation

Navigate to this directory and compile the program using `gcc`:

```bash
gcc main.c vector.c -o vector