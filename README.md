# Single-Process TCP Relay Room (`poll`) & Duplex Client

A complete C-based client-server networking utility designed to bridge communication between two remote network users. 

The project features a high-performance **I/O Multiplexed Server** built with `poll()` and a **Full-Duplex Client** utilizing `fork()`.

## System Architecture

### 1. The Relay Server (`poll`)
The server acts as a central room. It completely removes the overhead of multi-processing by running entirely inside a **single thread**. 
* **I/O Multiplexing:** Using `poll()`, it monitors the main listening port and up to 2 active client file descriptors simultaneously without blocking.
* **Dynamic Slot Re-use:** If a client disconnects, its index slot is set back to `-1`. The server immediately accepts a new incoming connection to fill that empty space without affecting the remaining client's session.
* **Safe Byte Forwarding:** Replaced string-based tracking (`strlen`) with raw network byte sizing to prevent buffer overflows and memory corruption.

### 2. The Chat Client (`fork`)
The client connects directly to the relay server using a hostname and port.
* **Dual-Process Duplexing:** It utilizes `fork()` to split into a parent and child process. 
  - **Parent Process:** Continuously reads incoming messages from the socket and prints them to the terminal. If the server goes down, it cleanly kills the child process using `SIGTERM`.
  - **Child Process:** Continously listens to user input via `stdin` (`fgets`) and sends it over the wire instantly.

---

## How to Get Started

### 1. Compile Both Files
```bash
# Compile the Server
gcc -o relay_server server.c

# Compile the Client
gcc -o chat_client client.c
