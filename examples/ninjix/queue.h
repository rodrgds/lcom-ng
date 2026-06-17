/**
 * @file queue.h
 * @brief Generic linked-list queue for arbitrary pointer payloads.
 * @ingroup group_collections
 *
 * The queue is used by the wave controller to schedule spawn entries in
 * FIFO order. It supports init, destroy, enqueue, dequeue, front, empty
 * check, and size queries.
 */

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stddef.h>
#include <stdbool.h>

/**
 * @brief Opaque queue handle.
 */
typedef struct queue queue_t;

/**
 * @brief Allocate and initialise a new queue.
 * @return Pointer to the queue, or NULL on allocation failure.
 */
queue_t *queue_init(void);

/**
 * @brief Destroy the queue and free all remaining nodes.
 *
 * Calls free() on all queued node allocations. Does NOT free the
 * payload pointers -- the caller retains ownership.
 *
 * @param q Queue to destroy (may be NULL, in which case this is a no-op).
 */
void queue_destroy(queue_t *q);

/**
 * @brief Enqueue an element at the rear of the queue.
 * @param q    Queue.
 * @param data Pointer payload (may be NULL).
 * @return 0 on success, 1 on allocation failure (or q is NULL).
 */
int queue_enqueue(queue_t *q, void *data);

/**
 * @brief Dequeue the element at the front of the queue.
 * @param q Queue.
 * @return Payload pointer, or NULL if the queue is empty or q is NULL.
 */
void *queue_dequeue(queue_t *q);

/**
 * @brief Peek at the front element without removing it.
 * @param q Queue.
 * @return Front payload pointer, or NULL if empty.
 */
void *queue_front(queue_t *q);

/**
 * @brief Check whether the queue is empty.
 * @param q Queue.
 * @return true if empty or q is NULL.
 */
bool queue_empty(queue_t *q);

/**
 * @brief Return the number of elements in the queue.
 * @param q Queue.
 * @return Element count (0 if q is NULL).
 */
size_t queue_size(queue_t *q);

#endif
