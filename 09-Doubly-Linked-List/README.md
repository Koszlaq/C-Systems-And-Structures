# ↔️ Doubly Linked List Implementation

> A dynamic, bi-directional linked list data structure in C. This implementation allows efficient traversal in both directions (forward and backward) using `next` and `prev` pointers.

---

## ✨ Key Features & Technical Highlights

* **Bi-directional Traversal:** Every node maintains a pointer to both its successor (`next`) and predecessor (`prev`). This allows operations like reverse displaying or moving backwards through the structure.
* **Optimized End-Operations:** By maintaining a `tail` pointer, appending elements to the end of the list (`push_back`) and retrieving from the end is done in constant `O(1)` time.
* **Strict Memory Management:** Handling bidirectional links during insertions and deletions (especially in the middle of the list) requires complex pointer updates to avoid memory leaks or broken links. This project safely implements these operations.
* **Comprehensive CLI:** Includes a fully featured, interactive menu to test every possible operation safely, with robust input buffer clearing to prevent infinite loops on invalid inputs.

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
* **`9`** - **Check if Empty:** Returns `1` if the list has no elements, `0` otherwise.
* **`10`** - **Get Size:** Displays the total number of elements currently in the list.
* **`11`** - **Clear List:** Deletes all elements and resets the list to an empty state.
* **`12`** - **Get Value at Index:** View the number at a specific position without removing it.
* **`13`** - **Display All:** Prints all elements in the list from first to last (Left to Right).
* **`14`** - **Display Reverse:** Prints all elements in the list from last to first (Right to Left) using `prev` pointers.

---

## 🛠️ Project Structure

* `doubly_linked_list.h` - Contains the structural definitions (`doubly_linked_list_t` and bi-directional `node_t`) and all function prototypes.
* `doubly_linked_list.c` - Core logic for traversing, linking, and unlinking nodes in both directions without memory leaks.
* `main.c` - An extensive interactive Command Line Interface (CLI) covering all 15 menu options (0-14) with strict user input validation.

---

## 🚀 Compilation

Navigate to this directory and compile the program using `gcc`:

```bash
gcc main.c doubly_linked_list.c -o dll