#include "sp_data_queue.h"

DataQueue* data_queue_init() {
    DataQueue* dq = (DataQueue*)malloc(sizeof(DataQueue));
    if(dq == NULL) {
        perror("Unable to allocate memory for DataQueue");
        exit(EXIT_FAILURE);
    }
    dq->max_capacity = 100; //max capacity of the queue for now. might change later
    dq->head = dq->queue_size = 0;
    dq->tail = 99; //
    dq->elements = (int*)malloc(dq->max_capacity * sizeof(int));
    return dq;
}

void data_queue_destroy_queue(DataQueue* dq) {
    free(dq->elements);
    free(dq);
}

bool data_queue_push(DataQueue* dq, uint8_t data){
    if (data_queue_is_full(dq)) return false;
    dq->tail = (dq->tail + 1) % dq->max_capacity;
    dq->elements[dq->tail] = data;
    dq->queue_size++;
    return true;
}

uint8_t data_queue_pop(DataQueue* dq){
    if (data_queue_is_empty(dq)) return 0;
    int element = dq->elements[dq->head];
    dq->head = (dq->head + 1) % dq->max_capacity;
    dq->queue_size--;
    return element;
}

int data_queue_current_size(DataQueue * dq){
    return dq->queue_size;
}


uint8_t data_queue_peek(DataQueue* dq) {
    if (data_queue_is_empty(dq))
        return 0;
    return dq->elements[dq->head];
}

bool data_queue_contains(DataQueue* dq, uint8_t element) {
    for (int i = dq->head; i != dq->tail; i = (i + 1) % dq->max_capacity) {
        if (dq->elements[i] == element)
            return true;
    }
    return false;
}

uint8_t data_queue_front(DataQueue* dq){
    if (data_queue_is_empty(dq)) return 0;
    return dq->elements[dq->head];
}

uint8_t data_queue_back(DataQueue* dq){
    if (data_queue_is_empty(dq)) return 0;
    return dq->elements[dq->tail];
}

bool data_queue_is_full(DataQueue* dq){
    return (dq->queue_size == (int) dq->max_capacity);
}

bool data_queue_is_empty(DataQueue* dq){
    return (dq->queue_size == 0);
}
