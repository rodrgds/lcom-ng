#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8254 Timer. Needs to be completed.
 */

#define KBC_ST_REG 0x64
#define KBC_CMD_REG 0x64
#define KBC_OUT_BUF 0x60
#define KEYBOARD_IRQ 1
#define MOUSE_IRQ 12

//Stat-register BIT7
#define KBC_PAR_ERR 0x80
#define KBC_OBF BIT(0)
#define KBC_ST_IBF BIT(1)
//Stat-register BIT6
#define KBC_TO_ERR 0x40

#define DELAY_US 20000

#define WRITE_MOUSE 0xD4

#define ACK 0xFA
#define NACK 0xFE

/**@}*/

#endif /* _LCOM_I8042_H */
