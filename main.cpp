#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#define NUM_PROCESSES 61

using namespace std;

struct Process {
    int pid;                // Process ID
    int burst_time;         // Burst time
    int waiting_time;       // Waiting time
    int turnaround_time;    // Turnaround time
    int priority;           // Priority
    int remaining_time;     // Remaining time
};

// Function to sort processes by burst time in descending order (LJF)
void sortProcessesByBurstTimeDescending(struct Process proc[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (proc[j].burst_time < proc[j + 1].burst_time) {
                struct Process temp = proc[j];
                proc[j] = proc[j + 1];
                proc[j + 1] = temp;
            }
        }
    }
}

// Function to sort processes by priority (Priority Scheduling)
void sortProcessesByPriority(struct Process proc[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (proc[j].priority > proc[j + 1].priority) {
                struct Process temp = proc[j];
                proc[j] = proc[j + 1];
                proc[j + 1] = temp;
            }
        }
    }
}

// Function to sort processes by burst time (SJF)
void sortProcessesByBurstTime(struct Process proc[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (proc[j].burst_time > proc[j + 1].burst_time) {
                struct Process temp = proc[j];
                proc[j] = proc[j + 1];
                proc[j + 1] = temp;
            }
        }
    }
}

// Function to find waiting time for all processes (FCFS, SJF, LJF, Priority)
void findWaitingTime(struct Process proc[], int n) {
    proc[0].waiting_time = 0; // Waiting time for the first process is 0

    // Calculating waiting time
    for (int i = 1; i < n; i++) {
        proc[i].waiting_time = proc[i - 1].waiting_time + proc[i - 1].burst_time;
    }
}

// Function to find turnaround time for all processes
void findTurnaroundTime(struct Process proc[], int n) {
    // Calculating turnaround time by adding burst_time and waiting_time
    for (int i = 0; i < n; i++) {
        proc[i].turnaround_time = proc[i].burst_time + proc[i].waiting_time;
    }
}

// Function to calculate average time for FCFS, SJF, LJF, and Priority
void findAvgTime(struct Process proc[], int n) {
    int total_wt = 0, total_tat = 0;

    findWaitingTime(proc, n);
    findTurnaroundTime(proc, n);

    printf("Processes  Burst time  Waiting time  Turnaround time\n");

    // Calculating total waiting time and total turnaround time
    for (int i = 0; i < n; i++) {
        total_wt += proc[i].waiting_time;
        total_tat += proc[i].turnaround_time;
        printf("   %d \t\t %d \t\t %d \t\t %d\n", proc[i].pid, proc[i].burst_time, proc[i].waiting_time, proc[i].turnaround_time);
    }

    printf("Average waiting time = %.2f\n", (float)total_wt / (float)n);
    printf("Average turnaround time = %.2f\n", (float)total_tat / (float)n);
}

// Function to find waiting time and turnaround time for Round Robin
void findWaitingAndTurnaroundTimes(struct Process proc[], int n, int time_quantum) {
    int t = 0; // Current time
    int processes_remaining = n; // Number of processes remaining

    while (processes_remaining > 0) {
        for (int i = 0; i < n; i++) {
            if (proc[i].remaining_time > 0) {
                if (proc[i].remaining_time > time_quantum) {
                    t += time_quantum;
                    proc[i].remaining_time -= time_quantum;
                } else {
                    t += proc[i].remaining_time;
                    proc[i].waiting_time = t - proc[i].burst_time;
                    proc[i].turnaround_time = t;
                    proc[i].remaining_time = 0;
                    processes_remaining--;
                }
            }
        }
    }
}

// Function to calculate average time for Round Robin
void findAvgTimeRoundRobin(struct Process proc[], int n, int time_quantum) {
    int total_wt = 0, total_tat = 0;

    findWaitingAndTurnaroundTimes(proc, n, time_quantum);

    printf("Processes  Burst time  Waiting time  Turnaround time\n");

    // Calculating total waiting time and total turnaround time
    for (int i = 0; i < n; i++) {
        total_wt += proc[i].waiting_time;
        total_tat += proc[i].turnaround_time;
        printf("   %d \t\t %d \t\t %d \t\t %d\n", proc[i].pid, proc[i].burst_time, proc[i].waiting_time, proc[i].turnaround_time);
    }

    printf("Average waiting time = %.2f\n", (float)total_wt / (float)n);
    printf("Average turnaround time = %.2f\n", (float)total_tat / (float)n);
}

int main() {
    struct Process proc[NUM_PROCESSES + 1];
    srand(time(0));

    // Generate 61 processes with random burst times and priorities
    for (int i = 0; i < NUM_PROCESSES; i++) {
        proc[i].pid = i + 1;
        proc[i].burst_time = rand() % 100 + 1;
        proc[i].priority = rand() % 10 + 1;
        proc[i].remaining_time = proc[i].burst_time;
    }

    // Add high-priority process at time t = 10
    proc[NUM_PROCESSES].pid = NUM_PROCESSES + 1;
    proc[NUM_PROCESSES].burst_time = 10;
    proc[NUM_PROCESSES].priority = 1;           // Highest priority
    proc[NUM_PROCESSES].remaining_time = 10;    // arbitrary value

    // FCFS, no sorting needed
    cout << "FCFS:" << endl;
    findAvgTime(proc, NUM_PROCESSES + 1);

    // Sort processes by priority for Priority Scheduling
    cout << "Priority Scheduling:" << endl;
    sortProcessesByPriority(proc, NUM_PROCESSES + 1);
    findAvgTime(proc, NUM_PROCESSES + 1);

    // Round Robin, no sorting needed
    cout << "Round Robin:" << endl;
    findAvgTimeRoundRobin(proc, NUM_PROCESSES + 1, 10);

    // Sort processes by burst time in descending order for LJF
    cout << "LJF:" << endl;
    sortProcessesByBurstTimeDescending(proc, NUM_PROCESSES + 1);
    findAvgTime(proc, NUM_PROCESSES + 1);

    // Sort processes by burst time for SJF
    cout << "SJF:" << endl;
    sortProcessesByBurstTime(proc, NUM_PROCESSES + 1);
    findAvgTime(proc, NUM_PROCESSES + 1);

    return 0;
}