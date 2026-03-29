# 📡 Asynchronous Signal-Driven Logger

> A highly advanced, thread-safe logging library for Linux. It runs asynchronously using pthreads and is controlled entirely from the outside via POSIX Real-time Signals (`SIGRTMIN`).

---

## ✨ Key Features & Technical Highlights

This project demonstrates expert-level Linux systems programming, focusing on concurrency, Inter-Process Communication (IPC), and real-time execution.

* **Real-Time Signal Control:** The logger is fully detached from the main application flow. It listens for real-time signals (`SIGRTMIN`, `SIGRTMIN+1`, `SIGRTMIN+2`) sent from the OS to dynamically change its behavior during runtime without blocking or restarting the app.
* **Asynchronous Threading:** Signal handlers (`sa_sigaction`) are kept strictly minimal and non-blocking (they only post semaphores). The actual heavy lifting (file I/O, state dumping) is delegated to three dedicated background worker threads (`pthread_t`), preventing deadlocks.
* **Thread Safety & Atomics:** File I/O operations are protected by Mutexes (`pthread_mutex_t`). Shared state variables (like logging level or active status) use `<stdatomic.h>` for lock-free, thread-safe memory access across threads.
* **Dynamic Log Levels:** Supports three verbosity levels (`MIN`, `STANDARD`, `MAX`). The function uses variadic arguments (`stdarg.h`) to format strings exactly like `printf`, but dynamically drops logs that don't meet the current verbosity threshold.
* **Application State Dumping:** Upon receiving a specific signal, a dedicated thread wakes up and generates a unique `dump_YYYY-MM-DD_HH-MM-SS.txt` file containing the exact internal state of the application at that microsecond.

---

## 🛠️ Project Structure

* `log.h` - Library header defining the API, signals, and `PoziomSzczegolowosci` enum.
* `log.c` - The core engine: thread loops, signal handlers, semaphores, mutexes, and file I/O logic.
* `main.c` - A demonstration program that simulates a working application (looping and generating logs) while printing exact terminal commands to allow external signal injection.

---

## 🎮 How to Operate the App (Signal Guide)

*Note: This project relies heavily on POSIX signals and pthreads. It must be compiled and run on a Linux environment (or WSL).*

### 1. Start the Application
Compile the program with the `-pthread` flag and run it:

gcc main.c log.c -o logger_app -pthread
./logger_app

### 2. Read the Generated Commands
Unlike standard CLI apps, this logger runs continuously in the background. Upon startup, it will print its `PID` and a list of exact terminal commands you can use to control it. **Leave this terminal running.**

*Example output:*
PID programu: 12345
Sygnaly do testow:
sigqueue 12345 34 0  # MIN
sigqueue 12345 34 1  # STANDARD
sigqueue 12345 34 2  # MAX
kill -35 12345  # toggle logowania
kill -36 12345  # dump

### 3. Send Control Signals (Open a second terminal)
Open a **new, second terminal window**. Copy and paste the commands provided by the app to control the logger in real-time:

* **Change Log Level:** Paste `sigqueue [PID] [SIGRTMIN] [LEVEL]` to change verbosity (0=MIN, 1=STD, 2=MAX).
* **Toggle Logging On/Off:** Paste `kill -[SIGRTMIN+1] [PID]` to pause or resume writing to `aplikacja.log`.
* **Generate State Dump:** Paste `kill -[SIGRTMIN+2] [PID]` to instantly generate a `dump_*.txt` file with the application's current internal state.

Watch the first terminal and the generated `.log` and `.txt` files to see the asynchronous engine react instantly!