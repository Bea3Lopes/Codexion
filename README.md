*This project has been created as part of the 42 curriculum by besoares.*

# Codexion

## Description

Codexion is a concurrent programming project developed as part of the 42 curriculum. It explores thread management, synchronization and resource sharing using POSIX threads (`pthreads`) in C.

The project simulates a group of coders who need to share a limited number of dongles (resources) to compile their code.

Each coder runs in its own thread and must acquire two adjacent dongles before compiling. After compiling, the coder releases the dongles and proceeds through debugging and refactoring before requesting resources again.

The main goal is to manage concurrent resource requests while preventing deadlocks, handling resource cooldowns and ensuring fair access according to a selected scheduling policy.

Two scheduling algorithms are implemented:

- **FIFO (First In, First Out):** requests are handled according to their arrival order.
- **EDF (Earliest Deadline First):** requests are prioritized according to the coder's earliest burnout deadline, calculated as the last compilation start time plus the allowed time before burnout.

A custom priority heap is used to manage pending requests.

## Instructions

### Requirements

- GCC or another compatible C compiler.
- POSIX threads (`pthreads`).
- Make.

### Compilation

Clone the repository:

```bash
git clone <repository_url>
cd codexion
```

Compile the project:

```bash
make
```

This generates the `codexion` executable.

### Execution

Run the program using the following arguments:

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

Arguments:

| Argument | Description |
|---|---|
| `number_of_coders` | Number of coder threads. |
| `time_to_burnout` | Maximum time a coder can go without starting a compilation. |
| `time_to_compile` | Time required to compile. |
| `time_to_debug` | Time required to debug. |
| `time_to_refactor` | Time required to refactor. |
| `number_of_compiles_required` | Number of successful compilations required per coder. |
| `dongle_cooldown` | Time a dongle remains unavailable after being released. |
| `scheduler` | Scheduling algorithm: `fifo` or `edf`. |

All time values are expressed in milliseconds.

### Example

```bash
./codexion 5 3000 200 200 200 10 800 fifo
```

This starts a simulation with five coders, a burnout limit of 3000 ms, 200 ms for each activity, a requirement of ten compilations per coder, an 800 ms dongle cooldown and FIFO scheduling.

To use EDF scheduling:

```bash
./codexion 5 3000 200 200 200 10 800 edf
```

## Blocking cases handled

The project addresses several concurrency challenges associated with shared resources and simultaneous thread execution.

### Deadlock prevention

To prevent this, coders do not acquire dongles independently. Instead, the dispatcher checks whether both required dongles are available and reserves them together while holding the resource mutex.

This avoids partial resource acquisition and prevents circular waiting between coders.

### Starvation prevention and fair arbitration

A scheduling policy determines which coder receives resources when multiple requests compete.

- FIFO prioritizes requests according to arrival time.
- EDF prioritizes requests according to their burnout deadlines.

EDF also uses a deterministic tie-breaking rule when two requests have identical deadlines.

Under feasible simulation parameters, EDF must ensure that coders are not indefinitely denied access to the resources they need.

### Dongle cooldown handling

After a coder finishes compiling, its dongles become temporarily unavailable.

Each dongle tracks its availability and cooldown expiration time. The dispatcher only grants a request when both required dongles are available and their cooldown periods have expired.

### Burnout detection

A dedicated monitor thread periodically checks the simulation state and the time elapsed since each coder's last compilation started.

If a coder exceeds its allowed time without starting another compilation, the simulation is stopped.

Coders that have already completed their required number of compilations are excluded from burnout checks.

### Log serialization

Multiple threads may attempt to print messages simultaneously.

A dedicated mutex protects output operations, ensuring that log messages remain serialized and readable.

The simulation also prevents further state messages from being printed after termination.

## Thread synchronization mechanisms

The project uses POSIX threading primitives to coordinate coder threads, the monitor thread and access to shared resources.

### `pthread_mutex_t`

Two main mutexes are used:

- **`resource_mutex`:** protects shared simulation state, including dongle availability, cooldown information, pending requests, coder grants, completion counters and the stop flag.
- **`write_mutex`:** protects terminal output, preventing messages from different threads from being interleaved.

This ensures that two coders cannot simultaneously reserve the same dongle.

### `pthread_cond_t`

A condition variable allows coder threads to wait until their resource requests have been granted or the simulation has stopped.
Instead of continuously checking whether their request has been granted, coders wait using `pthread_cond_wait()`.
The dispatcher updates the grant state and broadcasts through `pthread_cond_broadcast()` to notify waiting threads.
Each coder checks its grant state and the simulation stop flag inside a loop while holding the associated mutex. This ensures that notifications are handled safely and that threads do not proceed without receiving permission.

### Custom event mechanism

The project uses coder state flags to represent events such as a resource grant and simulation termination.

The `grant` flag indicates whether a coder has received permission to compile, while the simulation's `stop_flag` indicates whether execution should terminate.

These flags are protected by `resource_mutex` and are combined with the condition variable to provide thread-safe communication between the dispatcher, coder threads and monitor.

### Race condition prevention

Race conditions are prevented by ensuring that shared variables are accessed under the appropriate mutex.

For example:

- Coder completion counters are protected when updated or read.
- Dongle availability and cooldown timestamps are modified under `resource_mutex`.
- The monitor checks coder deadlines while holding the same mutex used by threads that update compilation timestamps.
- The stop flag is always accessed under `resource_mutex`.
- Log messages are serialized using `write_mutex`.

This synchronization ensures consistent access to shared state and predictable communication between threads.

## Resources

### Documentation and references

- [POSIX Threads Programming](https://hpc-tutorials.llnl.gov/posix/)
- [Linux man pages – pthreads](https://man7.org/linux/man-pages/man7/pthreads.7.html)
- [pthread_mutex_lock](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)
- [pthread_cond_wait](https://man7.org/linux/man-pages/man3/pthread_cond_wait.3p.html)
- [Valgrind – Helgrind](https://valgrind.org/docs/manual/hg-manual.html)
- [The Little Book of Semaphores](https://greenteapress.com/wp/semaphores/)

### AI usage

AI tools (ChatGPT) were used as supplementary learning and development resources throughout the project.

They assisted with:

- Understanding thread synchronization concepts, mutexes and condition variables.
- Discussing deadlocks, race conditions and resource arbitration.
- Reviewing the design and responsibilities of functions and data structures.
- Draft this README content.
AI was used as a support tool for learning, reasoning and debugging. The implementation, integration, testing and final technical decisions were carried out by the project author.
