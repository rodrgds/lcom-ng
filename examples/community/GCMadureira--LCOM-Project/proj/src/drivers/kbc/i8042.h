/** 
 * @file i8042.h
 * @brief This file defines several useful macros to use with the kbc
 */


#ifndef _I8042_H_
#define _I8042_H_

#include <lcom/lcf.h>

#define KEYBOARD_IRQ 1 ///< keyboard IRQ line
#define MOUSE_IRQ 12 ///< mouse IRQ line

/* KBC registers */
#define KBC_STAT_REG 0x64 ///< KBC status register (for reading)
#define KBC_CMD_REG 0x64 ///< KBC command register (for writing)
#define KBC_OUT_REG 0x60 ///< KBC output register (for reading)
#define KBC_CMDARG_REG 0x60 ///< KBC command argument register (for writing)

/* KBC status byte */
#define KBC_PAR_ERR BIT(7) ///< KBC status byte parity error
#define KBC_TO_ERR BIT(6) ///< KBC status byte timeout error
#define KBC_ST_AUX BIT(5) ///< KBC status byte aux bit (1 if mouse data, 0 if keyboard data)
#define KBC_ST_IBF BIT(1) ///< KBC status byte input buffer full
#define KBC_ST_OBF BIT(0) ///< KBC status byte output buffer full

/*KBC commands */
#define RD_CMD_BYTE 0x20 ///< KBC command to read command byte
#define WR_CMD_BYTE 0x60 ///< KBC command to write command byte
#define WR_MOUSE_BYTE 0xD4 ///<  KBC command to write a byte directly to the mouse

/* KBC command byte */
#define KBD_ENABLE_INT BIT(1) ///< KBC command byte, bit to enable keyboard interrupts
#define MOUSE_ENABLE_INT BIT(2) ///< KBC command byte, bit to enable mouse interrupts

/* Keyboard scancodes */
#define ESC_KEY_BREAKCODE 0x81 ///< ESC key scancode's breakcode
#define EXTENDED_SCANCODE 0xE0 ///< First of two bytes scancode
#define BREAKCODE BIT(7) ///< Breakcode bit

/* Mouse byte commands */
#define DISABLE_DATA_REPORTING 0xF5 ///< Mouse command to disable data reporting in stream mode
#define ENABLE_DATA_REPORTING 0xF4 ///< Mouse command to enable data reporting in stream mode
#define MOUSE_BYTE_ACK 0xFA ///< Acknowledgement value if the mouse byte sent is valid
#define MOUSE_BYTE_NACK 0xFE ///< Acknowledgement value if the mouse byte sent is invalid, resend
#define MOUSE_BYTE_ERROR 0xFC ///< Acknowledgement value if the mouse byte sent generated and error

/* Mouse data packet */
#define LEFT_BUTTON_PRESSED BIT(0) ///< This bit is set if the left mouse button is pressed
#define RIGHT_BUTTON_PRESSED BIT(1) ///< This bit is set if the right mouse button is pressed
#define MIDDLE_BUTTON_PRESSD BIT(2) ///< This bit is set if the middle mouse button is pressed
#define MSB_X_DELTA BIT(4) ///< This bit represents the sign of the x movement
#define MSB_Y_DELTA BIT(5) ///< This bit represents the sign of the y movement
#define X_OVERFLOW BIT(6) ///< This bit is set if the 8 bit x movement overflowed
#define Y_OVERFLOW BIT(7) ///< This bit is set if the 8 bit y movement overflowed


#endif /*_I8042_H */
