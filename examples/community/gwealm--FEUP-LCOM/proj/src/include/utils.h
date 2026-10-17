#pragma once

#include <lcom/lcf.h>

/**
 * @brief Returns the address of an item in an array
 *
 * @param ptr The address of the array
 * @param index The index of the item to get the address of
 * @param base_size The size of the items in the array
 */
void *(util_addr)(void* ptr, size_t index, size_t base_size);

/**
 * @brief Fills an array with a given integer
 * 
 * @param dest The array to fill
 * @param fill The integer to fill the array with
 * @param num The number of elements in the array
 * @param base_size The size of the items in the array
 */
void *(util_memset)(void *dest, int fill, size_t num, size_t base_size);

/**
 * @brief Moves an array in memory
 * 
 * @param dest The destination address of the allocated memory
 * @param src The origin address of the allocated memory
 * @param num The number of elements in the array
 * @param base_size The size of the items in the array
 */
void *(util_memmove)(void *dest, const void *src, size_t num, size_t base_size);

/**
 * @brief Copies an array in memory to another position in memory
 * 
 * @param dest The address to copy the memory to
 * @param src The origin address of the allocated memory
 * @param num The number of elements in the array
 * @param base_size The size of the items in the array 
 */
void *(util_memcpy)(void *dest, const void *src, size_t num, size_t base_size);

/**
 * @brief Copies an element to another position in memory
 *
 * @param dest Will hold the position of the new element
 * @param src The origin address of the allocated memory
 * @param size The size of the element
 */
int (util_copy_elem)(void **dest, void *src, size_t size);

/**
 * @brief Reads information from a port
 * 
 * @param port The port to read from
 * @param dest Will hold the value written after the function call
 * @return 0 if the operation was successful, non-zero otherwise
 */
int (util_sys_inb)(int port, uint8_t *dest);
