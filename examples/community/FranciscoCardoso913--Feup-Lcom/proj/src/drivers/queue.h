#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct Queue_val Queue_val;

struct Queue;

typedef struct Queue Queue;

Queue* create_queue();

void destroy_queue(Queue *q);

void push_queue(Queue *q, uint8_t val);

void pop_queue(Queue *q);

uint8_t front(Queue *q);

bool is_empty(Queue *q);

void clear_queue(Queue *q);

uint8_t get_size(Queue *q);

#endif
