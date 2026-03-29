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

## 🛠️ Project Structure

* `linked_list.h` - Contains the `node_t` and `linked_list_t` struct definitions, along with 16 function prototypes.
* `linked_list.c` - Core implementation of all list operations (push, pop, insert, remove, clear, access).
* `main.c` - An extensive interactive Command Line Interface (CLI) covering all 13 required operations with strict user input validation.

---

## 🚀 How to Compile and Run

Navigate to this directory and compile the program using `gcc`:

```bash
gcc main.c linked_list.c -o linked_list