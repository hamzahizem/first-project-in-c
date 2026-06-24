# Single-Process TCP Relay Server (`poll`)

A lightweight C-based TCP relay server designed to bridge communication between two network clients. 

### The Architecture Shift: `fork()` vs `poll()`
Originally, this project split the workload into parent and child processes using `fork()`, which required shared memory (`mmap`) to sync connection states. 

This version completely removes multi-processing in favor of **I/O Multiplexing via `poll()`**. The entire application now monitors the listening port and all active client connections within a **single thread**, drastically simplifying the codebase and removing cross-process race conditions.

## Key Improvements
* **No More `fork()` Overhead:** Zero process creation cost and zero need for shared memory allocations.
* **Dynamic Slot Re-use:** If a client disconnects, its slot in the poll array is set back to `-1`. The server immediately accepts a new incoming connection to fill that empty space without affecting the remaining client.
* **Safe Memory Management:** Replaced string-based tracking (`strlen`) with explicit raw network byte sizing to prevent buffer overflows and memory corruption.

## How to Run

1. **Compile:**
   ```bash
   gcc -o relay_server main.c

   Run the Server:
   
    ./relay_server 8080

    Connect Two Clients:
    Open two separate terminals and bridge them using netcat:
    Bash

    nc localhost 8080


This clearly explains to anyone looking at your GitHub that you upgraded the project's design to a cleaner, production-standard approach! Ready to push it to your repository?
