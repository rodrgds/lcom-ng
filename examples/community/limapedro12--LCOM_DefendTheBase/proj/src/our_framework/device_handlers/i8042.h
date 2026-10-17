#ifndef __I8042_H
#define __I8042_H

/** @defgroup i8042 i8042
 * @{
 *
 * @brief Keyboard and Mouse constants
 * 
 * Constants for programming the i8042 KBC(for the Keyboard and Mouse).
 */

#define KBC_ST_REG 0x64
#define KBC_OUT_BUF 0x60
#define KBC_OBF BIT(0)
#define KBC_PAR_ERR BIT(7)
#define KBC_TO_ERR BIT(6)
#define WAIT_KBC 20000
#define KBD_IRQ 1

#define KBC_ST_IBF BIT(1)
#define KBC_ST_REG 0x64
#define KBC_CMD_REG 0x64

#define MOUSE_IRQ 12

/**@}*/

#endif
