/**
 * @file utils_drivers.h
 * @brief This file contains utilitary functions 
 * @date 2024-06-02
 * 
 * 
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#include <lcom/lcf.h>

#include <stdint.h>


//Some utility functions of our own

/**
 * @brief Turns a binary coded decimal to binary
 * 
 * @param bcd binary coded decimal to transform
 * @return the binary number
 * 
 */
uint8_t (bcd_to_binary)(uint8_t bcd);
/**
 * @brief Generates a random integer between lower_bound and upper bound
 * 
 * @param lower_bound lower bound
 * @param upper_bound upper bound
 * @return a random integer
 */
int getRandomInt(int lower_bound, int upper_bound);
/**
 * @brief Generates a random float between lower_bound and upper_bound
 * 
 * @param lower_bound lower bound 
 * @param upper_bound upper bound
 * @return a random float 
 */
float getRandomFloat(float lower_bound, float upper_bound);

#endif
