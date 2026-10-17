#ifndef _LCOM_MOUSE_MACROS_H_
#define _LCOM_MOUSE_MACROS_H_

#include <lcom/lcf.h>

/** @defgroup i8254 i8254
 * @{
 *
 * Constants for programming the i8042 Keyboard.
 */


#define KEYBOARD_IRQ 1 /**< @brief Keyboard IRQ line */
#define MOUSE_IRQ 12 /**< @brief Mouse IRQ line */
#define DELAY_US 20000 /**< @brief Delay in micro seconds */
#define ESC_BREAKCODE 0x81 /**<@brief ESC break code */
#define SCANCODE_2BYTES 0xE0 /**<@brief Used to identify if it is/isn't a 2 byte scancode */
#define SCANCODE_IS_MAKECODE BIT(7) /**<@brief Bit used to identify if it is a make or break code */
#define INVALID_SCANCODE 0 /**<@brief Indicates that the scancode is invalid */


/* Return values */

#define OK 0 /**< @brief Function worked as expected */
#define ERROR -1 /**< @brief There was an error in the function */
#define NO_DATA_AVAIVABLE 1 /**< @brief There is no data available for readding in the output buffer */


/* I/O port addresses */

#define KBC_OUT_BUF 0x60 /**< @brief Address where the kbc writes (scancodes, commands related outputs, ...) */
#define KBC_WRITE_COMMAND_ARG 0x60 /**< @brief Address where command byte is written */
#define KBC_STAT_REG 0x64 /*<@brief Address where the status of the kbc is read */
#define KBC_WRITE_COMMAND_CMD 0x64 /**< @brief Address where Keyboard-Commands are written */

/* Status register bits definition */

#define KBC_STATUS_OBF BIT(0) /*<@brief Indicates if the output buffer is full, meaning there is/isn't data to be read */
#define KBC_STATUS_IBF BIT(1) /*<@brief Indictes if the input buffer is full, meaning if we can/cannot write commands */
#define KBC_STATUS_AUX BIT(5) /*<@brief Indicates if the data is from the mouse or keyboard */
#define KBC_STATUS_TIMEOUT BIT(6) /*<@brief Indicates if there was any error */
#define KBC_STATUS_PARITY BIT(7) /*<@brief Indicates if there was any error */

/* Command Byte bits definition */

#define KBC_KEYBOARD_INT BIT(0) /*<@brief Bit on the command byte that enables the interrupts of keyboard */

/* Commands related to the command byte */

#define KBC_READ_COMMAND_BYTE 0x20 /*<@brief KBC command for reading the command byte */
#define KBC_WRITE_COMMAND_BYTE 0x60 /*<@brief KBC command for writting a new command byte */

/* Mouse Data Packet */
#define MOUSE_Y_OV BIT(7) /**<@brief Mouse Y overflow bit */
#define MOUSE_X_OV BIT(6) /**<@brief Mouse X overflow bit */
#define MOUSE_MB BIT(2) /**<@brief Mouse middle button state (pressed or not) */
#define MOUSE_RB BIT(1) /**<@brief Mouse right button state (pressed or not) */
#define MOUSE_LB BIT(0) /**<@brief Mouse left button state (pressed or not) */

/* Mouse command bytes */
#define MOUSE_WRITE_BYTE 0xD4 /**<@brief Write byte to mouse */
#define MOUSE_ENABLE_DATA_REPORTING 0xF4 /**<@brief Enable data reporting */
#define MOUSE_DISABLE_DATA_REPORTING 0xF5 /**<@brief Disable data reporting */
#define MOUSE_ACK 0xFA /**<@brief Mouse acknowledgment */
#define MOUSE_SET_STREAM 0xEA /**<@brief Set stream mode */
#define MOUSE_SET_REMOTE 0xF0 /**<@brief Set remote mode */
#define MOUSE_READ_DATA 0xEB /**<@brief Read mouse data */

/* Mouse gesture */
#define BUTTON_PRESSED 1 /**<@brief Mouse button pressed */
#define GESTURE_COMPLETE 0 /**<@brief Gesture complete */
#define GESTURE_IN_PROGRESS -1 /**<@brief Gesture in progress */


/**@}*/

#endif /* _LCOM_I8042_H */
