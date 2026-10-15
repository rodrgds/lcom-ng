#include "event_queue.h"
#include <stdlib.h>

struct EventQueue {
    GameEvent *events;
    uint16_t capacity;
    uint16_t head;
    uint16_t tail;
    uint16_t count;
};

EventQueue *event_queue_create(uint16_t capacity) {
    if (capacity == 0) return NULL;

    EventQueue *q = malloc(sizeof(EventQueue));
    if (q == NULL) return NULL;

    q->events = malloc(sizeof(GameEvent) * capacity);
    if (q->events == NULL) {
        free(q);
        return NULL;
    }

    q->capacity = capacity;
    q->head = 0;
    q->tail = 0;
    q->count = 0;
    return q;
}

void event_queue_destroy(EventQueue *q) {
    if (q == NULL) return;
    free(q->events);
    free(q);
}

bool event_queue_empty(const EventQueue *q) {
    if (q == NULL) return true;
    return q->count == 0;
}

bool event_queue_full(const EventQueue *q) {
    if (q == NULL) return true;
    return q->count == q->capacity;
}

int event_queue_push(EventQueue *q, GameEvent *event) {
    if (q == NULL || event == NULL) return 1;
    if (event_queue_full(q)) return 1;
    q->count++;
    q->events[q->tail] = *event;
    q->tail = (q->tail + 1) % q->capacity;
    return 0;
}

int event_queue_pop(EventQueue *q, GameEvent *event) {
    if (q == NULL || event == NULL) return 1;
    if (event_queue_empty(q)) return 1;
    *event = q->events[q->head];
    q->head = (q->head + 1) % q->capacity;
    q->count--;
    return 0;
}
