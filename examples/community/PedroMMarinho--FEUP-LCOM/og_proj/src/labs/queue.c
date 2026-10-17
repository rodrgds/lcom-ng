#include "queue.h"

Queue* new_queue(){
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->capacity = QUEUESIZE;
    queue->front = 0;
    queue->size = 0;
    queue->back = QUEUESIZE - 1;
    queue->values = (uint8_t*)malloc(queue->capacity * sizeof(uint8_t));
    
    return queue;
}

bool queue_push(Queue* queue, uint8_t value){
    if (!queue_isFull(queue)){
        queue->size++;
        queue->back = (queue->back + 1) % queue->capacity;
        queue->values[queue->back] = value;
        return true;
    }
    else return false;

}

uint8_t queue_pop(Queue* queue){
    if (!queue_isEmpty(queue)){
        uint8_t value = queue->values[queue->front];
        queue->front = (queue->front + 1) % queue->capacity;
        queue->size--;
        return value;
    }
    else return 0;

}

uint8_t queue_front(Queue* queue){
    if (!queue_isEmpty(queue)){
        return queue->values[queue->front];
    }
    else return 0;
}

uint8_t queue_back(Queue* queue){
    if (!queue_isEmpty(queue)){
        return queue->values[queue->back];
    }
    else return 0;
}


bool queue_isFull(Queue* queue){
    return (queue->size == (int) queue->capacity);
}

bool queue_isEmpty(Queue* queue){
    return (queue->size == 0);
}


void queue_clear(Queue* queue){
    free(queue->values);
    free(queue);
}
