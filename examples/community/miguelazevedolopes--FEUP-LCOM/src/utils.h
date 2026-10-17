#include <lcom/lcf.h>

/** @defgroup utils utils.h
 * @{
 *
 * This file contains useful functions .
 * 
 */

int(util_get_LSB)(uint16_t val, uint8_t *lsb);
int(util_get_MSB)(uint16_t val, uint8_t *msb);
int (util_sys_inb)(int port, uint8_t *value);
/**@}*/
