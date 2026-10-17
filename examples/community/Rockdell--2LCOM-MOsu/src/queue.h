//Implementation of queue with a dynamic array working as a circular buffer (source from rosettacode.org)

#include "circle.h"

/** @defgroup queue Queue
 *  @{
 *
 *  Queue related functions
 */

/** Typedef for circles */
typedef Circle* DATA;

/**
 *  A queue is a "data structure" used to save circles.
 *
 *  It allows us to access the circles according to the order of their appearance on the screen, simplifying our job..
 */
typedef struct {
	DATA *buf;				/**< Content of the queue. */
	unsigned int head;		/**< Beginning of the queue. */
	unsigned int tail;		/**< End of the queue. */
	unsigned int alloc;		/**< Beginning, end and number of elements */
} queue_t;

/**
 *  A queue is a "data structure" used to save circles.
 *
 *  It allows us to access the circles according to the order of their appearance on the screen, simplifying our job..
 */
typedef struct {
	DATA *buf;				/**< Content of the queue. */
	unsigned int head;		/**< Beginning of the queue. */
	unsigned int tail;		/**< End of the queue. */
	unsigned int alloc;		/**< Beginning, end and number of elements */
} *queue;

/**
 *  @brief Creates a new queue.
 *  @returns Returns the recently created queue.
 */
queue q_new();

/**
 *  @brief Checks if the queue is empty.
 *  @param q Queue to check.
 *  @returns Returns 1 if true and 0 otherwise.
 */
int empty(queue q);

/**
 *  @brief Adds an "object" to the beginning of a queue.
 *  @param q Queue to add the "obejct".
 *  @param n "Object" to add.
 */
void enqueue(queue q, DATA n);

/**
 *  @brief Removes the "object" at the beginning of a queue.
 *  @param q Queue to remove the "object".
 *  @param n Variable to save the removed "object".
 */
int dequeue(queue q, DATA *n);

/** @} end of queue */
