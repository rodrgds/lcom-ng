#pragma once

#include <lcom/lcf.h>
#include "global_vars.h"

/**
 * @brief Structure representing a position with x and y coordinates.
 */
typedef struct Position {
    uint16_t x; /**< The x-coordinate of the position. */
    uint16_t y; /**< The y-coordinate of the position. */
} Position;

/**
 * @brief Structure representing a node in a queue.
 */
typedef struct Node {
  void *data; /**< Pointer to the data stored in the node. */
  struct Node *prev; /**< Pointer to the previous node in the queue. */
} Node;

/**
 * @brief Structure representing a queue.
 */
typedef struct Queue {
    int size; /**< The size of the queue. */
    struct Node *front; /**< Pointer to the front (head) of the queue. */
    struct Node *back; /**< Pointer to the back (tail) of the queue. */
} Queue;

/**
 * @brief Get the size of the queue.
 *
 * @param queue The pointer to the queue.
 * @return The size of the queue.
 */
int queue_size(Queue **queue);

/**
 * @brief Check if the queue is empty.
 *
 * @param queue The pointer to the queue.
 * @return 1 if the queue is empty, 0 otherwise.
 */
int queue_empty(Queue **queue);

/**
 * @brief Get the data at the front of the queue.
 *
 * @param queue The pointer to the queue.
 * @return A pointer to the data at the front of the queue, or NULL if the queue is empty.
 */
void *queue_front(Queue **queue);

/**
 * @brief Push data into the queue.
 *
 * @param queue The pointer to the queue.
 * @param data The pointer to the data to be pushed into the queue.
 * @param size The size of the data.
 */
void queue_push(Queue **queue, void *data, size_t size);

/**
 * @brief Pop the front element from the queue.
 *
 * @param queue The pointer to the queue.
 */
void queue_pop(Queue **queue);

/**
 * @brief Clear the queue, removing all elements.
 *
 * @param queue The pointer to the queue.
 */
void queue_clear(Queue **queue);



