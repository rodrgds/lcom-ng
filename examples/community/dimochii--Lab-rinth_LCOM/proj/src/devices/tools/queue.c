#include "queue.h"

/** @defgroup queue queue
 * @{
 *
 * Functions that simulate the queue data structure.
 */


/**
 * @brief Creates a new queue with the given capacity.
 * This function initializes a new queue with the given capacity and puts its
 * front, size and last elements.
 * @param capacity The maximum number of elements that the queue can have.
 * @return A pointer to the created queue.
 */
Queue* newQueue(unsigned capacity){
    Queue* queue = (Queue*)malloc( sizeof(Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
 
    queue->last = capacity - 1;
    queue->values = (uint8_t*)malloc( queue->capacity * sizeof(uint8_t));
    return queue;
}

/**
 * @brief Sees if the queue is full.
 * This function sees if the queue has its maximum capacity occupied.
 * @param queue The queue.
 * @return True if the queue is full and false if it isn't.
 */
bool (queueIsFull)(Queue* queue){
    return (queue->size == (int) queue->capacity);
}

/**
 * @brief Sees if the queue is empty.
 * This function sees if the queue doesn't have elements.
 * @param queue The queue.
 * @return True if the queue is empty and false if it isn't.
 */
bool (queueIsEmpty)(Queue* queue){
    return (queue->size == 0);
}

/**
 * @brief Size of the queue.
 * This function returns the number of elements that are in the queue.
 * @param queue The queue.
 * @return The number of elements in the queue.
 */
int (sizeQueue)(Queue * queue){
    return queue->size;
}

/**
 * @brief Adds an element to the queue.
 * This function adds an element to the back of the queue if it is not full.
 * @param queue The queue.
 * @param element The element to add to the queue.
 * @return True if the element was added nad false if it wasn't.
 */
bool (enqueue)(Queue* queue, uint8_t element){
    if (queueIsFull(queue))
        return false;
    queue->last = (queue->last + 1) % queue->capacity;
    queue->values[queue->last] = element;
    queue->size = queue->size + 1;
    return true;
}

/**
 * @brief Removes an element from the queue.
 * This function removes an element from the front of the queue if it is not empty.
 * @param queue The queue.
 * @return The element removed from the front of the queue and 0 if the queue is empty.
 */
uint8_t (dequeue)(Queue* queue){
    if (queueIsEmpty(queue))
        return 0;
    int element = queue->values[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return element;
}

/**
 * @brief Gets the front element of the queue without removing it.
 * This function returns the front element of the queue if it is not empty.
 * @param queue The queue.
 * @return The front element of the queue and 0 if the queue is empty.
 */
uint8_t (frontQueue)(Queue* queue){
    if (queueIsEmpty(queue))
        return 0;
    return queue->values[queue->front];
}

/**
 * @brief Gets the back element of the queue without removing it.
 * This function returns the back element of the queue if it is not empty.
 * @param queue The queue.
 * @return The back element of the queue and 0 if the queue is empty.
 */
uint8_t (backQueue)(Queue* queue){
    if (queueIsEmpty(queue))
        return 0;
    return queue->values[queue->last];
}

/**
 * @brief Clears the queue and frees its memory.
 * This function clears the queue by freeing the memory allocated for its elements
 * and the queue structure.
 * @param queue The queue.
 */
void (clearQueue)(Queue* queue){
    free(queue->values);
    free(queue);
}

