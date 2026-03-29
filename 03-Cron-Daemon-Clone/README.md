# 🕒 Cron Daemon Clone (Task Scheduler)

> A robust, client-server task scheduling daemon replicating the core functionality of the Unix `cron` utility. Built with POSIX Message Queues, Interval Timers, and integrated asynchronous logging.

---

## ✨ Key Features & Technical Highlights

This project is a deep dive into Linux Systems Programming, demonstrating advanced Inter-Process Communication (IPC) and process lifecycle management.

* **Client-Server Architecture (Singleton):** The compiled binary acts as both the server (daemon) and the client. The first execution initializes the background server. It uses file locking (`flock` on `/tmp/cron_server.lock`) to guarantee that only a single server instance can run at any given time. Subsequent executions act as clients.
* **POSIX Message Queues (`mqueue.h`):** All client-server communication (submitting tasks, requesting lists, canceling tasks) is securely routed through non-blocking message queues.
* **Advanced POSIX Timers:** Relies on `timer_create` and `timer_settime` using `SIGEV_THREAD`. Instead of busy-waiting, the OS kernel notifies the daemon exactly when a task needs to run by spawning a lightweight thread.
* **Efficient Process Spawning:** Utilizes `posix_spawn` instead of the traditional `fork()`/`exec()` combo to launch scheduled external programs with their arguments safely and efficiently.
* **Integrated Logger:** Reuses a custom-built, thread-safe, signal-driven asynchronous logger to record the daemon's activity seamlessly in `/tmp/cron_server.log`.

---

## 🛠️ Project Structure

* `MyCron.h` & `MyCron.c` - Main entry point. Determines whether to launch the server or act as a CLI client parsing user arguments.
* `cron_server.h` & `cron_server.c` - The daemon engine. Handles the incoming message queue, task array mutexes, timer callbacks, and `posix_spawn` executions.
* `cron_client.h` & `cron_client.c` - Client-side logic for packaging CLI arguments into structs and sending them via `mq_send` to the server.
* `log.h` & `log.c` - The asynchronous logging library.

---

## 🎮 How to Use the Scheduler

### 1. Start the Server (Daemon)
Run the program without any arguments. It will start in the background and wait for tasks.
`./mycron`

### 2. Client Commands (Scheduling Tasks)
Open another terminal (or use the same one, as the server runs independently) and use the following commands:

* **List all active tasks:** `./mycron lista`
* **Add a Relative Task:** (e.g., Run `ls -l` after 10 seconds)
  `./mycron dodaj-wzglednie 10 /bin/ls -l`
* **Add an Absolute Task:** (Run at a specific Unix Epoch timestamp)
  `./mycron dodaj-bezwzglednie 1700000000 /bin/echo "Hello"`
* **Add a Cyclic Task:** (e.g., Start in 5s, repeat every 60s)
  `./mycron dodaj-cyklicznie 5 60 /bin/date`
* **Cancel a Task:** `./mycron anuluj [TASK_ID]`
* **Shutdown the Server:** `./mycron wylacz`

---

## 🚀 Compilation

**Critical Note for Linux users:** You must link both the POSIX Threads library (`-pthread`) and the Real-Time library (`-lrt`) which is required for `mq_open` and `timer_create`.

`gcc MyCron.c cron_client.c cron_server.c log.c -o mycron -pthread -lrt`