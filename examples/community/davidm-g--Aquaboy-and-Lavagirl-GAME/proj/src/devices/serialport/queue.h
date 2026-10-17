/**
 * @file queue
 * @brief This file contains the functions for the implements a queue system.
 *
 * @author www.geeksforgeeks.org
 *
 * @date 25/05/2022
 */

#ifndef __QUEUE_H_
#define __QUEUE_H_

#define QUEUESIZE 100

#include <lcom/lcf.h>

typedef struct {
  int front, back, size;
  unsigned capacity;
  uint8_t *values;
} Queue;

/**
 * @brief Creates a new empty queue.
 * @return A pointer to the newly created queue.
 */
Queue *createQueue();

/**
 * @brief Gets the current size of the queue.
 * @param queue A pointer to the queue.
 * @return The size of the queue.
 */
int queueSize(Queue *queue);

/**
 * @brief Pushes an element to the back of the queue.
 * @param queue A pointer to the queue.
 * @param element The element to be pushed.
 * @return True if the element was successfully pushed, false otherwise.
 */
bool push(Queue *queue, uint8_t element);

/**
 * @brief Pops and returns the front element of the queue.
 * @param queue A pointer to the queue.
 * @return The front element of the queue.
 */
uint8_t pop(Queue *queue);

/**
 * @brief Returns the front element of the queue without removing it.
 * @param queue A pointer to the queue.
 * @return The front element of the queue.
 */
uint8_t front(Queue *queue);

/**
 * @brief Returns the back element of the queue without removing it.
 * @param queue A pointer to the queue.
 * @return The back element of the queue.
 */
uint8_t back(Queue *queue);

/**
 * @brief Checks if the queue is empty.
 * @param queue A pointer to the queue.
 * @return True if the queue is empty, false otherwise.
 */
bool isEmpty(Queue *queue);

/**
 * @brief Checks if the queue is full.
 * @param queue A pointer to the queue.
 * @return True if the queue is full, false otherwise.
 */
bool isFull(Queue *queue);

/**
 * @brief Clears the queue, removing all elements.
 * @param queue A pointer to the queue.
 */
void clear(Queue *queue);

#endif
