#ifndef __QUEUE_H_
#define __QUEUE_H_

#define QUEUESIZE 128

#include <lcom/lcf.h>


typedef struct {
    int front, back, size;
    unsigned capacity;
    uint8_t* values;
}Queue;

/**
 * @brief Creates a new queue
 * 
 * @return Queue* 
*/
Queue* new_queue();

/**
 * @brief push a value to the queue
 * 
 * @param queue
 * @param value
 * @return true if the value was pushed successfully
*/
bool queue_push(Queue* queue, uint8_t value);

/**
 * @brief pop a value from the queue
 * 
 * @param queue
 * @return value popped else 0
*/
uint8_t queue_pop(Queue* queue);

/**
 * @brief get the front value of the queue
 * 
 * @param queue
 * @return value at the front else 0
*/
uint8_t queue_front(Queue* queue);

/**
 * @brief get the back value of the queue
 * 
 * @param queue
 * @return value at the back else 0
*/
uint8_t queue_back(Queue* queue);

/**
 * @brief check if the queue is empty
 * 
 * @param queue
 * @return true if the queue is empty
*/

bool queue_isEmpty(Queue* queue);

/**
 * @brief check if the queue is full
 * 
 * @param queue
 * @return true if the queue is full
*/
bool queue_isFull(Queue* queue);

/**
 * @brief clear the queue
 * 
 * @param queue
 * 
*/
void queue_clear(Queue* queue);

#endif
