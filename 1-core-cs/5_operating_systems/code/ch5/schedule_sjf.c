/*
- Shortest-job-first (SJF), which schedules tasks in order of the length of the
tasks’ next CPU burst.
*/

#include "cpu.h"
#include "list.h"
#include "schedulers.h"
#include "task.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int min(int a, int b);

struct node *taskList = NULL;

// add a task to the list
void add(char *name, int priority, int burst) {
    Task *task = malloc(sizeof(Task));

    task->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(task->name, name);

    task->priority = priority;
    task->burst = burst;

    // Insert the task
    insert(&taskList, task);
}

// find the next task
Task *pickNext() {
    Task *shortest_job = taskList->task;
    struct node *n = taskList;
    while (n) {
        if (n->task->burst <= shortest_job->burst) {
            shortest_job = n->task;
        }
        n = n->next;
    }
    return shortest_job;
}

// invoke the scheduler
void schedule() {
    while (taskList) {
        Task *task = pickNext();
        run(task, task->burst);
        delete (&taskList, task);
    }
}

int min(int a, int b) { return (a > b) ? b : a; }