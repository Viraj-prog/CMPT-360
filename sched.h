// CMPT 360 - ASO1 | X02L
// -----------------------------------------------
// Name: Viraj Randeria
// ID: 3143233
// Date: 21/02/2026
// -----------------------------------------------

#ifndef SCHED_H
#define SCHED_H
#include <stdbool.h>

typedef struct process{
    int pid; // the process ID 
    int arrival; // the arrival time 
    int cpu_time; // the cpu_time requered 
    int remaining; // remaining cpu_time
    int first_run; // time when it is run first, remains -1 until first run
    int completion; // time of completion
} process_t;

typedef struct queue{
    int *data; // A pointer to a dynamically allocated array that stores the process id in the Ready queue.
    int capacity; // the capacity of the data array.(changes if the size increases)
    int head; // the location of the first element of the queue
    int size; // the current number of processes in the queue
} queue_t;

typedef struct timeline {
    int *run; // a pointer to the array which stores the process
    int cap; // capacity of the run array
    int len; // counter for the time, increments after each time slice is run
} timeline_t;


void queue_init(queue_t *q, int capacity);
bool queue_is_empty(const queue_t *q);
void queue_free(queue_t *q);
void enqueue(queue_t *q, int value);
int queue_pop(queue_t *q);
void queue_resize(queue_t *q);

#endif