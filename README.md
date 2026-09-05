*This project has been created as part of the 42 curriculum by msbii.*

## Description
Codexion is a concurrency simulation where multiple coders compete for limited resources (USB dongles) in a shared workspace. Coders alternate between compiling, debugging, and refactoring. To compile their quantum code, they must hold two dongles simultaneously. If a coder fails to start compiling before their `time_to_burnout` expires, they burn out, and the simulation stops. The core objective is to orchestrate thread synchronization, implement fair scheduling (FIFO or EDF), and prevent deadlocks or starvation.

## Instructions
The project compiles cleanly using `cc` with the flags `-Wall -Wextra -Werror -pthread`.

**Compilation:**
Run the following command at the root of the repository to generate the executable:
`make`

**Execution:**
The program takes exactly 8 mandatory arguments:
`./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler`

*   `scheduler` must be strictly either `fifo` or `edf`.
*   Example: `./codexion 5 3000 200 200 200 10 400 fifo`

## Resources
*   **Documentation:** POSIX Threads (`pthread`) manual pages, specifically for mutex management and thread lifecycle.
*   **Concepts:** Edsger W. Dijkstra's Dining Philosophers problem, Coffman conditions for deadlock prevention, and Earliest Deadline First (EDF) scheduling algorithms.
*   **AI Usage:** Artificial Intelligence was utilized as a learning resource to grasp complex multithreading concepts, understand the theory behind deadlock prevention and scheduling algorithms, and learn the underlying mechanics of thread-safety analysis tools like Helgrind and ThreadSanitizer.

## Blocking cases handled
This project addresses several critical concurrency and synchronization issues:
*   **Deadlock Prevention (Coffman's Conditions):** A strict hierarchical lock-ordering mechanism (`order_dongles`) forces all coders to always lock the lower-indexed dongle first, mathematically eliminating the risk of circular wait deadlocks.
*   **Starvation Prevention & Fair Arbitration:** Handled via a centralized priority queue (Heap) enforcing either strict First-In, First-Out (FIFO) arrival order or Earliest Deadline First (EDF) arbitration based on burnout proximity.
*   **Cooldown Handling:** Dongle availability is strictly managed via a timestamp mechanism (`dongles_status`) ensuring a released dongle cannot be taken again until the mandatory `dongle_cooldown` period has elapsed.
*   **Precise Burnout Detection:** A dedicated monitor thread polls coder states continuously (`usleep(1000)`), ensuring that a burnout log is detected and printed within 10 ms of the actual deadline.
*   **Log Serialization:** interleaved outputs are prevented by double-locking (`print_mutex` and `simulation_mutex`). This guarantees the "burned out" message is always the absolute last line printed.
*   **Edge Case (1 Coder):** Handled gracefully. A single coder will take the only available dongle, realize they cannot compile, and wait safely for burnout without causing a segmentation fault or modulo arithmetic error.

## Thread synchronization mechanisms
The simulation uses `pthread_mutex_t` as its primary synchronization primitive to safely coordinate access to shared resources and prevent data races:
*   **Resource Mutexes (`data->dongles`):** An array of mutexes directly protects the state of each USB dongle, ensuring a single dongle is never held by two coders at the same time.
*   **State Mutex (`coder->state_mutex`):** Protects the internal data of each coder (e.g., `last_compile_time`, `compile_count`) so the monitor thread can read these values safely without race conditions while the coder thread is updating them.
*   **Simulation Mutex (`data->simulation_mutex`):** Serves as a global lock to protect the shared queue, dongle cooldown states, and the `simulation_running` boolean.
*   **Print Mutex (`data->print_mutex`):** Ensures absolute serialization of terminal output.
*   **Micro-polling over Conditions:** To avoid OS-level internal signal false positives detected by `valgrind --tool=helgrind`, `pthread_cond_t` was replaced by a highly controlled micro-polling mechanism. Coders release the global state lock, perform a `usleep(500)` micro-pause, and reacquire the lock to check resource availability.
