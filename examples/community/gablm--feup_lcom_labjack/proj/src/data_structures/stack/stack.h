#ifndef _LCOM_STACK_H_
#define _LCOM_STACK_H_

#include <lcom/lcf.h>

/** @defgroup data_structures Data Structures */

/** 
 * @defgroup stack_t Stack 
 * @ingroup data_structures
 * @{
 *
 * @brief Container which follows the LIFO (Last in, First out) principle.
*/

/**
 * @struct my_stack_t
 * @brief Structure to represent a LIFO fixed-size container.
*/
typedef struct {
	void 	**content; /**< @brief Stack content. */
	size_t	base_size; /**< @brief Max stack size. */
	size_t	curr_size; /**< @brief Current stack size. */
	size_t	end_pos; /**< @brief Position of the last valid element inside the stack. @details This is the position of the last pushed element, and of the element to be popped. */
} my_stack_t;

/**
 * @brief Creates an empty stack.
 * 
 * @param size Maximum capacity
 * @return Pointer to the stack if successful, NULL otherwise
*/
my_stack_t	*stack_create(size_t size);

/**
 * @brief Pushes an element to the top of a stack.
 * 
 * @note If the stack is full (at max capacity), nothing will be added.
 * 
 * @param stack stack
 * @param content Pointer to the added content
 * @return 0 if the addition is successful, 1 otherwise.
*/
int			stack_push(my_stack_t *stack, void *content);

/**
 * @brief Removes an element from top of the stack and returns it.
 * 
 * @note If the stack if empty, NULL is returned.
 * 
 * @param stack Stack
 * @return Pointer to content if the removal is sucessful, NULL otherwise
*/
void		*stack_pop(my_stack_t *stack);

/**
 * @brief Returns the size of a stack.
 * 
 * @param stack Stack
 * @return Stack size or 0 if stack is NULL
 */
size_t	stack_size(my_stack_t *stack);

/**
 * @brief Checks if the stack is full.
 * 
 * @param stack Stack
 * @return 1 if full, 0 otherwise or if stack is NULL
*/
int			stack_full(my_stack_t *stack);

/**
 * @brief Checks if a stack is empty.
 * 
 * @param stack Stack
 * @return 1 if empty, 0 otherwise or if stack is NULL
*/
int			stack_empty(my_stack_t *stack);

/**
 * @brief Stack destroyer. Frees all allocated memory inside the stack, 
 * including the content still stored inside it using the function passed by f.
 * 
 * @note stack_destroy_nothing() can be passed in the f paramenter and elements inside 
 * the stack won't be destroyed.
 * 
 * @param stack Pointer to stack struct pointer
 * @param f Function to free content memory
*/
void		stack_destroy(my_stack_t **stack, void (*f)(void *));

/**
 * @brief Can be used in the stack_destroy() function to prevent it 
 * from deleting the content still inside the stack.
*/
void		stack_destroy_nothing(void *ptr);

/**
 * @brief Returns the element at a specific position inside the stack.
 * 
 * @note Beware that the last position is the most recent pushed item into the stack. This means the 0th element would be the bottom element of the stack.
 * 
 * @param stack
 * @param pos Position
 * @return Pointer to content if the position exists, NULL otherwise
*/
void 		*stack_at(my_stack_t *stack, size_t pos);

/** @} */

#endif /* _LCOM_STACK_H_ */
