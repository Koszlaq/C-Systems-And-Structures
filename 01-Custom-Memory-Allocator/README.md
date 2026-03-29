# 🧠 Custom Memory Allocator

> A complete from-scratch implementation of a dynamic memory allocator (replacing the standard `malloc`, `calloc`, `realloc`, and `free`). It manages a simulated heap via `sbrk` system call equivalents and includes advanced memory corruption detection.

---

## ✨ Key Features & Technical Highlights

* **Full Allocation API:** Accurately mimics the GNU C Library (glibc) behavior for `malloc`, `calloc`, `realloc`, and `free`.
* **Dynamic Heap Expansion:** Automatically requests more memory pages (4KB chunks) from the OS using a simulated `sbrk` system call (`custom_sbrk`) when the current heap is exhausted.
* **Memory Corruption Protection (Fences):** Every allocated block is enclosed by protective "fences" (boundary tags made of `#` characters). The allocator actively monitors these fences to detect off-by-one errors or buffer overflows caused by user space code.
* **Control Block Checksums:** Heap metadata (control structs linking the memory blocks) is protected by mathematical checksums. If a user accidentally overwrites the heap's linked list pointers, `heap_validate()` detects the anomaly and prevents a segmentation fault.
* **Automatic Coalescing:** The `heap_free` function automatically detects adjacent free blocks and merges (defragments) them to prevent severe memory fragmentation over time.
* **Pointer Validation:** Features a robust `get_pointer_type` function that categorizes any given memory address (e.g., pointing inside a fence, inside metadata, or unallocated space), preventing invalid free operations.

---

## 🛠️ Project Structure

* `heap.h` - Contains the allocator API, control structures (`menadzer_sterty`, `blok_sterty`), and pointer type enums.
* `heap.c` - Core logic for memory traversal, splitting, coalescing, fence generation, and checksum validation.
* `custom_unistd.h` - Simulates the POSIX `sbrk()` system call to request raw memory pages without desynchronizing the real C standard library.

---

## 🎮 How to Use the Allocator

Unlike standard `malloc`, this allocator requires initialization and cleanup to map and unmap the simulated heap.

### 1. Initialize the Heap
Call this function at the very beginning of your program:
`heap_setup();`

### 2. Allocate and Reallocate Memory
Use the custom API exactly as you would use the standard C library:
`void* ptr = heap_malloc(100);`
`void* ptr2 = heap_calloc(10, sizeof(int));`
`ptr = heap_realloc(ptr, 200);`

### 3. Validate State (Debugging)
Call `heap_validate()` at any time to check the integrity of the heap. 
It returns:
* `0` - Heap is healthy.
* `1` - Fences are corrupted (Buffer overflow detected).
* `3` - Internal control structures are damaged.

### 4. Free Memory
`heap_free(ptr);`

### 5. Destroy the Heap
Call this at the end of your program to return all memory to the OS and completely reset the state:
`heap_clean();`

---

## 🚀 Compilation

Navigate to this directory and compile the program using `gcc`. You must include the custom `sbrk` implementation provided with the project.

gcc main.c heap.c custom_sbrk.c -o custom_malloc