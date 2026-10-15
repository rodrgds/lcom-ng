#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{
 * 
 * Constants for programming the KBC controller (i8042).
 */

#define KBD_IRQ        1      /**< @brief Keyboard IRQ line */
#define KBD_FIRST_BYTE 0xE0   /**< @brief First byte of two byte scancodes (usually) */
#define KBD_BREAK_BIT  BIT(7) /**< @brief MSB of code is set to 1 */
#define DELAY_US       20000  /**< @brief Keyboard responds to a command in 20 ms */

/* ESC Codes */

#define ESC_MK_CODE 0x01 /**< @brief Make code of the ESC key */
#define ESC_BK_CODE 0x81 /**< @brief Break code of the ESC key */

/* KBC Port Adresses */

#define KBC_ST_REG  0x64 /**< @brief Status register (for reading the KBC state) */
#define KBC_CMD_REG 0x64 /**< @brief Address used to write KBC commands */
#define KBC_CMD_ARG 0x60 /**< @brief Address used to pass arguments of KBC commands */
#define KBC_OUT_BUF 0x60 /**< @brief Output buffer register (scancodes or commands return values) */

/* Status Register */

#define KBC_ST_PARITY  BIT(7)            /**< @brief Parity error - invalid data */
#define KBC_ST_TIMEOUT BIT(6)            /**< @brief Timeout error - invalid data */
#define KBC_ST_ERROR   (BIT(7) | BIT(6)) /**< @brief Error in KBD-KBC communication */
#define KBC_ST_AUX     BIT(5)            /**< @brief Mouse data (1-TRUE)(0-FALSE)*/
#define KBC_ST_INH     BIT(4)            /**< @brief Inhibit flag: 0 if keyboard is inhibited */
#define KBC_ST_IBF     BIT(1)            /**< @brief Input buffer full */
#define KBC_ST_OBF     BIT(0)            /**< @brief Output buffer full */

/* KBC Commands for PC-AT/PS2 */

#define KBD_RCMD      0x20 /**< @brief Read command byte (returns command byte) */
#define KBD_WCMD      0x60 /**< @brief Write command byte (takes arguments: command byte) */
#define KBD_CHECK     0xAA /**< @brief Check KBC (self-test) (returns 0x55 if OK, 0xFC if error) */
#define KBD_ITF_CHECK 0XAB /**< @brief Check keyboard interface (returns 0 if OK) */
#define KBD_ITF_OFF   0XAD /**< @brief Disable keyboard interface */
#define KBD_ITF_ON    0xAE /**< @brief Enable keyboard interface */

/* KBC Command Byte */

#define KBC_CMD_DIS_MOUSE BIT(5) /**< @brief Disable mouse interface */
#define KBC_CMD_DIS_KBD  BIT(4) /**< @brief Disable keyboard interface */
#define KBC_CMD_INT_MOUSE BIT(1) /**< @brief Enable interrupt on OBF, from mouse */
#define KBC_CMD_INT_KBD  BIT(0) /**< @brief Enable interrupt on OBF, from keyboard */

/**@}*/

#endif /* _LCOM_I8042_H */
