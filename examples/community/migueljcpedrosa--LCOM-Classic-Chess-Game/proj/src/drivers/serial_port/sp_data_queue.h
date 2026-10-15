#ifndef __SP_DATA_QUEUE_H_
#define __SP_DATA_QUEUE_H_

#include <lcom/lcf.h>

typedef struct {
    int head, tail, queue_size;
    unsigned max_capacity;
    int* elements;
} DataQueue;

DataQueue* data_queue_init();
void data_queue_destroy_queue(DataQueue* dq);
bool data_queue_push(DataQueue* dq, uint8_t data);
uint8_t data_queue_pop(DataQueue* dq);
int data_queue_current_size(DataQueue *dq);
uint8_t data_queue_peek(DataQueue* dq);
bool data_queue_contains(DataQueue* dq, uint8_t element);
uint8_t data_queue_front(DataQueue* dq);
uint8_t data_queue_back(DataQueue* dq);
bool data_queue_is_full(DataQueue* dq);
bool data_queue_is_empty(DataQueue* dq);
#endif
