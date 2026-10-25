#include <stdio.h>

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int turnaround_time;
    int waiting_time;
    int response_time;
};

void fcfs(struct Process input[], int n) {
    struct Process p[100];

    /* Copy the original processes. */
    for (int i = 0; i < n; i++) {
        p[i] = input[i];
    }

    /* Sort by arrival time, preserving input order for ties. */
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].arrival_time > p[j + 1].arrival_time) {
                struct Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }

    int time = 0;
    double total_waiting = 0;
    double total_turnaround = 0;
    double total_response = 0;

    printf("\n=== First Come First Served (FCFS) ===\n");
    printf("Gantt chart:\n");

    for (int i = 0; i < n; i++) {
        /* Wait if the next process has not arrived. */
        if (time < p[i].arrival_time) {
            printf("| IDLE [%d-%d] ", time, p[i].arrival_time);
            time = p[i].arrival_time;
        }

        int start = time;
        time += p[i].burst_time;

        p[i].waiting_time = start - p[i].arrival_time;
        p[i].turnaround_time = time - p[i].arrival_time;
        p[i].response_time = p[i].waiting_time;

        printf("| P%d [%d-%d] ", p[i].pid, start, time);
    }
    printf("|\n");

    printf("\nPID\tAT\tBT\tWT\tTAT\tRT\n");

    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid,
               p[i].arrival_time,
               p[i].burst_time,
               p[i].waiting_time,
               p[i].turnaround_time,
               p[i].response_time);

        total_waiting += p[i].waiting_time;
        total_turnaround += p[i].turnaround_time;
        total_response += p[i].response_time;
    }

    printf("\nAverage Waiting Time: %.2f\n", total_waiting / n);
    printf("Average Turnaround Time: %.2f\n", total_turnaround / n);
    printf("Average Response Time: %.2f\n", total_response / n);
}

void sjf(struct Process input[], int n) {
    struct Process p[100];
    int finished[100] = {0};

    /* Copy the original processes. */
    for (int i = 0; i < n; i++) {
        p[i] = input[i];
    }

    int time = 0;
    int completed = 0;
    double total_waiting = 0;
    double total_turnaround = 0;
    double total_response = 0;

    printf("\n=== Shortest Job First (SJF) ===\n");
    printf("Gantt chart:\n");

    while (completed < n) {
        int chosen = -1;

        /* Choose the shortest unfinished process that has arrived. */
        for (int i = 0; i < n; i++) {
            if (!finished[i] && p[i].arrival_time <= time) {
                if (chosen == -1 ||
                    p[i].burst_time < p[chosen].burst_time ||
                    (p[i].burst_time == p[chosen].burst_time &&
                     p[i].arrival_time < p[chosen].arrival_time)) {
                    chosen = i;
                }
            }
        }

        /* No process is ready: advance to the next arrival. */
        if (chosen == -1) {
            int next_arrival = -1;

            for (int i = 0; i < n; i++) {
                if (!finished[i] &&
                    (next_arrival == -1 ||
                     p[i].arrival_time < next_arrival)) {
                    next_arrival = p[i].arrival_time;
                }
            }

            printf("| IDLE [%d-%d] ", time, next_arrival);
            time = next_arrival;
            continue;
        }

        /* Run the chosen process until completion. */
        int start = time;
        time += p[chosen].burst_time;

        p[chosen].waiting_time = start - p[chosen].arrival_time;
        p[chosen].turnaround_time = time - p[chosen].arrival_time;
        p[chosen].response_time = p[chosen].waiting_time;

        printf("| P%d [%d-%d] ", p[chosen].pid, start, time);

        finished[chosen] = 1;
        completed++;
    }
    printf("|\n");

    /* Display results in the original PID order. */
    printf("\nPID\tAT\tBT\tWT\tTAT\tRT\n");

    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid,
               p[i].arrival_time,
               p[i].burst_time,
               p[i].waiting_time,
               p[i].turnaround_time,
               p[i].response_time);

        total_waiting += p[i].waiting_time;
        total_turnaround += p[i].turnaround_time;
        total_response += p[i].response_time;
    }

    printf("\nAverage Waiting Time: %.2f\n", total_waiting / n);
    printf("Average Turnaround Time: %.2f\n", total_turnaround / n);
    printf("Average Response Time: %.2f\n", total_response / n);
}

int main(void) {
    struct Process processes[100] = {0};
    int n;

    printf("Enter the number of processes: ");

    if (scanf("%d", &n) != 1 || n < 1 || n > 100) {
        printf("Enter a number between 1 and 100.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        processes[i].pid = i + 1;

        printf("Enter arrival time and burst time for process %d: ",
               processes[i].pid);

        if (scanf("%d %d",
                  &processes[i].arrival_time,
                  &processes[i].burst_time) != 2) {
            printf("Invalid input. Enter two integers.\n");
            return 1;
        }

        if (processes[i].arrival_time < 0 ||
            processes[i].burst_time <= 0) {
            printf("Arrival must be >= 0 and burst must be > 0.\n");
            return 1;
        }
    }

    fcfs(processes, n);
    sjf(processes, n);

    return 0;
}
