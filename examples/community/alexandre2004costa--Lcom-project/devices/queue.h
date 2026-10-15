#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#define MAXIM 100

typedef struct {
    uint8_t front, rear, size;
    uint8_t array[MAXIM];
} Queue;

Queue* createQueue();

int isFull(Queue* queue);

int isEmpty(Queue* queue);

void enqueue(Queue* queue, uint8_t item);

uint8_t dequeue(Queue* queue);

