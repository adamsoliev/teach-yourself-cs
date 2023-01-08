/**
 * "Virtual" CPU that also maintains track of system time.
 */

#include "task.h"
#include <stdio.h>

// run this task for the specified time slice
void run(Task *task, int slice) {
    printf("Running task = [%s] [%d] [%d] for %d units.\n", task->name,
           task->priority, task->burst, slice);
}