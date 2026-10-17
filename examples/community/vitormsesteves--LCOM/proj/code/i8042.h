#pragma once

#include <minix/sysutil.h>
#include <minix/syslib.h>

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 KBC.
 */

#define BIT(n) (0x01<<(n)) /**< @brief macro to turn a n bit to 1  */

/* Keyboard */

#define OBF BIT(0) /**< @brief Output buffer full  */
#define IBF BIT(1) /**< @brief Input buffer full  */
#define TO_ERR BIT(6) /**< @brief Timeout error  */
#define PAR_ERR BIT(7) /**< @brief Parity error */
#define AUX BIT(5) /**< @brief Mouse data  */

#define IRQ_KBD 1 /**< @brief Keyboard IRQ  */

#define STAT_REG 0x64 /**< @brief Status address  */
#define CMD_REG 0x64 /**< @brief Command address  */
#define IN_BUF 0x60 /**< @brief Input buffer address  */
#define OUT_BUF 0x60 /**< @brief Output buffer address  */
#define DATA 0x60 /**< @brief Data address  */
#define KBC_IO_MAX_TRIES 3 /**< @brief KBC max tries  */

#define SW_LEDS 0xED /**< @brief Switch Leds address  */

#define RESEND 0xFE /**< @brief Resend response  */
#define ERROR 0xFC /**< @brief Error response  */
#define ACK 0xFA /**< @brief Acknowledged response */

/* Mouse */
#define WRITE_BYTE 0xD4 /**< @brief Write bytes address */
#define ENABLE_PACKETS 0xF4 /**< @brief Enable mouse data packets */
#define MOUSE_STATUS 0xE9 /**< @brief Mouse status address */
#define STREAM_MODE 0xEA /**< @brief Enable Stream mode address */
#define DISABLE_STREAM 0xF5 /**< @brief Disable Stream mode address */
#define IRQ_MOUSE 12 /**< @brief Mouse IRQ */
#define MOUSE_HOOK_ID 12 /**< @brief Mouse Hook ID */
#define DELAY_US    20000 /**< @brief Delay definition */


#define LEFT_B(b) ((b) & 1) /**< @brief Mouse left button pressed  */
#define RIGHT_B(b) (((b) >> 1) & 1) /**< @brief Mouse right button pressed */
#define MIDDLE_B(b) (((b) >> 2) & 1) /**< @brief Mouse left button pressed */
#define X_OVERFLOW(b) (((b) >> 6) & 1) /**< @brief X values overflow */
#define Y_OVERFLOW(b) (((b) >> 7) & 1) /**< @brief Y values overflow */
#define X_NEGATIVE(b) (((b) >> 4) & 1) /**< @brief X negative values */
#define Y_NEGATIVE(b) (((b) >> 5) & 1) /**< @brief Y negative values */
