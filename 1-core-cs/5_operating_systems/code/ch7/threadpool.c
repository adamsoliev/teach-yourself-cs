/**
 * Implementation of thread pool.
 */

#include "threadpool.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define QUEUE_SIZE 10
#define NUMBER_OF_THREADS 3

#define TRUE 1

// this represents work that has to be
// completed by a thread in the pool
typedef struct {
    void (*function)(void *p);
    void *data;
} task;

pthread_mutex_t lock; // for enqueue and dequeue
sem_t taskCnt;

// the work queue
task tasks[QUEUE_SIZE + 1];
size_t queueH = 0, queueT = 0;

// the worker bees
pthread_t bees[NUMBER_OF_THREADS];

// insert a task into the queue
// returns 0 if successful or 1 otherwise,
int enqueue(task t) {
    pthread_mutex_lock(&lock);                       // acquire the lock
    if ((queueT + 1) % (QUEUE_SIZE + 1) == queueH) { // the queue is full
        pthread_mutex_unlock(&lock);
        return 1;
    }
    tasks[queueT] = t;
    queueT = (queueT + 1) % (QUEUE_SIZE + 1);
    pthread_mutex_unlock(&lock);
    return 0;
}

// remove a task from the queue
task dequeue() {
    pthread_mutex_lock(&lock);
    task ret = tasks[queueH];
    queueH = (queueH + 1) % (QUEUE_SIZE + 1);
    pthread_mutex_unlock(&lock);
    return ret;
}

// the worker thread in the thread pool
void *worker(void *param) {
    while (TRUE) {
        sem_wait(&taskCnt); // block until a work becomes available
        task work = dequeue();
        execute(work.function, work.data); // execute the task
    }
    // pthread_exit(0);
}

/**
 * Executes the task provided to the thread pool
 */
void execute(void (*somefunction)(void *p), void *p) { (*somefunction)(p); }

/**
 * Submits work to the pool.
 */
int pool_submit(void (*somefunction)(void *p), void *p) {
    int err = 0;
    task newTask;
    newTask.function = somefunction;
    newTask.data = p;
    err = enqueue(newTask);
    if (!err) {             // sucess
        sem_post(&taskCnt); // signal semaphore
    }
    return err;
}

// initialize the thread pool
void pool_init(void) {
    pthread_mutex_init(&lock, NULL);
    sem_init(&taskCnt, 0, 0);
    for (size_t i = 0; i != NUMBER_OF_THREADS; ++i) {
        pthread_create(&bees[i], NULL, worker, NULL);
    }
}

// shutdown the thread pool
void pool_shutdown(void) {
    for (size_t i = 0; i != NUMBER_OF_THREADS; ++i) {
        pthread_cancel(bees[i]);
        pthread_join(bees[i], NULL);
    }
    sem_destroy(&taskCnt);
    pthread_mutex_destroy(&lock);
}