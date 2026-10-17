/**
 * @file queue.h
 * @brief Interface for circular queue management.
 *
 * This header provides the structure and functions for creating, using, and destroying
 * a circular queue, including enqueue, dequeue, and utility operations.
 */
#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/**
 * @brief Structure representing a circular queue.
 */
struct queue;
typedef struct queue queue_t;

/**
 * @brief Creates a new queue.
 * 
 * @param size Initial queue size.
 * @param elem_size Element size.
 * @return Pointer to newly allocated queue, or NULL on failure.
 */
queue_t *new_queue(unsigned int size, int elem_size);

/**
 * @brief Deletes a queue and frees its allocated memory.
 * 
 * @param q Pointer to the queue to delete.
 */
void delete_queue(queue_t *q);

/**
 * @brief Checks if a queue is empty.
 * 
 * @param q Pointer to the queue to delete.
 * @return 1 if queue is empty, 0 if not.
 */
bool is_empty_queue(queue_t *q);

/**
 * @brief Checks if a queue is not empty.
 * 
 * @param q Pointer to the queue to delete.
 * @return 1 if queue is not empty, 0 if it is.
 */
bool is_not_empty_queue(queue_t *q);

/**
 * @brief Enqueues an element into the queue.
 * 
 * @param q Pointer to the queue.
 * @param elem Element to enqueue.
 * @return 0 on success, -1 if queue is full or allocation fails.
 */
int put_queue(queue_t *q, void *elem);

/**
 * @brief Dequeues the first element from the queue.
 * 
 * @param q Pointer to the queue.
 * @param elem Pointer to store the dequeued element.
 * @return 0 on success, -1 if queue is empty.
 */
int get_queue(queue_t *q, void *elem);

#endif // QUEUE_H
