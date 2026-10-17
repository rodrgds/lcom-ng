/**
 * @file i8042.h
 * 
 * @brief Constants for programming the i8042 KBC Controller
*/


#ifndef _I8042_H_
#define _I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 KBC Controller.
 */

#define DELAY_US    20000   /** @brief Delay to wait*/
#define LIMIT_TRIES 20      /** @brief Maximum  number of tries*/

/* Interrupt Lines*/

#define KEYBOARD_IRQ_LINE 1 /** @brief  Keyboard's IRQ line */
#define MOUSE_IRQ_LINE 12   /** @brief  Mouse's IRQ line */


/* KBC Port addresses */

#define KBC_STAT_REG 0x64  /**< @brief  KBC's STAT_REG */
#define KBC_CTRL_REG 0x64   /**< @brief  KBC's CTRL_REG */
#define KBC_IN_BUF 0x60       /**< @brief  KBC's IN_BUF */
#define KBC_OUT_BUF 0x60   /**< @brief  KBC's OUT_BUF  */

/* KBC Status Masks */

#define PAR_ERROR_BIT BIT(7)   /** @brief Parity error bit */
#define TIME_ERROR_BIT BIT(6)  /** @brief Time error bit */
#define MOUSE_DATA_BIT BIT(5)  /** @brief Data is from Mouse bit */
#define IBF_BIT BIT(1)         /** @brief  Input Buffer Full bit */
#define OBF_BIT BIT(0)         /** @brief  Output Buffer Full bit*/


/* KBC Commands */

#define READ_COMMAND_BYTE 0x20  /** @brief Command to read a command byte */
#define WRITE_COMMAND_BYTE 0x60  /** @brief Command to write a command byte */
#define DISABLE_KBD_INTERFACE 0xAD    /** @brief Command to disable the KBD Interface */
#define ENABLE_KBD_INTERFACE 0xAE    /** @brief Command to enable the KBD Interface */
#define WRITE_BYTE_TO_MOUSE 0xD4     /** @brief Command to write a byte to the mouse */

/* Command Byte */
#define ENABLE_KEYBOARD_INT BIT(0)  /** @brief Command byte ti enable keyboard interrupts */



/* Scancodes */
#define BREAK_CODE BIT(7)   /** @brief Bit is set if it's a breakcode*/
#define FIRST_OF_TWO_BYTES 0xE0 /** @brief First byte of a two-byte scancode*/


#define ESC_BREAK_CODE 0x81 /** @brief ESC key breakcode*/
#define ARROW_RIGHT_MAKE 0x4d   /** @brief Right Arrow key makecode*/
#define ARROW_RIGHT_BREAK 0xcd  /** @brief Right Arrow key breakcode*/
#define ARROW_LEFT_MAKE 0x4b    /** @brief Left Arrow key makecode*/
#define ARROW_LEFT_BREAK 0xcb   /** @brief Left Arrow key breakcode*/
#define ARROW_UP_MAKE 0x48      /** @brief Up Arrow key makecode*/
#define ARROW_UP_BREAK 0xcd     /** @brief Up Arrow key breakcode*/
#define ARROW_DOWN_MAKE 0x50    /** @brief Down Arrow key makecode*/
#define ARROW_DOWN_BREAK 0xcb   /** @brief Down Arrow key breakcode*/
#define SPACE_BAR_MAKE 0x39     /** @brief Spacebar key makecode*/
#define BACK_SPACE_MAKE 0x0e     /** @brief Backspace key makecode*/
#define BACK_SPACE_BREAK 0x8e    /** @brief Backspace key breakcode*/
#define A_MAKE  0x1E  /** @brief A key makecode*/
#define A_BREAK 0x9e  /** @brief A key breakcode*/
#define D_MAKE 0x20   /** @brief D key makecode*/
#define D_BREAK 0xa0  /** @brief D key breakcode*/
#define W_MAKE 0x11   /** @brief W key makecode*/
#define W_BREAK 0x91  /** @brief W key breakcode*/
#define S_MAKE 0x1f   /** @brief S key makecode*/
#define S_BREAK 0x9f  /** @brief S key breakcode*/
#define Q_MAKE 0x10   /** @brief Q key makecode*/
#define Q_BREAK 0x90  /** @brief Q key breakcode*/
#define ALT_LEFT_MAKE 0x38  /** @brief Left ALT key makecode*/
#define ALT_LEFT_BREAK 0xb8  /** @brief Left ALT key breakcode*/



#define PCKT_CTRL_BYTE BIT(3) /**< @brief Identifies the  Control byte. It has the BIT(3) set */


/* Mouse Acknowledgement Byte */
#define ACK 0xFA   /** @brief Acknowledgement response */
#define NACK 0xFE  /** @brief Non-acknowledgement response  */
#define ERROR 0xFC /** @brief Error response */

/* Mouse Command Bytes */
#define DISABLE_DATA_REPORT 0xF5 /** @brief Mouse Command Byte to disable Mouse data report*/
#define ENABLE_DATA_REPORT 0xF4  /** @brief Mouse Command Byte to enable Mouse data report*/
#define STREAM_MODE 0xEA   /** @brief Mouse Command Byte to set Mouse Stream Mode*/
#define REMOTE_MODE 0xF0  /** @brief Mouse Command Byte to set Mouse Remote Mode*/

/* Mouse First Byte */
#define Y_OV BIT(7)   /** @brief Mouse Packet Y value*/
#define X_OV BIT(6)   /** @brief Mouse Packet X value*/
#define DELTA_Y_SIGNAL BIT(5) /** @brief Mouse Packet Delta Y signal*/
#define DELTA_X_SIGNAL BIT(4) /** @brief Mouse Packet Delta X signal*/
#define MIDDLE_BUTTON BIT(2)  /** @brief Mouse Packet Middle Button bit*/
#define RIGHT_BUTTON BIT(1)    /** @brief Mouse Packet Right Button bit*/
#define LEFT_BUTTON BIT(0)     /** @brief Mouse Packet Left Button bit*/

/* Mouse Buttons */
#define MCB_PRESSED BIT(2)   /** @brief Mouse Middle button pressed*/
#define MRB_PRESSED BIT(1)    /** @brief MouseRight button pressed*/
#define MLB_PRESSED BIT(0)    /** @brief Mouse Left button pressed*/




#endif /* _I8042_H */
