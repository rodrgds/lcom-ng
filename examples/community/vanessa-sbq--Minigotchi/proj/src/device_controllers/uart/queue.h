#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdio.h>
#include <stdlib.h>

// Node structure
typedef struct Node {
    char data;
    struct Node* next;
} Node;

// Queue structure
typedef struct Queue {
    Node* front;
    Node* rear;
    int size;
} Queue;

Node* createNode(char data);
Queue* createQueue();
int isEmpty(Queue* queue);
int getSize(Queue* queue);
void enqueue(Queue* queue, char data);
char dequeue(Queue* queue);
char peek(Queue* queue);

#endif
