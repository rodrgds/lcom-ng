/**
 * @file i8042
 * @brief This file has all defines related to the keyboard.
 */

#ifndef _MKBD_H_
#define _MKBD_H_

/** @brief Keyboard interrupt request line */
#define KBD_IRQ     1

/** @brief Keyboard status register I/O port */
#define STATUS_PORT 0x64
/** @brief Keyboard output buffer I/O port */
#define KBD_OUT_BUF 0x60

/** @brief Command register I/O port */
#define INPUT_CMD   0x64
/** @brief Data register I/O port */
#define INPUT_DATA  0x60

/** @brief Delay in microseconds for keyboard operations */
#define DELAY_US    20000

/** @brief Mouse interrupt request line */
#define MOUSE_IRQ   12

/** @brief Mouse command register */
#define MOUSE_CMD   0xD4

/** @brief Bitmask for mouse left button */
#define MOUSE_LEFT_BTN    BIT(0)
/** @brief Bitmask for mouse right button */
#define MOUSE_RIGHT_BTN   BIT(1)
/** @brief Bitmask for mouse middle button */
#define MOUSE_MIDDLE_BTN  BIT(2)
/** @brief Bitmask for mouse sync byte */
#define MOUSE_SYNC_BYTE   BIT(3)
/** @brief Bitmask for 9th bit of mouse x displacement */
#define MOUSE_DX_BIT9     BIT(4)
/** @brief Bitmask for 9th bit of mouse y displacement */
#define MOUSE_DY_BIT9     BIT(5)
/** @brief Bitmask for mouse x overflow */
#define MOUSE_X_OVF       BIT(6)
/** @brief Bitmask for mouse y overflow */
#define MOUSE_Y_OVF       BIT(7)

/** @brief Bitmask for output buffer readiness */
#define OUT_BUF_READY     BIT(0)
/** @brief Bitmask for input buffer readiness */
#define IN_BUF_READY      BIT(1)
/** @brief Bitmask for data source (mouse) */
#define DATA_FROM_MOUSE   BIT(5)
/** @brief Bitmask for time out error */
#define TIME_OUT_ERROR    BIT(6)
/** @brief Bitmask for parity error */
#define PARITY_ERROR      BIT(7)

/** @brief Enable data report command */
#define ENBL_DATA_REP 0xF4
/** @brief Disable data report command */
#define DIS_DATA_REP  0xF5
/** @brief Forward request command */
#define REQUEST_FRWD  0xD4
/** @brief Command execution success code */
#define SUCCESS       0xFA

/** @brief Keyboard scan code for the ESC (escape) key */
#define ESC 0x01
/** @brief Keyboard scan code for the ENTER key */
#define ENTER 0x9c

#endif
