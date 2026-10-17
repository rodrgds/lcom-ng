#pragma once

/** 
 * @defgroup Keyboard Macros
 * @{
 * 
 *  Constants for programming the PC's Keyboard.
 */

#define KBD_STAT_REG    0x64 /**< @brief KBC Status Register */
#define KBC_CMD_REG     0x64 /**< @brief KBC Command Register */
#define OUT_BUF         0x60 /**< @brief Output Buffer */
#define KBC_ARG_REG     0x60 /**< @brief KBC Argument Register */
#define IRQ1            1 /**< @brief Keyboard IRQ line */

#define PARITY    BIT(7) /**< @brief Parity error */
#define TIMEOUT   BIT(6) /**< @brief Timeout error */
#define AUX       BIT(5) /**< @brief Mouse data */
#define INH       BIT(4) /**< @brief Inhibit flag */
#define A2        BIT(3) /**< @brief A2 input line */
#define SYS       BIT(2) /**< @brief System flag */
#define IBF       BIT(1) /**< @brief Input buffer full */
#define OBF       BIT(0) /**< @brief Output buffer full */

#define TWO_BYTE 0xe0 /**< @brief Two byte code */

#define READ_CMD 0x20 /**< @brief Read command */
#define WRITE_CMD 0x60 /**< @brief Write command */

#define WAIT_KBC  20000 /**< @brief Microseconds to wait */

#define ESC       0x81 /**< @brief Esc key code */
#define NKEY      0xb1 /**< @brief N key code */
#define SPACE     0x39 /**< @brief Space key code */
#define CKEY      0xae /**< @brief C key code */
#define KKEY      0xa5 /**< @brief K key code */
#define AKEY      0x1e /**< @brief K key code */
#define DKEY      0x20 /**< @brief K key code */

/**@}*/
