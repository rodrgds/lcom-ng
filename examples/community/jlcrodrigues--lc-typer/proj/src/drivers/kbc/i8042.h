#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/** @defgroup kbc i8042
 * @{
 *
 * Constants for programming the i8042 KBC.
 */

/* I/O port addresses */

#define KBC_OUT_BUFF 0x60 /**< @brief KBC out buffer */
#define KBC_STATUS 0x64 /**< @brief KBC status port */
#define KBC_COMMAND_ARG 0x60 /**< @brief KBC command arguments port */
#define KBC_COMMAND 0x64 /**< @brief KBC command port */
#define MOUSE_WRITE_COMMAND_BYTE 0xD4

/* KBC commands */

#define READ_COMMAND_BYTE 0x20
#define WRITE_COMMAND_BYTE 0x60

/* KBC Mouse commands */

#define DISABLE_DATA_REPORTING 0xF5
#define ENABLE_DATA_REPORTING 0xF4

/* IRQ lines */

#define KEYBOARD_IRQ 1
#define MOUSE_IRQ 12

/* Relevant codes */

#define BREAKCODE_ESC 0x81
#define BREAKCODE_TAB 0x8f
#define BREAKCODE_ENTER 0x9c
#define MAKECODE_BACKSPACE 0X0E
#define FIRST_BYTE 0xE0

/* Masks */

#define MSB BIT(7)
#define OUT_BUFFER_FULL BIT(0)
#define IN_BUFFER_FULL BIT(1)
#define AUX BIT(5)
#define KEYBOARD_PARITY BIT(7)
#define KEYBOARD_TIMEOUT BIT(6)
#define INTERRUPT_KEYBOARD_OBF BIT(0)
#define KBC_READ_MASK (KEYBOARD_PARITY | KEYBOARD_TIMEOUT | AUX)
#define MOUSE_READ_MASK (OUT_BUFFER_FULL | !KEYBOARD_PARITY | AUX | !KEYBOARD_TIMEOUT)

/* Mouse */

#define COMPLETE_SIGNAL 0xFF00
#define ACK_SUCCESS 0xFA
#define PACKET_NUMBER 3
#define KBC_TRIES_NUM 3

/* Mouse first byte masks */

#define LEFT_BYTE BIT(0)
#define RIGHT_BYTE BIT(1)
#define MIDDLE_BYTE BIT(2)
#define SIGN_X BIT(4)
#define SIGN_Y BIT(5)
#define OVFL_X BIT(6)
#define OVFL_Y BIT(7)


/**@}*/

#endif
