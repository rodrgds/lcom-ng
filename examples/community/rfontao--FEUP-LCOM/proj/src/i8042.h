#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>



/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 KBC.
 */

/*Keyboard Macros*/

#define KBC_IRQ         1               /**< @brief KBC IRQ line */

#define OUT_BUF         0x60            /**< @brief Address of the outbuffer */
#define STAT_REG        0x64            /**< @brief Address of the status register */
#define KBC_ARG_REG     0x60            /**< @brief KBC commands arguments */
#define KBC_CMD_REG     0x64            /**< @brief KBC issue command register*/
#define PAR_ERR         BIT(7)          /**< @brief Parity error */
#define TO_ERR          BIT(6)          /**< @brief Timeout error */
#define AUX             BIT(5)          /**< @brief Mouse data */
#define IBF             BIT(1)          /**< @brief Input buffer full */
#define OBF             BIT(0)          /**< @brief Output buffer full */
#define READ_CMD        0x20            /**< @brief Command to read command byte */
#define WRITE_CMD       0x60            /**< @brief Command to write command byte */

#define MAX_TRIES       4               /**< @brief Number of tries of the loops */

#define ESC_BREAKCODE   0x81            /**< @brief Breakcode of the ESC key */

/*Mouse Macros */

#define MOUSE_IRQ               12              /** @brief Mouse IRQ line */
#define MOUSE_LB_PRESSED        BIT(0)          /** @brief Mouse LB pressed */
#define MOUSE_RB_PRESSED        BIT(1)          /** @brief Mouse RB pressed */
#define MOUSE_MB_PRESSED        BIT(2)          /** @brief Mouse MB pressed */
#define MOUSE_X_DELTA_SIGN      BIT(4)          /** @brief Sign of the X delta */
#define MOUSE_Y_DELTA_SIGN      BIT(5)          /** @brief Sign of the Y delta */
#define MOUSE_X_OVF             BIT(6)          /** @brief X overflow */
#define MOUSE_Y_OVF             BIT(7)          /** @brief Y overflow */
#define MOUSE_CMD               0xD4            /** @brief Mouse command */
#define MOUSE_ENABLE_CMD        0xF4            /** @brief Command to enable the mouse */
#define MOUSE_DISABLE_CMD       0xF5            /** @brief Command to disable the mouse */
#define MOUSE_ACK_ERROR         0xFC            /** @brief ACK Error */
#define MOUSE_NACK              0xFE            /** @brief NACK Error */
#define MOUSE_STREAM            0xEA            /** @brief Stream Mode*/
#define MOUSE_READ              0xEB            /** @brief */

/**@}*/

#endif /* _LCOM_I8042_H */

