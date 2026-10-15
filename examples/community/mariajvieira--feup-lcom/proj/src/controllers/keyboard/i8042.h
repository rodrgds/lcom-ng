#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{
 * Constants for programming the i8042 Keyboard Controller.
 */

#define KBD_IRQ           1    /**< @brief Keyboard IRQ line */

/* I/O port addresses */
#define STAT_REG          0x64 /**< @brief Status register */
#define KBC_CMD_REG       0x64 /**< @brief Command register */
#define IN_BUF            0x60 /**< @brief Input buffer */
#define OUT_BUF           0x60 /**< @brief Output buffer */

/* Status register */
#define OBF               BIT(0) /**< @brief Output buffer full */
#define IBF               BIT(1) /**< @brief Input buffer full */
#define SYS               BIT(2) /**< @brief System flag */
#define A2                BIT(3) /**< @brief A2 input line */
#define INH               BIT(4) /**< @brief Inhibit flag */
#define MOUSE_DATA        BIT(5) /**< @brief Mouse data */
#define TO_ERR            BIT(6) /**< @brief Timeout error */
#define PAR_ERR           BIT(7) /**< @brief Parity error */

/* Scan codes */
#define ESC_BREAK_CODE    0x81
#define ENTER_KEY         0x1C
#define SPACE_KEY         0x39
#define W_KEY             0x11
#define S_KEY             0x1F
#define A_KEY             0x1E
#define D_KEY             0x20
#define ARROW_UP          0x48
#define ARROW_DOWN        0x50
#define ARROW_LEFT        0x4B
#define ARROW_RIGHT       0x4D

/** @} end of i8042 */

#endif /* _LCOM_I8042_H */
