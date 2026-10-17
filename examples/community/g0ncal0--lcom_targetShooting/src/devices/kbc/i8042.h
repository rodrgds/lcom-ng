#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/* IRQ numbers */
#define KEYBOARD_IRQ 1 /** @brief Keyboard IRQ line */
#define MOUSE_IRQ 12 /** @brief Mouse IRQ line */


/* I/O port adresses */
#define KBC_ST_REG 0X64 /** @brief KBC Status Register */
#define KBC_CMD_REG 0x64 /** @brief KBC Command Register */
#define KBC_OUT_BUF 0X60 /** @brief KBC Output Buffer */


/* KBC errors */
#define KBC_PAR_ERR BIT(7) /** @brief KBC Parity Error */
#define KBC_TO_ERR BIT(6) /** @brief KBC Timeout Error */
#define KEYBOARD_ERROR 0X00 /** @brief Keyboard Error */

/* KBC I/O buffer full */
#define KBC_IBF BIT(1) /** @brief KBC Input Buffer Full */
#define KBC_OBF BIT(0) /** @brief KBC Output Buffer Full */

/* KBC Commands*/
#define WRITE_CMD_BYTE 0X60 /** @brief Write Command Byte */
#define READ_CMD_BYTE 0X20 /** @brief Read Command Byte */
#define KBC_ENABLE_INT BIT(0) /** @brief Enable KBC Interrupts */
#define WRITE_BYTE_MOUSE 0XD4 /** @brief Write Byte to Mouse */

/* Mouse Commands */
#define MOUSE_GET_DEVICE_ID 0xF2 /** @brief Get Mouse Device ID */
#define MOUSE_SET_SAMPLE_RATE 0xF3 /** @brief Set Mouse Sample Rate */
#define MOUSE_ENABLE_DATA_REPORT 0xF4 /** @brief Enable Mouse Data Report */
#define MOUSE_DISABLE_DATA_REPORT 0xF5 /** @brief Disable Mouse Data Report */

/* PS/2 Mouse Data Packet */
#define Y_OVF BIT(7) /** @brief Y overflow */
#define X_OVF BIT(6) /** @brief X overflow */
#define MSB_Y_DELTA BIT(5) /** @brief Most significant bit of Y delta */
#define MSB_X_DELTA BIT(4) /** @brief Most significant bit of X delta */
#define VALIDATION_BIT BIT(3) /** @brief Bit 3 of the first byte is always 1 */
#define MIDDLE_BUTTON BIT(2) /** @brief Middle button bit */
#define RIGHT_BUTTON BIT(1) /** @brief Right button bit */
#define LEFT_BUTTON BIT(0) /** @brief Left button bit */

/* Commands auxs */
#define ACK 0xFA /** @brief Acknowledgment */
#define MAX_TRIES 20 /** @brief Max number of tries */
#define DELAY 20000 /** @brief Delay between tries */

/* Mouse ids */
#define PS2_MOUSE 0X00 /** @brief PS/2 Mouse ID */
#define INTELLIMOUSE 0X03 /** @brief Intellimouse ID */

#endif /* _LCOM_I8042_H */
