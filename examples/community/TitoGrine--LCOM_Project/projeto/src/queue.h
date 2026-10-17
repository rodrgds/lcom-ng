#ifndef QUEUE_H
#define QUEUE_H

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Struct used to store the coordinates of an object.
 */
typedef struct {
  uint16_t x; /**< Coordinate on the x axis. */
  uint16_t y; /**< Coordinate on the y axis. */
} pair;


/**
 * @brief Struct used to store the information of a queue.
 */
typedef struct {
  pair *array;    /**< Elements of the queue */
  int in;         /**< In index for the array */
  int out;        /**< Out index for the array */
  int size;       /**< Maximum size of the queue */
  int counter;    /**< Number of elements of the queue */
} queue;


/* =============== */
/*   CONSTRUCTOR   */
/* =============== */
/**
 * @brief Constructor for the queue, allocates memory for a queue of initial_size and initializes
 *        the queue elemets
 * 
 * @param initial_size Size desired for the queue
 * 
 * @return queue*   Returns a queue* if it was possible to allocate enough memory, 
 *                  returns NULL if a queue of the specifiec size cannot be allocated
 */
queue *new_queue(unsigned int initial_size);


/* ============== */
/*   DESTRUCTOR   */
/* ============== */
/**
 * @brief Destructor for the queue, frees the memory that was once allocated for a queue
 * 
 * @param q     Queue pointer that points to the queue that will be deleted
 */
void delete_queue(queue *q);

/* ====================== */
/* Public Queue Functions */
/* ====================== */

/**
 * @brief Pushes a pair to the back of the queue
 * 
 * @param q       Queue pointer that points to the queue to which the pair will be puhed
 * @param n       Pair that will be pushed to the queue
 * 
 * @return int    Returns 0 if the pair was successfully pushed to the queue,
 *                returns -1 if the pair was not added to the queue
 */
int push(queue *q, pair n);

/**
 * @brief Pops the pair that is on the top of the queue
 * 
 * @param q       Queue pointer that points to the queue from which the pair will be poped
 * @param n       Pair pointer that will receive the value of the poped pair
 * 
 * @return int    Returns 0 if  the pair was successfully poped from the queue,
 *                returns -1 if the pair was not poped from the queue
 */
int pop(queue *q, pair *n);

/**
 * @brief Checks if the queue is empty or not
 * 
 * @param q       Queue pointer that points to the queue which will be checked
 * 
 * @return true   The queue is empty
 * @return false  The queue is not empty
 */
bool is_empty(queue *q);

/**
 * @brief Returns the number of elements of a given queue
 * 
 * @param q       Queue pointer that points to the queue which will be checked
 * @return int    Number of elements of the queue
 */
int num_elements(queue *q);

#endif
