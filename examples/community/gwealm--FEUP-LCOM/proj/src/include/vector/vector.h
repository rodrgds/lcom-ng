#pragma once

#include <lcom/lcf.h>
#include "errors.h"

#define VEC_DEFAULT_CAPACITY (16)
#define VEC_DEFAULT_INCREMENT (16)

typedef struct {

  void *values;

  size_t len;
  size_t capacity;

  size_t base_size;

} vector;

vector *(vec_create)(size_t base_size);
void (vec_destroy)(vector *vec);

/**
 * @brief Insert a copy of element at the given index.
 * If the provided index is greater than or equal to the vector's current length, the element will be inserted at the end of the vector.
 * 
 * @param vec 
 * @param element 
 * @param index 
 * @return int 
 */
int (vec_insert)(vector *vec, void *element, size_t index);

/**
 * @brief Erases values from vector
 * 
 * @param vec Vector to erase the value from
 * @param index Index of the value to be erased
 */
int (vec_erase)(vector *vec, size_t index);

/**
 * @brief Resizes vector
 * 
 * @param vec Vector to be resized
 * @param new_capacity The new vector capacity
 */
int (vec_resize)(vector *vec, size_t new_capacity);

/**
 * @brief Clears the vector
 * @param vec The vector to clear
 */
void (vec_clear)(vector *vec);

/**
 * @brief Fill entire vector with copies of element.
 * The vector's capacity remains unchanged and its length will be set to its capacity.
 * 
 * @param vec The vector to fill
 * @param element The element to fill the vector with, or NULL
 * @return 0, if operation is sucessful, or 1, if an error occured
 */
int (vec_fill)(vector *vec, void* element);

/**
 * @brief Pushes value to vector
 * 
 * @param vec Vector to push the value to
 * @param element Element to be pushed to the vector
 */
int (vec_push)(vector *vec, void *element);

/**
 * @brief Get the address of an element in a vector
 * @param vec The vector to search
 * @param index The index of the element
 */
void *(vec_at)(vector *vec, size_t index);
