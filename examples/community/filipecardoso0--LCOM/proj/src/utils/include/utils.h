#ifndef _UTILS_H_
#define _UTILS_H_

#include <lcom/lcf.h>
#include <stdint.h>

/**
 * @brief Entity's position with coordinates row and col;
 * 
 */
typedef struct{
    int row;
    int col;
}position;


/**
 * @brief Gets the LSB of a 2B register;
 * 
 */
int(util_get_LSB)(uint16_t, uint8_t*);

/**
 * @brief Gets the MSB of a 2B register;
 * 
 */
int(util_get_MSB)(uint16_t, uint8_t*);

/**
 * @brief Reads the value of a port and trasfers it to an 8-bit register;
 * 
 */
int (util_sys_inb)(int port, uint8_t* value);

#endif // _UTILS_H_
