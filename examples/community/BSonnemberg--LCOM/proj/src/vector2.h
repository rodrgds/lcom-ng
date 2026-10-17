#ifndef VECTOR2_H
#define VECTOR2_H

#include <stdbool.h>

#define NULL_VEC (vec_2d){0.0, 0.0}

// A vector in 2d space
typedef struct vec {
  float x;
  float y;
} vec_2d;

/**
 * @brief Negate a 2d vector
 * @param vector to negate
 * @return negated vector
*/
vec_2d vec_neg(vec_2d v);

/**
 * @brief Add two vectors
 * @return resulting vector after addition
*/
vec_2d vec_add(vec_2d v1, vec_2d v2);

/**
 * @brief Add a vector to a scalar
 * @return resulting vector after addition
*/
vec_2d vec_add_scalar(vec_2d v1, float scalar);

/**
 * @brief Multiply two vectors
 * @return resulting vector after multiplication
*/
vec_2d vec_mult(vec_2d v1, vec_2d v2);

/**
 * @brief Multiply a vector by a scalar
 * @return resulting vector after multiplication
*/
vec_2d vec_mult_scalar(vec_2d v1, float scalar);

/**
 * @brief Get sign vector of a vector
 * @return resulting vector after operation
*/
vec_2d vec_sign(vec_2d v);

/**
 * Get a vector normal to another vector
 * @return resulting vector after X-axis flip
*/
vec_2d vec_normal(vec_2d v);

/**
 * @brief Get unit vector of a vector
 * @return resulting unit vector
*/
vec_2d vec_unit(vec_2d v);

/**
 * @brief Check if two vector are equal
 * @return whether vector coordinates match
*/
bool vec_equals(vec_2d v1, vec_2d v2);

/**
 * @brief Get dot product of two vectors
 * @return result of dot product
*/
float vec_dot(vec_2d v1, vec_2d v2);

/**
 * @brief Get length of a vector
 * @return length of the vector
*/
float vec_len(vec_2d v);

#endif
