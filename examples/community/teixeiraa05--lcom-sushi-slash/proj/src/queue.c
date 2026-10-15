#include "../include/game/queue.h"

void queue_init(Queue* q) {
    q->front = 0;
    q->back = 0;
    q->size = 0;
}

bool queue_is_empty(const Queue* q) {
    return q->size == 0;
}

bool queue_is_full(const Queue* q) {
    return q->size == MAX_QUEUE_SIZE;
}

bool queue_push(Queue* q, char value) {
    if (queue_is_full(q)) {
        return false;
    }
    q->data[q->back] = value;
    q->back = (q->back + 1) % MAX_QUEUE_SIZE;
    q->size++;
    return true;
}

bool queue_pop(Queue* q, char* value) {
    if (queue_is_empty(q)) {
        return false;
    }
    *value = q->data[q->front];
    q->front = (q->front + 1) % MAX_QUEUE_SIZE;
    q->size--;
    return true;
}

bool queue_top(Queue* q, char* value) {
    if (queue_is_empty(q)) {
        return false;
    }
    *value = q->data[q->front];
    return true;
}

int queue_size(const Queue* q) {
    return q->size;
}
