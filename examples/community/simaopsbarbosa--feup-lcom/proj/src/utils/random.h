/**
 * @file random.h
 * @brief Utility functions for random number generation and related helpers.
 */

 #ifndef _LCOM_RANDOM_H_
#define _LCOM_RANDOM_H_
#include <lcom/lcf.h>

/**
 * @brief Generates a random integer between min and max .
 * 
 * This function returns a random number within the specified range.
 * 
 * @param min The minimum value in the range.
 * @param max The maximum value in the range.
 * @return int A random integer such that min <= value <= max.
 */
int random_number(int min, int max);

#endif /* _LCOM_RANDOM_H_ */
