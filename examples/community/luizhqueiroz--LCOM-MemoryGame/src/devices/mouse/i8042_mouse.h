/**
 * @file i8042_mouse
 * @brief Constants for programming the i8042 mouse
 */

#ifndef _LCOM_I8042_MOUSE_H_
#define _LCOM_I8042_MOUSE_H_

#include <lcom/lcf.h>

/** @defgroup i8042_Mouse i8042_Mouse
 * @{
 *
 * Constants for programming the i8042 mouse
 */

#define MOUSE_IRQ   12 /**< @brief Mouse IRQ line */

#define WAIT_MOUSE  20000 /**< @brief Tick delay for mouse */

#define MOUSE_ST_REG    0x64 /**< @brief Mouse status register */
#define MOUSE_CMD_REG   0x64 /**< @brief Register for write mouse commands */
#define MOUSE_OUT_BUF   0x60 /**< @brief Mouse output buffer register */
#define MOUSE_ARG_REG   0x60 /**< @brief Address to pass the arguments of a command */

#define MOUSE_ST_OBF    BIT(0) /**< @brief Bit indicates output buffer full */
#define MOUSE_ST_IBF    BIT(1) /**< @brief Bit indicates input buffer full */
#define MOUSE_ST_AUX    BIT(5) /**< @brief Bit indicates mouse data */

#define MOUSE_PAR_ERR   BIT(7) /**< @brief Bit indicates parity error */
#define MOUSE_TO_ERR    BIT(6) /**< @brief Bit indicates timeout error */

#define MOUSE_ACK   0xFA /**< @brief Acknolegment byte */
#define MOUSE_NACK  0xFE /**< @brief Bit indicates timeout error */

#define MOUSE_LB        BIT(0) /**< @brief Bit indicates left mouse button pressed */
#define MOUSE_RB        BIT(1) /**< @brief Bit indicates right mouse button pressed */
#define MOUSE_MB        BIT(2) /**< @brief Bit indicates middle mouse button pressed */
#define MOUSE_BIT_SYNC  BIT(3) /**< @brief Bit indicates syncronization in byte 1 */
#define MOUSE_X_OV      BIT(6) /**< @brief Bit indicates mouse x-displacement overflow */
#define MOUSE_Y_OV      BIT(7) /**< @brief Bit indicates mouse y-displacement overflow */
#define MOUSE_X_MSB     BIT(4) /**< @brief Bit indicates mouse MSB X displacement */
#define MOUSE_Y_MSB     BIT(5) /**< @brief Bit indicates mouse MSB Y displacement */

#define WRITE_B_TO_MOUSE_CMDB       0xD4 /**< @brief Command to write byte to a mouse */
#define ENABLE_STREAM_MODE_CMDB     0xEA /**< @brief Command to set stream mode */
#define ENABLE_DATA_REPORT_CMDB     0xF4 /**< @brief Command to enable data report */
#define DISABLE_DATA_REPORT_CMDB    0xF5 /**< @brief Command to disable data report */
#define ENABLE_MOUSE_CMDB           0xA8 /**< @brief Command to enable mouse */
#define DISABLE_MOUSE_CMDB          0xA7 /**< @brief Command to disable mouse */
#define CHCK_MOUSE_ITF_CMDB         0xA9 /**< @brief Command to check mouse interface */

/**@}*/

#endif
