# 💽 FAT12 File System Reader

> A low-level, read-only file system parser for FAT12 volume images. Built entirely from scratch in C, this project simulates how an operating system mounts and traverses a block device.

---

## ✨ Key Features & Technical Highlights

This project bypasses standard high-level file I/O operations (like `fopen` on actual files) to read raw binary disk images directly, demonstrating a profound understanding of computer architecture and OS design.

* **Binary Container Parsing:** The project is a dedicated parser for file containers formatted in the FAT12/16 standard.
* **Block Device Abstraction:** It features a custom API (`disk_open_from_file`, `disk_read`, `disk_close`) that treats a file as a raw block device, reading data in strict 512-byte sector chunks.
* **Volume Management:** Implements `fat_open` and `fat_close` to validate the Boot Sector (checking the `0xaa55` signature) and load the File Allocation Tables (FAT1 and FAT2) into memory.
* **Root Directory Traversal:** Supports opening and reading the root directory (`dir_open`, `dir_read`), extracting metadata such as file names, sizes, starting clusters, and attributes (Hidden, System, Directory).
* **File Operations:** Implements custom `file_open`, `file_seek`, and `file_read` functions. It successfully decodes the FAT12 cluster chain (1.5-byte entries) to read fragmented files across the volume.
* **POSIX Error Handling:** Every function in the API strictly adheres to POSIX standards, communicating failures (like missing files, out of memory, or out of bounds) via the standard `errno` variable (e.g., `ENOENT`, `ENOMEM`, `ENXIO`).
* **Packed Structures:** Uses `__attribute__((__packed__))` extensively to prevent the C compiler from padding structs (like `boot_t` and `dir_entry_pelny_t`), allowing direct memory mapping from the binary disk image to C data structures.

---

## 🛠️ Project Structure

* `file_reader.h` - Contains the heavily packed definitions for the Boot Sector, FAT directories, and cluster chains, alongside the complete API prototypes.
* `file_reader.c` - The core engine responsible for binary parsing, cluster chain decoding (`get_chain_fat12`), and sector-to-byte math.
* `main.c` - An entry point meant for custom user testing.

---

## 🚀 Compilation

Navigate to this directory and compile the program using `gcc`.

gcc main.c file_reader.c -o fat_reader

*(Note: To test this program, you will need a valid FAT12 `.img` disk image to pass into the `disk_open_from_file` function).*