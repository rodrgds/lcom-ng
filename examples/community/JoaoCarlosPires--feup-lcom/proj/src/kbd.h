#ifndef KBD_H
#define KBD_H

#include <stdbool.h>
#include <stdint.h>

#define OUT_BUF 0x60

#define IN_BUF 0x60

#define STAT_REG 0x64

#define KBC_CMD_REG 0x64

#define KBD_IRQ 1

#define IBF BIT(1)

#define OBF BIT(0)

#define PAR_ERR BIT(7)

#define TO_ERR BIT(6)

#define DELAY_US 20000

/**
 * @brief Interrupt handler for keyboard
 *
 */
void(kbc_ih)();

/**
 * @brief Reads the value of port into variable value, using cast and sys_inb()
 *
 * @param port value to be read
 * @param value variable to set with data from port
 * @return Return 0 upon success and non-zero otherwise
 */
int(util_sys_inb)(int port, uint8_t *value);

/**
 * @brief Sets the variable msb with the 8 most significant bits of variable val
 *
 * @param val value to be read and get the 8 most significant bits
 * @param msb variable to set with the 8 most significant bits
 * @return Return 0 upon success and non-zero otherwise
 */
int get_MSB(uint8_t val, uint8_t *msb);

/**
 * @brief Sets the variable lsb with the 8 least significant bits of variable val
 *
 * @param val value to be read and get the 8 least significant bits
 * @param lsb variable to set with the 8 least significant bits
 * @return Return 0 upon success and non-zero otherwise
 */
int get_LSB(uint16_t val, uint8_t *lsb);

#endif
