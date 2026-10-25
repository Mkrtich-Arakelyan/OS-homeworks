# Process Scheduling Simulator

A C program for Linux that simulates two non-preemptive scheduling algorithms:

- First Come First Served (FCFS)
- Shortest Job First (SJF)

Each process has one CPU burst. Scheduling uses simulated time units and assumes zero context-switch overhead.

## Compile

```bash
gcc scheduler.c -o scheduler
```

## Run

```bash
./scheduler
```

Enter the number of processes, followed by the arrival time and burst time for each process.

- Number of processes: 1 to 100.
- Arrival time: a non-negative integer.
- Burst time: a positive integer.
- Process IDs are assigned automatically, starting at 1.

Example input:

```text
4
0 8
1 4
2 9
3 5
```

## Scheduling rules

FCFS runs processes in arrival order. Equal arrival times are resolved using input order.

SJF selects the shortest burst among unfinished processes that have already arrived. Equal bursts are resolved by earlier arrival time, then input order.

Both algorithms run each selected process until completion. When no process is ready, the Gantt chart includes an IDLE interval.

## Output

For each algorithm, the program displays:

- A Gantt chart with execution intervals.
- Each process's PID, arrival time, burst time, waiting time, turnaround time, and response time.
- Average waiting, turnaround, and response times.

The calculations are:

- Waiting time = start time - arrival time.
- Turnaround time = completion time - arrival time.
- Response time = first start time - arrival time.

Waiting and response times are equal for this non-preemptive, single-burst simulation.

For the example input:

| Algorithm | Average waiting | Average turnaround | Average response |
|---|---:|---:|---:|
| FCFS | 8.75 | 15.25 | 8.75 |
| SJF | 7.75 | 14.25 | 7.75 |

## Testing

The program was checked using:

- The assignment's sample input.
- Simultaneous arrivals with different and equal burst times.
- Unsorted arrivals with initial and intermediate idle periods.
