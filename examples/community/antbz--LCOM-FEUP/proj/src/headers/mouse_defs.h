#pragma once
#include <lcom/lcf.h>

/** 
 * @defgroup Mouse Macros
 * @{
 * 
 *  Constants for programming the mouse.
 */

#define IRQ12         12 /**< @brief Mouse IRQ line */

#define LB            BIT(0) /**< Left button pressed */
#define RB            BIT(1) /**< Right button pressed */
#define MB            BIT(2) /**< Middle button pressed */
#define MSB_X_DELTA   BIT(4) /**< MSB of X delta byte */
#define MSB_Y_DELTA   BIT(5) /**< MSB of y delta byte */
#define X_OVF         BIT(6) /**< X Overflow */
#define Y_OVF         BIT(7) /**< Y Overflow */

#define READ_CMD        0x20 /**< Read Command Byte */
#define WRITE_CMD       0x60 /**< Write Command Byte */
#define DISABLE_MOUSE   0xA7 /**< Disable Mouse */
#define ENABLE_MOUSE    0xA8 /**< Enable Mouse */
#define CHECK_INTERFACE 0xA9 /**< Check Mouse Interface */
#define WRITE_TO_MOUSE  0xD4 /**< Write Byte to Mouse */

#define RESET           0xFF /**< Mouse Reset */
#define RESEND          0xFE /**< Resend for serial communication errors */
#define SET_DEF         0xF6 /**< Set default values */
#define DISABLE_DR      0xF5 /**< Disable data reporting */
#define ENABLE_DR       0xF4 /**< Enable data reporting */
#define SET_SAMPLE_RATE 0xF3 /**< Set sample rate */
#define SET_RMT_MODE    0xF0 /**< Set remote mode */
#define READ_DATA       0xEB /**< Read data */
#define SET_STREAM_MODE 0xEA /**< Set stream mode */
#define STATUS_RQST     0xE9 /**< Get mouse configuration */
#define SET_RESOLUTION  0xE8 /**< Set resolution */
#define SET_SCALE_2_1   0xE7 /**< Acceleration mode */
#define SET_SCALE_1_1   0xE6 /**< Linear mode */

#define ACK 0xFA /**< @brief Everything OK acknowledgment byte */
#define NACK  0xFE /**< @brief Invalid byte */
#define ERROR 0xFC /**< @brief Second consecutive invalid byte */

/**@}*/
