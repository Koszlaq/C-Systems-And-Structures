# 🔄 Circular Buffer (Ring Buffer)

> A highly efficient, fixed-capacity circular buffer implementation in C. Designed for continuous data streams where old data is automatically overwritten once the capacity is reached.

---

## ✨ Key Features & Technical Highlights

This structure is a staple in embedded systems and operating system design (e.g., I/O buffering), demonstrating advanced pointer and index management.

* **Constant Time Operations ($O(1)$):** Thanks to modulo arithmetic (`index % capacity`), the buffer "wraps around" its allocated memory. Elements are never physically shifted, ensuring that all `push` and `pop` operations execute in constant $O(1)$ time.
* **Automatic Overwrite Policy (FIFO):** When a new element is pushed into a fully occupied buffer, it safely overwrites the oldest element and advances the `begin` tracker, preventing overflow crashes and ensuring the buffer always holds the most recent data.
* **Bit-field Optimization:** Utilizes a 1-bit bit-field (`unsigned char full: 1;`) inside the struct to track the buffer's state (full vs empty) when the `begin` and `end` indices overlap, minimizing memory footprint.
* **Double-Ended Extraction:** Supports retrieving data from both ends: `pop_front` (the oldest data) and `pop_back` (the most recently added data).

---

## 🎮 How to Use the Program (Menu Guide)

When you launch the program, it will first ask for the buffer's capacity: `Podaj rozmiar bufora:` (Enter buffer size).

Once initialized, the menu `Co chesz zrobic?` (What do you want to do?) will appear. Enter a number to execute an action:

* **`0`** - **Exit:** Safely frees the allocated memory and terminates the application.
* **`1`** - **Push Back:** Add a number to the buffer. If the buffer is full, it will overwrite the oldest element.
* **`2`** - **Pop Back:** Remove and display the *newest* element (the one added last).
* **`3`** - **Pop Front:** Remove and display the *oldest* element.
* **`4`** - **Display All:** Prints the current valid contents of the buffer from oldest to newest.
* **`5`** - **Check Empty:** Returns `1` if the buffer is empty, `0` otherwise.
* **`6`** - **Check Full:** Returns `1` if the buffer is completely full, `0` otherwise.

The program includes strict input validation and memory checking:
* Entering non-numeric data terminates the program safely (Exit Code `1`).
* Insufficient system memory returns `Failed to allocate memory` (Exit Code `8`).
* Popping from or displaying an empty buffer triggers a safe `Buffer is empty` prompt.

---

## 🛠️ Project Structure

* `circular_buffer.h` - Contains the `circular_buffer_t` struct (with dynamic `ptr`, indices, capacity, and `full` flag) alongside function prototypes.
* `circular_buffer.c` - Core modulo-based logic for continuous buffer operations.
* `main.c` - A continuous CLI for real-time monitoring of the buffer's state and testing wrap-around behavior.

---

## 🚀 Compilation

Navigate to this directory and compile the program using `gcc`:

```bash
gcc main.c circular_buffer.c -o circular_buffer