#ifndef _LCOM_QUEUE_H_
#define _LCOM_QUEUE_H_

#include <lcom/lcf.h>

/** @defgroup data_structures Data Structures */

/**
 * @defgroup queue_t Queue 
 * @ingroup data_structures
 * @{
 *
 * @brief Container which follows the FIFO (First in, First out) principle.
 * 
 * @details This implementation uses a memory-allocated vector with a static size, 
 * which will be the queue's maximum capacity. The actual current queue size is saved
 * to the curr_size variable. To avoid moving every element in the container when pushing or 
 * removing elements, this implementation uses two variables (front_pos and end_pos) to determine
 * what section of the vector is valid. 
*/

/**
 * @struct queue_t
 * @brief Structure to represent a FIFO fixed-size container.
*/
typedef struct {
	void 	**content; /**< @brief Queue content. */
	size_t	base_size; /**< @brief Max queue size. */
	size_t	curr_size; /**< @brief Current queue size. */
	size_t	front_pos; /**< @brief Position of the first valid element inside the queue. */
	size_t	end_pos; /**< @brief Position of the last valid element inside the queue. */
} queue_t;

/**
 * @brief Creates an empty queue.
 * 
 * @param size Maximum capacity
 * @return Pointer to the queue if successful, NULL otherwise
*/
queue_t	*queue_create(size_t size);

/**
 * @brief Pushes an element to the end of a queue.
 * 
 * @note If the queue is full (at max capacity), nothing will be added.
 * 
 * @param queue Queue
 * @param content Pointer to the added content
 * @return 0 if the addition is successful, 1 otherwise.
*/
int			queue_push(queue_t *queue, void *content);

/**
 * @brief Removes an element from the queue and returns it.
 * 
 * @note If the queue if empty, NULL is returned.
 * 
 * @param queue Queue
 * @return Pointer to content if the removal is sucessful, NULL otherwise
*/
void		*queue_pop(queue_t *queue);

/**
 * @brief Returns the element at a specific position inside the queue.
 * 
 * @param queue Queue
 * @param pos Position
 * @return Pointer to content if the position exists, NULL otherwise
*/
void		*queue_at(queue_t *queue, size_t pos);

/**
 * @brief Returns a pointer to the place where the element at a specific position is 
 * stored inside the queue. Allows changing specific queue elements.
 * 
 * @param queue Queue
 * @param pos Position
 * @return Pointer to content if the position exists, NULL otherwise
*/
void 		**queue_at_ref(queue_t *queue, size_t pos);

/**
 * @brief Returns the size of a queue.
 * 
 * @param queue Queue
 * @return Queue size or 0 if queue is NULL
 */
size_t	queue_size(queue_t *queue);

/**
 * @brief Checks if the queue is full.
 * 
 * @param queue Queue
 * @return 1 if full, 0 otherwise or if queue is NULL
*/
int			queue_full(queue_t *queue);

/**
 * @brief Checks if a queue is empty.
 * 
 * @param queue Queue
 * @return 1 if empty, 0 otherwise or if queue is NULL
*/
int			queue_empty(queue_t *queue);

/**
 * @brief Queue destroyer. Frees all allocated memory inside the queue, 
 * including the content still stored inside it using the function passed by f.
 * 
 * @note queue_destroy_nothing() can be passed in the f paramenter and elements inside 
 * the queue won't be destroyed.
 * 
 * @param queue Pointer to queue struct pointer
 * @param f Function to free content memory
*/
void		queue_destroy(queue_t **queue, void (*f)(void *));

/**
 * @brief Can be used in the queue_destroy() function to prevent it 
 * from deleting the content still inside the queue.
*/
void		queue_destroy_nothing(void *ptr);

/**
 * @brief Shuffles the content inside a queue.
 * 
 * @param queue Queue
*/
void		queue_shuffle(queue_t *queue);

/** @} */

#endif /* _LCOM_QUEUE_H_ */
