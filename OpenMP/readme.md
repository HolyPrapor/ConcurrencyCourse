| # Threads | User (s) | System (s) | Elapsed Time (s) | CPU Usage (%) |
|-----------|----------|------------|------------------|---------------|
| 1         | 60.02    | 0.02       | 60.05            | 99            |
| 2         | 60.51    | 0.00       | 45.31            | 133           |
| 4         | 60.69    | 0.02       | 26.40            | 229           |
| 6         | 62.32    | 0.03       | 18.79            | 331           |
| 8         | 64.60    | 0.02       | 14.75            | 437           |
| 10        | 69.93    | 0.01       | 12.84            | 544           |
| 12        | 75.92    | 0.05       | 11.58            | 655           |
| 14        | 82.62    | 0.00       | 10.88            | 758           |
| 16        | 90.01    | 0.00       | 10.02            | 916           |

Each row corresponds to a separate run of the find_palindrome with a different number of threads.

- `# Threads`: This is the number of threads involved in the computation.
- `User (s)`: refers to the total amount of CPU time spent in user-mode within a process. This is the time spent performing program actions, excluding time the CPU spent performing system calls.
- `System (s)`: the amount of CPU time the process spent in the kernel within the operating system, i.e., the amount of time that the CPU was executing system calls.
- `Elapsed Time (s)`: This is the total real time spent. 
- `CPU Usage (%)`: This shows how much of the CPU's capacity was used by the process ((user + system) / elapsed).

We see that as the number of threads increases, the User + System times increase as well, while Elapsed time significantly decreases. 

One note to make here is that since we divide all the work between the threads, the increase in user time is solely due to the overhead of managing more threads.

Another interesting observation is that there is a point of diminishing returns, and increasing the number of threads beyond that point yields more time spent in User time. We can clearly see that being the case starting from 6 threads. Before that point, the increase in User time is insignificant. Most likely, this is due to the critical section in the print routine, which can be avoided by using some kind of multiple producer lock-free queue, since we do not care about the order of results anyways. As the number of threads increases, the more time processes spend for synchronisation (which might be the case if the critical section implementation is spinlock-based) and thread management. 
