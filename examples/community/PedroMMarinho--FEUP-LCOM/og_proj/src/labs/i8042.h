#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_


#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 Keyboard. Might need some adjustments.
 */

#define KEYBOARD_IRQ       1                                 /**< @brief Keyboard IRQ line */
#define MOUSE_IRQ           12
#define COMMAND_WAIT tickdelay(micros_to_ticks(20000)) /** <@brief Wait for command to respond*/
/* I/O port addresses */

#define  KBC_OUTPUT_BUFFER 0x60 /**< @brief Port for the output buffer of the keyboard controler */
#define  KBC_STATUS        0x64 /**< @brief Keyboard controler status register */
#define  KBC_WRITE_COMMAND 0x64 /**< @brief Port for writing commands to the keyboard controler */
#define  KBC_COMMAND_ARGS  0x60 /**< @brief Port for writing the arguments of a command to the keyboard controler  */


/* Status register */
#define KBC_STATUS_PARITY       BIT(7) /**< @brief Parity error bit in the status byte  */
#define KBC_STATUS_TIMEOUT      BIT(6) /**< @brief Timeout error bit in the status byte  */
#define KBC_STATUS_OUT_BUF_FULL BIT(0) /**< @brief Output buffer full bit in the status byte  */
#define KBC_STATUS_IN_BUF_FULL  BIT(1) /**< @brief Input buffer full bit in the status byte  */
#define KBC_STATUS_AUX          BIT(5) /**< @brief Mouse data bit in the status byte  */

/* Important scancodes*/
#define ESC_KEY_SCAN 0x81 

/* Keyboard controler commands */
#define KBC_READ_CMD_BYTE      0x20
#define KBC_WRITE_CMD_BYTE     0x60
#define KBC_SELF_CHECK         0xAA
#define KBC_CHECK_INTERFACE    0xAB
#define KBC_DISABLE_INTERFACE  0xAD
#define KBC_ENABLE_INTERFACE   0xAE

#define KBC_WRITE_BYTE_TO_MOUSE 0xD4

/* Command byte bit selectors*/
#define KBC_ENABLE_KB_INTERRUPT     BIT(0)
#define KBC_ENABLE_MOUSE_INTERRUPT  BIT(1)
#define KBC_DISABLE_KB_INTERFACE    BIT(4)
#define KBC_DISABLE_MOUSE_INTERFACE BIT(5) 

/* Mouse commands*/
#define MOUSE_DISABLE_DATA_REPORTING 0xF5
#define MOUSE_ENABLE_DATA_REPORTING 0xF4

/* Mouse Data Bits */
#define MOUSE_DATA_PACKET_XOVFL BIT(7)
#define MOUSE_DATA_PACKET_YOVFL BIT(6)
#define MOUSE_DATA_PACKET_YMSB BIT(5)
#define MOUSE_DATA_PACKET_XMSB BIT(4)
#define MOUSE_DATA_PACKET_MB BIT(2)
#define MOUSE_DATA_PACKET_RB BIT(1)
#define MOUSE_DATA_PACKET_LB 1

/* Acknowlegment bytes*/
#define MOUSE_OK 0xFA
#define MOUSE_INVALID_BYTE 0xFE
#define MOUSE_ERROR 0xFC

/**@}*/

#endif
