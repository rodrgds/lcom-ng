#include <lcom/lcf.h>

#include <stdint.h>
/**
 * @brief Invokes sys_inb() system call but reads the value into a uint8_t variable.
 * 
 * @param port	the input port that is to be read
 * @param value	address of 8-bit variable to be update with the value read
 * @return Return 0 upon success and non-zero otherwise
 */
int (util_sys_inb)(int port, uint8_t *value);
