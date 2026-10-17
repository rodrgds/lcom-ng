#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming KBC.
 */

#define DELAY_US 20000
#define BIT(n) (0x01<<(n))

#define KBD_IRQ 1 /**< @brief Keyboard IRQ line >*/
#define MOUSE_IRQ 12 /**< @brief Mouse IRQ line >*/

/* I/O port addresses */

// KBC Ports
#define OUT_PORT	0x60
#define CMD_PORT 	0x64
#define ARGS_PORT 	0x60
#define OUT_BUF 0x60 /**< @brief Keyboard Output Buffer >*/
#define IN_BUF 0x60 /**< @brief Keyboard Input Buffer >*/

// KBC Status Byte
#define OUT_BUF_FULL 	BIT(0)
#define IN_BUF_FULL 	BIT(1)


// KBC Mouse Commands
#define READ_CMD_BYTE 0x20
#define WRITE_CMD_BYTE 0x60
#define MOUSE_ENABLE 0x48
#define MOUSE_DISABLE 0x48
#define CHECK_MOUSE_ITF 0xA9
#define WRITE_AUX 0xD4 /**< @brief Write (byte) to the Auxiliary Device >*/

// Mouse Commands
#define SET_STREAM 0xEA
#define DISABLE_STREAM 0xF5
#define ENABLE_SENDING_DATA 0xF4

/**@}*/

#endif /* _LCOM_I8042_H */
