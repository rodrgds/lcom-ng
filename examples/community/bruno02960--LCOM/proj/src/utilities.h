/**
 * @file utilities.h
 * @author Bruno Pinto and Vitor Magalhaes
 * @brief Contains utility functions
 */

#ifndef __UTILITIES_H
#define __UTILITIES_H

/** @defgroup utilities utilities
 * @{
 *
 * Utility functions
 */

/**
 * @brief Converts a BCD number to Binary
 *
 * @param number number to be converted
 *
 * @return Returns converted number
 */
unsigned long convertBCDtoBinary(unsigned long number);

/**
 * @brief Converts a number to its twos complement
 *
 * @param number Number to be converted
 *
 * @return Returns converted number
 */
char twos_complement(char number);

/** @} end of utilities */

#endif
