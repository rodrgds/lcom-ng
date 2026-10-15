#include "queue.h"

struct queue_imp{
    int front, back, size;
    unsigned capacity;
    uint8_t* data;
};

Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (!queue) {
        return NULL;
    }
    queue->capacity = QUEUESIZE;
    queue->front = queue->size = 0;
    queue->back = QUEUESIZE - 1;
    queue->data = (uint8_t*)malloc(queue->capacity * sizeof(uint8_t));
    if (!queue->data) {
        free(queue);
        return NULL;
    }
    return queue;
}

bool isFull(Queue* queue) {
    return (queue->size == (int)queue->capacity);
}

bool isEmpty(Queue* queue) {
    return (queue->size == 0);
}

int queueSize(Queue* queue) {
    return queue->size;
}

bool push(Queue* queue, uint8_t element) {
    if (isFull(queue)){
        printf("Queue is full, cannot push element: %d\n", element);
        return false;
    }
    queue->back = (queue->back + 1) % queue->capacity;
    queue->data[queue->back] = element;
    queue->size = queue->size + 1;
    return true;
}

uint8_t pop(Queue* queue) {
    if (isEmpty(queue)){
        printf("Queue is empty, cannot pop element.\n");
        return 0; 
    }
    uint8_t element = queue->data[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return element;
}

uint8_t front(Queue* queue) {
    if (isEmpty(queue)){
        printf("Queue is empty, front element is undefined.\n");
        return 0;
    }
    return queue->data[queue->front];
}

uint8_t back(Queue* queue) {
    if (isEmpty(queue)){
        printf("Queue is empty, back element is undefined.\n");
        return 0;
    }
    return queue->data[queue->back];
}

void destroyQueue(Queue* queue) {
    if (!queue) {
        printf("Queue is NULL, cannot clear.\n");
        return;
    }
    if (queue->data) {
        free(queue->data);
    }
    free(queue);
}
