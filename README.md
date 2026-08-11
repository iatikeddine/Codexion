*This project has been created as part of the 42 curriculum by iatikedd.*

# Codexion

## Description
**Codexion** is a multi-threading simulation project where multiple coders compete for limited resources (dongles) to compile their quantum code. The project models a scenario where coders sit at a circular table and must alternate between compiling, debugging, and refactoring. The core challenge is to ensure fair resource allocation, prevent data races, and guarantee that no coder "burns out" due to starvation or deadlocks. 

The simulation supports two scheduling policies:
- **FIFO (First In, First Out)**: Grants dongle access based on arrival order.
- **EDF (Earliest Deadline First)**: Grants dongle access based on the coder closest to burning out.

## Instructions

### Compilation
A `Makefile` is provided to compile the project. You can compile the program using the following commands:
- `make` or `make all`: Compiles the `codexion` executable.
- `make clean`: Removes object files.
- `make fclean`: Removes object files and the executable.
- `make re`: Recompiles the entire project.

### Execution
Run the compiled executable with the following mandatory arguments:
```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

**Arguments Explanation:**
- `number_of_coders`: Number of coders (and number of dongles).
- `time_to_burnout`: Milliseconds a coder can go without compiling before they burn out.
- `time_to_compile`: Milliseconds spent compiling (requires two dongles).
- `time_to_debug`: Milliseconds spent debugging.
- `time_to_refactor`: Milliseconds spent refactoring.
- `number_of_compiles_required`: Number of times all coders must compile for the simulation to end successfully.
- `dongle_cooldown`: Milliseconds a dongle must cool down after being released.
- `scheduler`: The arbitration policy, either `fifo` or `edf`.

**Example:**
```bash
./codexion 6 800 200 200 200 7 100 fifo
```

## Blocking cases handled
This project tackles several critical concurrency issues to guarantee liveness and stability:

- **Deadlock Prevention (Circular Wait)**: Deadlocks are entirely prevented by utilizing a global gatekeeper mutex (lock_sim). Before any coder can attempt to acquire their left and right dongles, they must first hold the lock_sim mutex.
- **Starvation Prevention**: Fair access to dongles is enforced through a strict queue-based scheduling mechanism. Depending on the `scheduler` argument, dongle access is granted strictly in `fifo` (arrival time) or `edf` (earliest deadline first) order, ensuring no coder is left waiting indefinitely.
- **Cooldown Handling**: Every dongle has an `is_available` state and a `cooldown` timestamp. When a dongle is dropped, its cooldown timestamp is updated. The scheduling condition (`both_ready`) strictly checks the current time against the cooldown timestamp before a dongle can be reacquired.
- **Precise Burnout Detection**: A dedicated monitor thread runs independently of the coders, constantly checking the `last_compile_time` of each coder. When a burnout is detected, the monitor sets a global state flag and broadcasts to all sleeping threads to shut down the simulation gracefully within the required 10ms tolerance.
- **Log Serialization**: Console output is protected by a dedicated `print_lock` mutex to ensure that state change messages never interleave or overlap on the terminal.

## Thread synchronization mechanisms

This implementation relies on POSIX threads (`pthread`) and synchronization primitives:

- **Threads (`pthread_create`)**: Each coder runs in its own independent thread to simulate parallel execution, alongside a single monitor thread that supervises their health.
- **Mutexes (`pthread_mutex_t`)**: 
  - **Dongle Locks**: Each dongle has its own mutex to protect its priority queue, availability state, and cooldown timestamp.
  - **State Lock (`state_lock`)**: Protects global shared data, such as the simulation ending flag and the last compile timestamps of coders, to prevent data races during burnout checks.
  - **Print Lock (`print_lock`)**: Guarantees mutually exclusive access to `stdout` for clean, serialized logging.
  - **Simulation Lock (`lock_sim`)**: Acts as a coarse-grained gatekeeper to ensure acquisition of dongles and safe condition variable waiting.
- **Condition Variables (`pthread_cond_t`)**: 
  - A central `wake_cond` condition variable is used to make coders sleep (`pthread_cond_wait`) while they are waiting for dongles. When dongles are released or when the monitor detects a burnout, a `pthread_cond_broadcast` is signaled, instantly waking up all waiting threads to re-evaluate their state or exit cleanly.

## Resources
- **Classic References**:
  - Unix threads in C by CodeVault (https://www.youtube.com/watch?v=d9s_d28yJq0&list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2)
  - multithreading necessary function from (https://man7.org)
- **AI Usage**: AI was utilized as a assistant to resolve strict formatting requirements (Norminette compliance), and for in depth explination of multithreading concepts.
