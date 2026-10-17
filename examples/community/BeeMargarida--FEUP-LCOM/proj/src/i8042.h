#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <minix/syslib.h>
#include <minix/drivers.h>

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming various peripherals
 */

#define BIT(n) (0x01<<(n)) /**< @brief put the chosen bit as 1 */

#define KBD_IRQ 			1 /** < @brief keyboard IRQ */

#define BC_ESC          0x81 /** < @brief breakcode of ESC key*/

#define DELAY_US     	20000 /** < @brief time to wait in tickdelay */

#define KBC_CMD_REG     0x64 /** < @brief KBC Register */

#define STAT_REG        0x64 /** < @brief KBC Register for status reading*/

#define IN_BUF			0x60/** < @brief KBC IN Buffer (writable register)*/

#define OUT_BUF 		0x60 /** < @brief KBC OUT Buffer (to read return values)*/

#define TWO_BYTE   		0xE0 /** < @brief first byte of two byte scancode */

#define LED_CMD			0xED /** < @brief switch ON/OFF LEDs*/

//MOUSE
#define IBF				(BIT(1)) /** < @brief Input buffer full*/
#define OBF				(BIT(0)) /** < @brief Output buffer full*/
#define PAR_ERR			(BIT(7)) /** < @brief Parity Error*/
#define TO_ERR 			(BIT(6)) /** < @brief Timeout Error*/

#define ACK				0xFA /** < @brief Everything went well*/
#define RESEND			0xFE /** < @brief Invalid information, resend it*/
#define ERROR			0xFC /** < @brief Fails consecutive to get valid information*/

#define BREAK_CODE    (BIT(7)) /** < @brief first byte of a breakcode*/

#define  KEY_W 		    0x0011 /** < @brief key W scancode*/
#define  KEY_W_B		0x0091 /** < @brief key W breakcode*/
#define  KEY_S			0x001F /** < @brief key S scancode*/
#define  KEY_S_B		0x009F /** < @brief key S breakcode*/
#define  KEY_O			0x0018 /** < @brief key O scancode*/
#define  KEY_O_B		0x0098 /** < @brief key O breakcode*/
#define  KEY_K			0x0025 /** < @brief key K scancode*/
#define  KEY_K_B		0x00A5 /** < @brief key K breakcode*/


#define MOUSE_IRQ 		12  /** < @brief Mouse IRQ*/
#define WRITE_MOUSE		0xD4 /** < @brief Write byte to mouse*/
#define STREAM_MODE		0xEA /** < @brief Enable stream mode (data on events)*/
#define DATA_MODE		0xF4 /** < @brief Enable data reporting */
#define DIS_STREAM_MODE	0xF5 /** < @brief Disable data reporting */
#define MOUSE_CONFIG	0xE9 /** < @brief Get mouse configuration*/


#define XOV				BIT(6) /** < @brief X overflow*/
#define YOV 			BIT(7) /** < @brief Y overflow*/
#define LB  			BIT(0) /** < @brief Left button*/
#define MB 				BIT(2) /** < @brief Middle button*/
#define RB				BIT(1) /** < @brief Right button*/
#define YSIGN			BIT(5) /** < @brief Y sign*/
#define XSIGN			BIT(4) /** < @brief X sign*/

#define BYTE_3			BIT(3) /** < @brief Bit 3 of Byte*/


/**@}*/


#endif
