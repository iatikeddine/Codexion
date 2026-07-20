*This project has been created as part of the 42 curriculum by iatikedd.*

## Description
Codexion is a concurrency and multi-threading simulation written in C, inspired by the classic Dining Philosophers problem but with a modern collaborative coding twist. The goal of this project is to orchestrate multiple coders competing for limited USB dongles using POSIX threads, mutexes, and smart scheduling without encountering deadlocks, data races, or starvation (burnout). 

Coders sit in a circular hub around a shared Quantum Compiler. They alternate between three states: compiling, debugging, and refactoring. To compile, a coder requires two USB dongles. The program accurately models strict timing, resource limitations, and custom scheduling arbitrations (FIFO and EDF) to ensure fair access.

## Instructions

### Compilation
The project includes a `Makefile` that compiles the source files using `cc` with the required flags (`-Wall -Wextra -Werror -pthread`).
To compile the project, run:
```bash
make
```
Other available rules:
- `make clean`: Removes object files.
- `make fclean`: Removes object files and the executable.
- `make re`: Recompiles the entire project.

### Execution
Run the executable with the following mandatory arguments:
```bash
./codexion [number_of_coders] [time_to_burnout] [time_to_compile] [time_to_debug] [time_to_refactor] [number_of_compiles_required] [dongle_cooldown] [scheduler]
```
- **number_of_coders**: The number of coders and dongles.
- **time_to_burnout**: Milliseconds a coder can wait since the start of their last compile before burning out.
- **time_to_compile**: Milliseconds spent compiling (requires 2 dongles).
- **time_to_debug**: Milliseconds spent debugging.
- **time_to_refactor**: Milliseconds spent refactoring.
- **number_of_compiles_required**: Simulation stops if all coders compile this many times.
- **dongle_cooldown**: Milliseconds a dongle remains unavailable after being released.
- **scheduler**: Arbitration policy, either `fifo` or `edf`.

Example:
```bash
./codexion 100 1000 200 100 100 5 100 fifo
```

## Blocking cases handled

Our solution robustly manages several advanced concurrency challenges:
- **Deadlock prevention (Coffman's conditions):** Deadlocks are structurally prevented by implementing an asymmetrical acquisition strategy. Even-numbered coders request their right dongle first, then left; odd-numbered coders request their left dongle first, then right. This breaks the circular wait condition.
- **Starvation prevention:** To prevent an initial race condition where a coder might hold one dongle hostage while waiting for a second (causing their neighbor to starve), an initial sub-millisecond delay (`usleep(15000)`) is introduced for even coders. This enforces a perfectly alternating desynchronized rhythm.
- **Cooldown handling:** Dongles enforce a mandatory cooldown period upon release. Coders attempting to acquire a cooling dongle are placed into a conditional wait queue (`pthread_cond_timedwait`) until the cooldown timestamp perfectly expires.
- **Precise burnout detection:** A dedicated monitor thread runs independently, periodically checking every coder's `last_compile_time`. CPU hogging is avoided with a micro-sleep (`usleep(1000)`), yielding execution back to the OS scheduler and ensuring burnout logs print within the strict 10ms tolerance.
- **Log serialization:** The standard output is protected by a dedicated `print_lock` mutex to guarantee that log messages from highly concurrent threads never interleave, tear, or print out of sequence. Moreover, logs are checked against the simulation's `ending_point` flag to strictly prevent any logs from printing after a burnout event occurs.

## Thread synchronization mechanisms

The project leverages POSIX threading primitives to coordinate access to shared resources efficiently:
- **`pthread_mutex_t`**: Used to ensure mutual exclusion. There is one mutex per dongle to prevent concurrent access by neighboring coders. Global mutexes (`state_lock` and `print_lock`) are used to protect read/write operations on shared state variables (like the `ending_point` flag and compile counts) and the standard output.
- **`pthread_cond_t`**: Condition variables manage custom wait queues for each dongle. When a coder requests a busy or cooling dongle, they call `pthread_cond_wait` (or `pthread_cond_timedwait` for cooldowns). When a coder drops a dongle, they call `pthread_cond_broadcast` to cleanly wake up queued threads.
- **Custom Arbitration (FIFO/EDF)**: A priority queue structure dictates the order in which waiting threads receive dongles. Condition variables are wrapped in a `while` loop that checks the `is_available` flag and ensures only the thread currently at the front of the queue (`requests[0]`) is allowed to break the loop and lock the dongle.
- **Thread-safe communication**: Coders do not communicate directly with each other or the monitor. The monitor thread safely observes the simulation status by acquiring the `state_lock`, copying the values (like `last_compile_time`), and releasing the lock immediately to minimize contention. If a burnout is detected, the monitor sets `ending_point = 't'` under the lock, which safely signals all coder threads to gracefully exit their loops.

## Resources
- [POSIX Threads Programming (LLNL)](https://hpc-tutorials.llnl.gov/posix/)
- [Dining Philosophers Problem (Wikipedia)](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [Mutexes and Condition Variables](https://pages.cs.wisc.edu/~remzi/OSTEP/threads-cv.pdf)

**AI Usage:** 
AI (Google DeepMind Antigravity) was used extensively in a pair-programming setup to assist with:
- **Concurrency debugging**: Identifying and patching highly complex starvation scenarios and deadlocks (e.g., analyzing the "hostage dongle" livelock condition with `edf` and `fifo`).
- **Norm compliance**: Restructuring `main.c` and `monitor.c` to adhere to the strict 42 Norminette 25-line function limit without sacrificing logic readability.