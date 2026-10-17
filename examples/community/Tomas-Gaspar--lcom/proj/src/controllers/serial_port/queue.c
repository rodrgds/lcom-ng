#include <lcom/lcf.h>

#include <stdint.h>
#include <stdbool.h>

#include "queue.h"

static uint8_t receive[QUEUE_SIZE];
static uint8_t transmit[QUEUE_SIZE];

static uint16_t front_receive = 0, front_transmit = 0;
static uint16_t back_receive = 0, back_transmit = 0;

void queue_push(queue_t q, uint8_t data) {
    uint16_t* front = q == QUEUE_RECEIVE ? &front_receive : &front_transmit;
    uint16_t* back = q == QUEUE_RECEIVE ? &back_receive : &back_transmit;
    uint8_t* queue = q == QUEUE_RECEIVE ? receive : transmit;

    if ((*back + 1) % QUEUE_SIZE == *front) {
        *front = (*front + 1) % QUEUE_SIZE;
    }

    queue[*back] = data;
    *back = (*back + 1) % QUEUE_SIZE;
}

int queue_pop(queue_t q, uint8_t* data) {
    uint16_t* front = q == QUEUE_RECEIVE ? &front_receive : &front_transmit;
    uint8_t* queue = q == QUEUE_RECEIVE ? receive : transmit;
    
    if (queue_is_empty(q)) return 1;

    *data = queue[*front];
    *front = (*front + 1) % QUEUE_SIZE;
    return 0;
}

bool queue_is_empty(queue_t q){
    uint16_t* front = q == QUEUE_RECEIVE ? &front_receive : &front_transmit;
    uint16_t* back = q == QUEUE_RECEIVE ? &back_receive : &back_transmit;

    return (*front == *back);
}
