/*
    - First-come, first-served (FCFS), which schedules tasks in the order in
   which they request the CPU.
*/
#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "list.h"
#include "schedulers.h"
#include "task.h"

struct node *taskList = NULL;

// add a task to the list
void add(char *name, int priority, int burst) {
    Task *task = malloc(sizeof(Task));

    task->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(task->name, name);

    task->priority = priority;
    task->burst = burst;

    // Insert
    insert(&taskList, task);
}

Task *pickNext() {
    struct node *lastNode = taskList;
    while (1) {
        if (!lastNode->next) {
            break;
        }
        lastNode = lastNode->next;
    }
    return lastNode->task;
}

// invoke the scheduler
void schedule() {
    while (taskList) {
        Task *task = pickNext();
        run(task, task->burst);
        delete (&taskList, task);
    }
}
