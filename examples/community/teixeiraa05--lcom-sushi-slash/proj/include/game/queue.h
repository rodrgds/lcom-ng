#ifndef __QUEUE_H
#define __QUEUE_H

#include <lcom/lcf.h>

/** @brief Maximum number of bytes stored by a Queue. */
#define MAX_QUEUE_SIZE 32

/**
 * @brief Fixed-size circular queue of byte-sized values.
 */
typedef struct {
    /** @brief Stored queue values. */
    char data[MAX_QUEUE_SIZE];

    /** @brief Index of the next value to pop. */
    int front;

    /** @brief Index where the next pushed value will be written. */
    int back;

    /** @brief Current number of queued values. */
    int size;
} Queue;

/**
 * @brief Initialize an empty queue.
 *
 * @param q Queue to initialize.
 */
void queue_init(Queue* q);

/**
 * @brief Check whether a queue contains no values.
 *
 * @param q Queue to inspect.
 * @return true if the queue is empty, false otherwise.
 */
bool queue_is_empty(const Queue* q);

/**
 * @brief Check whether a queue has reached MAX_QUEUE_SIZE.
 *
 * @param q Queue to inspect.
 * @return true if no more values can be pushed, false otherwise.
 */
bool queue_is_full(const Queue* q);

/**
 * @brief Push a value at the back of the queue.
 *
 * @param q Queue to mutate.
 * @param value Value to enqueue.
 * @return true on success, false if the queue is full or invalid.
 */
bool queue_push(Queue* q, char value);

/**
 * @brief Pop the value at the front of the queue.
 *
 * @param q Queue to mutate.
 * @param value Output storage for the popped value.
 * @return true on success, false if the queue is empty or invalid.
 */
bool queue_pop(Queue* q, char* value);

/**
 * @brief Read the value at the front of the queue without removing it.
 *
 * @param q Queue to inspect.
 * @param value Output storage for the front value.
 * @return true on success, false if the queue is empty or invalid.
 */
bool queue_top(Queue* q, char* value);

/**
 * @brief Get the current number of values in the queue.
 *
 * @param q Queue to inspect.
 * @return Queue size, or 0 for an invalid queue.
 */
int queue_size(const Queue* q);

#endif // __QUEUE_H
