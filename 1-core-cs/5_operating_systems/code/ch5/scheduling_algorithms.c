/*

- First-come, first-served (FCFS), which schedules tasks in the order in which
they request the CPU.

- Shortest-job-first (SJF), which schedules tasks in order of the length of the
tasks’ next CPU burst.

- Priority scheduling, which schedules tasks based on priority.

- Round-robin (RR) scheduling, where each task is run for a time quantum
(or for the remainder of its CPU burst).

- Priority with round-robin, which schedules tasks in order of priority and
uses round-robin scheduling for tasks with equal priority.

Priorities range from 1 to 10, where a higher numeric value indicates a higher
relative priority. For round-robin scheduling, the length of a time quantum is
10 milliseconds.
*/

/*
T1, 4, 20
T2, 2, 25
T3, 3, 25
T4, 3, 15
T5, 10, 10
*/

/*
Strategy for organizing a list of tasks
    Have the tasks unordered
    Have them ordered by some criteria
    Have different queues based on priority
*/

struct task {
    char *name;
    int tid;
    int priority;
    int burst;
};


