/*
    - Round-robin (RR) scheduling, where each task is run for a time quantum
    (or for the remainder of its CPU burst).
    - The length of a time quantum is 10 milliseconds.
*/

#include "cpu.h"
#include "list.h"
#include "schedulers.h"
#include "task.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node *taskList = NULL;
struct node *next_node;

void add(char *name, int priority, int burst) {
    Task *task = malloc(sizeof(Task));
    task->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(task->name, name);
    task->priority = priority;
    task->burst = burst;

    insert(&taskList, task);
}

Task *pickNext() {
    Task *ret = next_node->task;
    next_node = (next_node->next) ? next_node->next : taskList; // circular
    return ret;
}

void schedule() {
    next_node = taskList;
    while (taskList) {
        Task *task = pickNext();
        int slice = task->burst > QUANTUM ? QUANTUM : task->burst;
        run(task, slice);
        task->burst -= slice;
        if (!task->burst) {
            delete (&taskList, task);
        }
    }
}