#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 Keyboard Controller.
 */

#define KBD_IRQ                 1 /**< @brief KBD IRQ line */
#define MOUSE_IRQ               12 /**< @brief Mouse IRQ line */
#define DELAY_US                20000 /**< @brief Delay between two consecutive reads of the output buffer */

/* I/O port addresses */

#define KBC_STAT_REG            0x64 /**< @brief KBC status register */
#define KBC_OUT_BUF             0x60 /**< @brief KBC output buffer */
#define KBC_IN_BUF              0x60 /**< @brief KBC input buffer */
#define KBC_CMD_REG             0x64 /**< @brief KBC command register */

/* Keyboard Controller Status Register */

#define KBC_STAT_PARITY         BIT(7) /**< @brief Parity error */
#define KBC_STAT_TIMEOUT        BIT(6) /**< @brief Timeout error */
#define KBC_STAT_AUX            BIT(5) /**< @brief Mouse data */
#define KBC_STAT_INH            BIT(4) /**< @brief Inhibit flag */
#define KBC_STAT_A2             BIT(3) /**< @brief A2 input line (not needed for LCOM) */
#define KBC_STAT_SYS            BIT(2) /**< @brief System flag (not needed for LCOM) */
#define KBC_STAT_IBF            BIT(1) /**< @brief Input buffer full */
#define KBC_STAT_OBF            BIT(0) /**< @brief Output buffer full */

/* Keyboard Controller Commands */

#define KBC_CMD_MOUSE_RESET     0xFF /**< @brief Reset mouse */
#define KBC_CMD_MOUSE_RESEND    0xFE /**< @brief Resend last command (If invalid byte) */
#define KBC_CMD_MOUSE_ERROR     0xFC /**< @brief Error */
#define KBC_CMD_MOUSE_ACK       0xFA /**< @brief Acknowledge */
#define KBC_CMD_DEFAULT         0xF6 /**< @brief Set default values */
#define KBC_CMD_DISABLE_DATA    0xF5 /**< @brief Disable data reporting */
#define KBC_CMD_ENABLE_DATA     0xF4 /**< @brief Enable data reporting */
#define KBC_CMD_SET_SAMPLE      0xF3 /**< @brief Set sample rate */
#define KBC_CMD_SET_REMOTE      0xF0 /**< @brief Set remote mode */
#define KBC_CMD_READ_DATA       0xEB /**< @brief Read data */
#define KBC_CMD_SET_STREAM      0xEA /**< @brief Set stream mode */
#define KBC_CMD_STATUS_REQUEST  0xE9 /**< @brief Status request */
#define KBC_CMD_SET_RESOLUTION  0xE8 /**< @brief Set resolution */
#define KBC_CMD_SET_SCALING21   0xE7 /**< @brief Set scaling 2:1 (Acceleration Mode) */
#define KBC_CMD_SET_SCALING11   0xE6 /**< @brief Set scaling 1:1 (Linear Mode) */
#define KBC_CMD_MOUSE_WRITE     0xD4 /**< @brief Write to mouse */
#define KBC_CMD_ENABLE          0xAE /**< @brief Enable KBC interface */
#define KBC_CMD_DISABLE         0xAD /**< @brief Disable KBC interface */
#define KBC_CMD_KBD_CHECK       0xAB /**< @brief Check keyboard interface */
#define KBC_CMD_CHECK           0xAA /**< @brief Check KBC interface */
#define KBC_CMD_MOUSE_CHECK     0xA9 /**< @brief Check mouse interface */
#define KBC_CMD_MOUSE_ENABLE    0xA8 /**< @brief Enable mouse interface */
#define KBC_CMD_MOUSE_DISABLE   0xA7 /**< @brief Disable mouse interface */
#define KBC_CMD_WRITE           0x60 /**< @brief Write command byte */
#define KBC_CMD_READ            0x20 /**< @brief Read command byte */

/* Keyboard Controller Command Byte */

#define KBC_CMD_MOUSE_INT_DIS   BIT(5) /**< @brief Disable mouse interface */
#define KBC_CMD_KBD_INT_DIS     BIT(4) /**< @brief Disable keyboard interface */
#define KBC_CMD_MOUSE_INT       BIT(1) /**< @brief Enable mouse interface */
#define KBC_CMD_KBD_INT         BIT(0) /**< @brief Keyboard interrupt */

/* Mouse Packets */

#define KBC_MOUSE_Y_OVF         BIT(7) /**< @brief Y overflow */
#define KBC_MOUSE_X_OVF         BIT(6) /**< @brief X overflow */
#define KBC_MOUSE_Y_SIGN        BIT(5) /**< @brief Y sign */
#define KBC_MOUSE_X_SIGN        BIT(4) /**< @brief X sign */
#define KBC_MOUSE_MB            BIT(2) /**< @brief Middle button */
#define KBC_MOUSE_RB            BIT(1) /**< @brief Right button */
#define KBC_MOUSE_LB            BIT(0) /**< @brief Left button */

#endif // _LCOM_I8042_H_
