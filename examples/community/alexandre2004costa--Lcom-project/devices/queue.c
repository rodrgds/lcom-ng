#include "queue.h"

/** @defgroup queue queue
  * @{
 */

/**
 * @brief Creates a new queue.
 * @return A pointer to the newly created Queue.
 */
Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->front = queue->size = 0;
    queue->rear = MAXIM - 1;
    return queue;
}

/**
 * @brief Checks if the queue is full.
 * @param queue A pointer to the Queue to check.
 * @return 1 if the queue is full, 0 otherwise.
 */
int isFull(Queue* queue) {
    return (queue->size == MAXIM);
}

/**
 * @brief Checks if the queue is empty.
 * @param queue A pointer to the Queue to check.
 * @return 1 if the queue is empty, 0 otherwise.
 */
int isEmpty(Queue* queue) {
    return (queue->size == 0);
}

/**
 * @brief Adds an item to the queue.
 * @param queue A pointer to the Queue to add an item to.
 * @param item The item to add to the queue.
 */
void enqueue(Queue* queue, uint8_t item) {
    if (isFull(queue)) {
        return;
    }
    queue->rear = (queue->rear + 1) % MAXIM;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}

/**
 * @brief Removes an item from the queue.
 * @param queue A pointer to the Queue to remove an item from.
 * @return The item removed from the queue, or -1 if the queue is empty.
 */
uint8_t dequeue(Queue* queue) {
    if (isEmpty(queue)) {
        return -1;
    }
    uint8_t item = queue->array[queue->front];
    queue->front = (queue->front + 1) % MAXIM;
    queue->size = queue->size - 1;
    return item;
}

/**@}*/
