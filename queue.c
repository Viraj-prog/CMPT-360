// CMPT 360 - ASO1 | X02L
// -----------------------------------------------
// Name: Viraj Randeria
// ID: 3143233
// Date: 21/02/2026
// -----------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "sched.h"

/*
Purpose: initializes and instance of queue
params: q -> the struct of the queue_t
        capacity -> the capacity of the queue
return: none
*/
void queue_init(queue_t *q, int capacity){
    q->capacity = capacity;
    q ->data = malloc(q->capacity *sizeof(int));
    q->head = 0;
    q->size=0;
}

/*
Purpose: checks if the queue is empty
params: q -> the instance of the queue_t
        
return: none 
*/
bool queue_is_empty(const queue_t *q){
    return q->size==0;
}

/*
Purpose: frees the dynamically allocated memory of data ones the queue is empty.
params: q -> the instance of the queue_t
return: none 
*/
void queue_free(queue_t *q){
    free(q->data);
    q->data = NULL;
    q->capacity = q->size = q->head = 0;
}

/*
Purpose: pushes an element to the end of the queue
params: q -> the instance of the queue_t
        value-> the pid of the process
return: none 
*/
void enqueue(queue_t *q, int value){
    if (q->size == q->capacity){
        queue_resize(q);
    }

    int tail = (q->head + q->size) % q->capacity; // this is to add an element at the end of the queue
    q->data[tail] = value;
    q->size++;
}

/*
Purpose: removes an element from the start of the queue. (The head element)
params: q -> the instance of the queue_t
return: none 
*/
int queue_pop(queue_t *q){
    if (queue_is_empty(q)){
        return -1;
    }
    int v = q->data[q->head];
    q->head = (q->head +1) % q->capacity;
    q->size--;
    return v;
}

/*
Purpose: if the data array is too short, it increases the capacity,
        creates a new malloc and copies the data of the current data array to the 
        new data array. Then frees the previous array.
        (realloc did not work for some reason)
params: q -> the instance of the queue_t
return: none 
*/
void queue_resize(queue_t *q){
    int newcap = q->capacity *2;
    int *new_data = malloc(newcap * sizeof(int));

    for (int i=0; i< q->size; i++){
        new_data[i] = q-> data[(q->head +i) % q->capacity];
    }
    free(q->data);
    q->data = new_data;
    q->capacity = newcap;
    q->head = 0;
}