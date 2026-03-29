# 📡 Asynchronous Signal-Driven Logger

> A highly advanced, thread-safe logging library for Linux. It runs asynchronously using pthreads and is controlled entirely from the outside via POSIX Real-time Signals (`SIGRTMIN`).

---

## ✨ Key Features & Technical Highlights

This project demonstrates expert-level Linux systems programming, focusing on concurrency, Inter-Process Communication (IPC), and real-time execution.

* **Real-Time Signal Control:** The logger is fully detached from the main application flow. It listens for `SIGRTMIN`, `SIGRTMIN+1`, and `SIGRTMIN+2` signals sent from the OS (e.g., via the `kill` or `sigqueue` commands) to dynamically change its behavior during runtime without restarting the app.
* **Asynchronous Threading:** Signal handlers (`sa_sigaction`) are kept strictly minimal and non-blocking (they only post semaphores). The actual heavy lifting (file I/O, state dumping) is delegated to three dedicated background worker threads (`pthread_t`), preventing signal handler deadlocks and maintaining application performance.
* **Thread Safety & Atomics:** * File I/O operations are protected by Mutexes (`pthread_mutex_t`) to prevent race conditions when multiple application threads try to log simultaneously.
  * Shared state variables (like logging level or active status) use `<stdatomic.h>` (`atomic_load`, `atomic_store`) for lock-free, thread-safe reads and writes across threads.
* **Dynamic Log Levels:** Supports three verbosity levels (`MIN`, `STANDARD`, `MAX`). The `zapisz_log` function uses variadic arguments (`stdarg.h`) to format strings exactly like `printf`, but it dynamically drops logs that don't meet the current verbosity threshold.
* **Application State Dumping:** Upon receiving a specific signal, a dedicated thread wakes up and generates a unique `dump_YYYY-MM-DD_HH-MM-SS.txt` file containing the current internal state of the application.

---

## 🛠️ Project Structure

* `log.h` - Library header defining the API, signals, and `PoziomSzczegolowosci` enum.
* `log.c` - The core engine: thread loops, signal handlers, semaphores, mutexes, and file I/O logic.
* `main.c` - A demonstration program that simulates a working application (looping and generating logs) while printing its PID to allow external signal injection.

---

## 🎮 How to Test and Use (Linux Only)

*This project relies heavily on POSIX signals and pthreads. It must be compiled and run on a Linux environment (or WSL).*

### 1. Compilation
Navigate to the directory and compile using `gcc` with the `-pthread` flag:
```bash
gcc main.c log.c -o logger_app -pthread