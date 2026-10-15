#ifndef _QUEUE_H_
#define _QUEUE_H_

#define QUEUESIZE 128
#include <lcom/lcf.h>

/**
 * @file queue.h
 * @brief Queue implementation for handling serial port data
 * 
 * This module provides a simple queue implementation to manage data received from the serial port.
 * It allows for pushing and popping elements, checking the size, and determining if the queue is empty or full.
 */

struct queue_imp;
typedef struct queue_imp Queue;


/**
 * @brief Creates a new queue
 * 
 * Allocates memory for the queue and initializes its properties.
 * @return Pointer to the newly created queue, or NULL if memory allocation fails.
 */
Queue* createQueue();

/**
 * @brief Returns the size of the queue
 * @param queue Pointer to the queue
 * @return The number of elements in the queue
 */
int queueSize(Queue *queue);

/**
 * @brief Pushes an element onto the queue
 * 
 * Adds an element to the back of the queue.
 * @param queue Pointer to the queue
 * @param element The element to be added
 * @return true if the element was successfully added, false if the queue is full
 */
bool push(Queue* queue, uint8_t element);

/**
 * @brief Pops an element from the queue
 * 
 * Removes and returns the front element of the queue.
 * @param queue Pointer to the queue
 * @return The popped element, or 0 if the queue is empty
 */
uint8_t pop(Queue* queue);

/**
 * @brief Returns the front element of the queue without removing it
 * 
 * @param queue Pointer to the queue
 * @return The front element, or 0 if the queue is empty
 */
uint8_t front(Queue* queue);

/**
 * @brief Returns the back element of the queue without removing it
 * 
 * @param queue Pointer to the queue
 * @return The back element, or 0 if the queue is empty
 */
uint8_t back(Queue* queue);

/**
 * @brief Checks if the queue is empty
 * 
 * @param queue Pointer to the queue
 * @return true if the queue is empty, false otherwise
 */
bool isEmpty(Queue* queue);

/**
 * @brief Checks if the queue is full
 * 
 * @param queue Pointer to the queue
 * @return true if the queue is full, false otherwise
 */
bool isFull(Queue* queue);

/**
 * @brief Destroys the queue and frees its memory
 * 
 * Releases the memory allocated for the queue and its data.
 * @param queue Pointer to the queue to be destroyed
 */
void destroyQueue(Queue* queue);

#endif
