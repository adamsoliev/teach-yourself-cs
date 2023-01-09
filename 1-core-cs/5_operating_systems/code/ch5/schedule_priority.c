
/*
- Priority scheduling, which schedules tasks based on priority.
*/

#include "cpu.h"
#include "list.h"
#include "schedulers.h"
#include "task.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node *taskList;

void add(char *name, int priority, int burst) {
    Task *task = malloc(sizeof(Task));
    task->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(task->name, name);

    task->priority = priority;
    task->burst = burst;

    insert(&taskList, task);
}

Task *pickNext() {
    Task *highest_priority_job = taskList->task;
    struct node *n = taskList;
    while (n) {
        if (highest_priority_job->priority <= n->task->priority) {
            highest_priority_job = n->task;
        }
        n = n->next;
    }
    return highest_priority_job;
}

void schedule() {
    while (taskList) {
        Task *task = pickNext();
        run(task, task->burst);
        delete (&taskList, task);
    }
}