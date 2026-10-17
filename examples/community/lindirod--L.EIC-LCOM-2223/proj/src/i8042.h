/** @defgroup i8042 i8042
 * @{
 *
 * Constants related to the keyboard and the mouse
 */
#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define DELAY_US 20000

#define MOUSE_IRQ 12
#define KBC_IRQ 1

#define IRQ_SET BIT(1)
#define KBD_ESC_BREAK 0x81
#define TWO_BYTES 0xE0
#define KBD_BREAK BIT(7)

#define KBC_OUT_BUF 0x60
#define KBC_ST_REG 0x64 
#define KBC_CMD_REG 0x64

#define KBC_READ 0x20
#define KBC_WRITE 0x60

#define KBC_PAR_ERR BIT(7)
#define KBC_TO_ERR BIT(6)
#define KBC_AUX BIT(5)
#define KBC_IBF BIT(1)
#define KBC_OBF BIT(0)

//These commands must be written using KBC_CMD_REG defined above

#define KBC_READ_CMB_BYTE 0x20
#define KBC_WRITE_CMB_BYTE 0x60
#define KBC_SELF_TEST 0xAA //Returns 0x55 if okay, 0xFC if error
#define KBC_CHECK_KB_INTERFACE 0xAB //Returns 0, if OK
#define KBC_ENABLE_KB_INTERFACE 0xAE
#define KBC_DISABLE_KB_INTERFACE 0xAD

//Mouse

//The data packet
#define LEFT_BIT BIT(0)
#define RIGHT_BIT BIT(1)
#define MIDDLE_BIT BIT(2)
#define X_SIGN BIT(4)
#define Y_SIGN BIT(5)
#define X_OVFL BIT(6)
#define Y_OVFL BIT(7)

#define WRITE_BYTE_TO_MOUSE 0xD4
#define DISABLE_MOUSE 0xA7
#define DISABLE_DATA_REPORTING 0xF5
#define ENABLE_DATA_REPORTING 0xF4
#define SET_REMOTE_MODE 0xF0
#define READ_DATA 0xEB
#define SET_STREAM_MODE 0xEA

//Mouse Controller command responses
#define ACK 0xFA   //Acknowledged command or argument
#define NACK 0xFE  //Invalid byte
#define ERROR 0xFC //Second consecutive invalid byte


#endif /* _LCOM_I8042_H */
