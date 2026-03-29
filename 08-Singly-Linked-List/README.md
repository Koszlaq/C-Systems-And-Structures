# 🔗 Full Singly Linked List Implementation

> A comprehensive, dynamic Singly Linked List data structure in C, featuring both `head` and `tail` pointers for optimized end-insertions, and supporting index-based operations.

---

## ✨ Key Features & Technical Highlights

This project goes beyond a simple stack/queue by implementing a full suite of linked list operations, demonstrating robust manual memory management and complex pointer manipulation.

* **Optimized Appends ($O(1)$ Time Complexity):** Unlike standard singly linked lists that only track the `head` and require $O(N)$ time to find the end, this implementation maintains a `tail` pointer. This allows `push_back` operations to execute instantly in $O(1)$ time.
* **Index-Based Operations:** Supports inserting (`ll_insert`) and removing (`ll_remove`) nodes at arbitrary indices. This requires careful iteration and state tracking (maintaining a pointer to the *previous* node) to successfully rewire the links without causing memory leaks or segmentation faults.
* **Granular Error Handling:** Every function meticulously checks for `NULL` pointers, out-of-bounds indices, and empty list states. Errors are communicated via specific return codes and an optional `err_code` output parameter.
* **Robust Memory Clearance:** Includes a dedicated `ll_clear` function that systematically traverses the list and frees every allocated node before resetting the structural pointers, preventing memory leaks during runtime resets.

---

## 🎮 How to Use the Program (Menu Guide)

Once compiled and executed, the program runs in a continuous loop. It will ask `Co chcesz zrobic?` (What do you want to do?). Enter a number from the list below and press Enter:

* **`0`** - **Exit:** Safely frees all allocated memory and terminates the program.
* **`1`** - **Push Back:** Add a new number to the **end** of the list.
* **`2`** - **Pop Back:** Remove and print the number from the **end** of the list.
* **`3`** - **Push Front:** Add a new number to the **beginning** of the list.
* **`4`** - **Pop Front:** Remove and print the number from the **beginning** of the list.
* **`5`** - **Insert at Index:** Add a number at a specific position. (You will be prompted for the number, then the index).
* **`6`** - **Remove at Index:** Remove a number from a specific position. (You will be prompted for the index).
* **`7`** - **Peek Back:** View the last number without removing it.
* **`8`** - **Peek Front:** View the first number without removing it.
* **`9`** - **Check if Empty:** Returns `1` if the list is empty, `0` otherwise.
* **`10`** - **Get Size:** Displays the total number of elements currently in the list.
* **`11`** - **Clear List:** Deletes all elements and resets the list to an empty state.
* **`12`** - **Get Value at Index:** View the number at a specific position without removing it.
* **`13`** - **Display All:** Prints all elements in the list from first to last.

*(Note: The program handles invalid inputs, such as letters or out-of-bounds indices, without crashing).*

---

## 🛠️ Project Structure

* `linked_list.h` - Contains the `node_t` and `linked_list_t` struct definitions, along with 16 function prototypes.
* `linked_list.c` - Core implementation of all list operations (push, pop, insert, remove, clear, access).
* `main.c` - An extensive interactive Command Line Interface (CLI) covering all 13 required operations with strict user input validation.

---

## 🚀 How to Compile and Run

Navigate to this directory and compile the program using `gcc`:

```bash
gcc main.c linked_list.c -o linked_list