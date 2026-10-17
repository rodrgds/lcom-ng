#ifndef __I8042_H_
#define __I8042_H_

#include "utils.h"

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 KBC
 */


//bitwise operations

#define IN_OK 	BIT(1)   /**< @brief check available input */
#define OUT_OK 	BIT(0)	 /**< @brief check available output */

//i8042 ports I/O access

#define STAT_REG 	0x64   	 /**< @brief read kbc state */

//use sysoutb e sysinb
//input

#define KBD_ARG 	0x60  	 /**< @brief adress for arguments to keyboard command */

#define KBD_CMD 	0x60  	/**< @brief adress for keyboard command */

#define KBC_CMD 	0x64  	/**< @brief adress for kbc command */

#define KBC_ARG 	0x60	/**< @brief adress for arguments to keyboard command */

//output

#define OUT_BUF 	0x60  /**< @brief adress of output buffer */


//bitmasks

#define ESC_BREAK 0x81 /**< @brief esc break code */

#define READ_CMD_BYTE 0x20  /**< @brief read command byte */

#define TWO_BYTE_SCAN 0xE0 /**< @brief two byte make code */

#define KEYBOARD_IRQ 1 /**< @brief kbd irq */

#define BREAK_CODE BIT(7) /**< @brief if it is a break code */

#define STAT_ERR (BIT(7) | BIT(6))/**< @brief check parity and timeout */


//mouse stuff

#define MOUSE_IRQ 12 /**< @brief mouse irq */

//MOUSE COMMANDS

#define ENABLE_MOUSE 0xA8 /**< @brief enable mouse command */

#define WRITE_TO_MOUSE 0xD4 /**< @brief write to mouse command */

#define MOUSE_STATUS 0xE9 /**< @brief check mouse status command */

#define DISABLE_STREAM 0xF5  /**< @brief disable stream mouse command */

#define SET_STREAM 0xEA  /**< @brief enable stream mouse command */

#define ENABLE_PACKETS 0xF4  /**< @brief enable mouse packets command */

#define LMB_PRESSED BIT(0) /**< @brief check left mouse button */

#define X_SIGNED 	BIT(4) /**< @brief check packet X sign */
#define Y_SIGNED 	BIT(5)	/**< @brief check packet Y sign */
#define X_OVERFLOW 	BIT(6) /**< @brief check packet X overflow */
#define Y_OVERFLOW 	BIT(7)	/**< @brief check packet Y overflow */


//other

#define DELAY_MS    20000  /**< @brief wait 2 miliseconds to receive */

#define N_TRY 		5 /**< @brief number of reading tries */



//responses

#define KBD_ACK			0xFA	//**< @brief acknowledged status  kbd */
#define KBD_RESEND		0xFE	/**< @brief resend status kbd */
#define KBD_ERROR		0xFC	/**< @brief error status kbd */


//responses

#define ACK			0xFA	//**< @brief acknowledged status  mouse */
#define NACK		0xFE	/**< @brief resend status mouse */
#define ERROR		0xFC	/**< @brief error status mouse */

/**@}*/

#endif /* _I8042_H */
