/**
 * @file i8042.h
 * @brief This file contains key definitions and constants for the i8042 keyboard and mouse controller.
 */

#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/* ----- KBC ----- */

/* KBC ports regs and command bytes */
#define KBC_STATUS_REG 0x64
#define KBC_IN_BUF 0x64
#define KBC_OUT_BUF 0x60
#define KBC_READ_CMD 0x20
#define KBC_WRITE_CMD 0x60

/* Status register */
#define PARITY_ERROR BIT(7)
#define TIMEOUT_ERROR BIT(6)
#define MOUSE_DATA BIT(5)
#define KEYBOARD_INHIBIT BIT(4)
#define INPUT_BUF_FULL BIT(1)
#define OUTPUT_BUF_FULL BIT(0) 
#define ENABLE_INTERRUPT BIT(0)

/* General constants */
#define WAIT_KBC 20000
#define MAX_ATT 10

/* ----- Keyboard ----- */

/* IRQs */
#define KBD_IRQ 1

/* keyboard auxiliars */
#define MAKE_CODE_BIT BIT(7)
#define TWO_BYTE_CODE 0xE0

/* Key codes */
#define ESC_KEY 0x01
#define ENTER_KEY 0x1C
#define UP_ARROW 0x48
#define DOWN_ARROW 0x50

/* ----- Mouse ----- */

#define MOUSE_IRQ 12

/* Mouse-related KBC Commands */
#define DISABLE_MOUSE 0xA7
#define ENABLE_MOUSE 0xA8
#define CHECK_MOUSE_INTERFACE 0xA9
#define WRITE_BYTE_TO_MOUSE 0xD4

/* Mouse Commands */
#define MOUSE_RESET 0xFF
#define MOUSE_RESEND 0xFE
#define DISABLE_DATA_REPORTING 0xF5
#define ENABLE_DATA_REPORTING 0xF4
#define SET_STREAM_MODE 0xEA
#define READ_MOUSE_DATA 0xEB

/* Mouse Data Packet */
#define MOUSE_Y_OVERFLOW BIT(7)
#define MOUSE_X_OVERFLOW BIT(6)
#define MSB_Y_DELTA BIT(5)
#define MSB_X_DELTA BIT(4)
#define FIRST_BYTE_SET BIT(3)
#define MOUSE_MIDDLE_BUTTON BIT(2)
#define MOUSE_RIGHT_BUTTON BIT(1)
#define MOUSE_LEFT_BUTTON BIT(0)

/* Acknowledgment commands */
#define ACK 0xFA
#define NACK 0xFE
#define ERROR 0xFC

#endif /* _LCOM_I8042_H */
