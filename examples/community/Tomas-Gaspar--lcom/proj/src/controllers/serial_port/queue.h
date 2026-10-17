#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdint.h>

/** @ingroup Serial_Port 
 * 
 * @defgroup Queue Queue
 * 
 * @brief Queue implementation to be used in serial communication
 * 
 * @{
*/

#define QUEUE_SIZE 256  /**< @brief Maximum queue size */

/**
 * @brief Queue type
*/
typedef enum {
    QUEUE_RECEIVE,      /**< @brief Queue used for storing received data */
    QUEUE_TRANSMIT      /**< @brief Queue used for storing data to be transmitted */
} queue_t;


/**
 * @brief Push an item to the back of the queue
 * 
 * @param q Queue which the item should be pushed into
 * @param data The data to be pushed to the queue
*/
void queue_push(queue_t q, uint8_t data);

/**
 * @brief Pop an item from the front of the queue
 * 
 * @param q Queue which the item should be popped from
 * @param data Pointer to variable used to store the value popped
 * 
 * @return 1 if the queue is empty, 0 otherwise
*/
int queue_pop(queue_t q, uint8_t* data);

/**
 * @brief Checks whether or not the queue is empty
 * 
 * @param q Queue to be checked
 * 
 * @return true if empty, false otherwise
*/
bool queue_is_empty(queue_t q);

/**
 * @}
 * @}
*/

#endif
