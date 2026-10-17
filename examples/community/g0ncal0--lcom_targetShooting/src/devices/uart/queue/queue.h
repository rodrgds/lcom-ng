#ifndef QUEUE_H
#define QUEUE_H
#include <lcom/lcf.h>
#include <stdint.h>

#define QUEUE_SIZE 1024

/**
 * @brief Queue structure definition.
 */
typedef struct queue {
  void *buf;        /** @brief Pointer to the buffer */
  int in, out;      /** @brief Index of the next element to be inserted and removed */
  int size, count;  /** @brief Size of the buffer and number of elements in the buffer */
  int el_size;      /** @brief Size of each element in the buffer */
} queue_t;


queue_t* create_queue(int size, int el_size);

/**
 * @brief Initializes a queue.
 * @param q Pointer to the queue structure.
 * @param size Size of the buffer (number of elements).
 * @param el_size Size of each element in the buffer.
 * @return 0 on success, non-zero otherwise.
 */
int (queue_init)(queue_t *q, int size, int el_size);

/**
 * @brief Destroys a queue, freeing its buffer.
 * @param q Pointer to the queue structure.
 * @return 0 on success, non-zero otherwise.
 */
int (queue_destroy)(queue_t *q);

/**
 * @brief Checks if the queue is empty.
 * @param q Pointer to the queue structure.
 * @return 1 if the queue is empty, 0 otherwise.
 */
bool (queue_is_empty)(queue_t *q);

/**
 * @brief Checks if the queue is full.
 * @param q Pointer to the queue structure.
 * @return 1 if the queue is full, 0 otherwise.
 */
bool (queue_is_full)(queue_t *q);

/**
 * @brief Adds an element to the queue.
 * @param q Pointer to the queue structure.
 * @param elem Pointer to the element to be added.
 * @return 0 on success, non-zero otherwise.
 */
int (queue_enqueue)(queue_t *q, void *elem);

/**
 * @brief Removes an element from the queue.
 * @param q Pointer to the queue structure.
 * @param elem Pointer to store the removed element.
 * @return 0 on success, non-zero otherwise.
 */
int (queue_dequeue)(queue_t *q, void *elem);

/**
 * @brief Retrieves the front element of the queue without removing it.
 * @param q Pointer to the queue structure.
 * @param elem Pointer to store the front element.
 * @return 0 on success, non-zero otherwise.
 */
int (queue_front)(queue_t *q, void *elem);

#endif /* QUEUE_H */
