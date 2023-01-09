
/*

- Priority with round-robin, which schedules tasks in order of priority and
uses round-robin scheduling for tasks with equal priority.

*/

#include "cpu.h"
#include "list.h"
#include "schedulers.h"
#include "task.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node *taskList[MAX_PRIORITY + 1];
struct node *next_node;

void add(char *name, int priority, int burst) {
    Task *task = malloc(sizeof(Task));
    task->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(task->name, name);
    task->priority = priority;
    task->burst = burst;

    insert(&taskList[priority], task);
}

Task *pickNext(struct node *n) {
    Task *ret = next_node->task;
    next_node = (next_node->next) ? next_node->next : n; // circular
    return ret;
}

void schedule() {
    for (int p = MAX_PRIORITY; p >= MIN_PRIORITY; --p) {
        next_node = taskList[p];
        while (taskList[p]) {
            Task *task = pickNext(taskList[p]);
            int slice = task->burst > QUANTUM ? QUANTUM : task->burst;
            run(task, slice);
            task->burst -= slice;
            if (!task->burst) {
                delete (&taskList[p], task);
            }
        }
    }
}
