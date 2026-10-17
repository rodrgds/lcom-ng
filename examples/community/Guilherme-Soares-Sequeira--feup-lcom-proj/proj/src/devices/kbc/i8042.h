#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#include "../../utils/utils.h"

/** @defgroup i8042 i8042
 *  @{
 *
 * Constants used for programming the KBC
 */

/* kbc registers */

#define KBC_IN_BUF_ARG 0x60 /**< @brief command byte arguments port */
#define KBC_IN_BUF_CMD 0x64 /**< @brief command byte port */
#define KBC_OUT_BUF 0x60    /**< @brief output buffer port */
#define KBC_OUT_BUF_ST 0x64 /**< @brief output buffer status port */

/* kbc status register */

#define KBC_ST_PARITY BIT(7)  /**< @brief status register parity error mask */
#define KBC_ST_TIMEOUT BIT(6) /**< @brief status register timeout error mask */
#define KBC_ST_AUX BIT(5)     /**< @brief status register aux mask */
#define KBC_ST_INH BIT(4)     /**< @brief status register inh mask */
#define KBC_ST_IBF BIT(1)     /**< @brief status register input buffer full mask */
#define KBC_ST_OBF BIT(0)     /**< @brief status register output buffer full mask */

/* kbc command byte structure */

#define KBC_INT BIT(0)  /**< @brief command byte enable interrupt on OBF from keyboard mask */
#define KBC_INT2 BIT(1) /**< @brief command byte enable interrupt on OBF from mouse mask */
#define KBC_DIS BIT(4)  /**< @brief command byte disable keyboard interface mask */
#define KBC_DIS2 BIT(5) /**< @brief command byte disable mouse interface mask */

/* kbc command bytes */

#define KBC_RD_CMD 0x20                /**< @brief read command byte */
#define KBC_WR_CMD 0x60                /**< @brief write command byte */
#define KBC_DISABLE_MOUSE 0xA7         /**< @brief disable mouse command byte */
#define KBC_ENABLE_MOUSE 0xA8          /**< @brief enable mouse command byte */
#define KBC_CHECK_MOUSE_INTERFACE 0xA9 /**< @brief check mouse interface command byte */
#define KBC_WR_BYTE_TO_MOUSE 0xD4      /**< @brief write byte to mouse command byte */

/* kbc keyboard interrupt line */

#define KBC_KEYBOARD_IRQ 1 /**< @brief KBC keyboard IRQ line */
#define KBC_MOUSE_IRQ 12   /**< @brief KBC PS/2 mouse IRQ line */

/* kbc scancode size indicators */

#define KBC_SCANCODE_DOUBLE_BYTE 0xE0 /**< @brief two-byte long scancode indicator */
#define KBC_SCANCODE_TRIPLE_BYTE 0xE1 /**< @brief three-byte long scancode indicator */

/* kbc scancode types */

#define KBC_SCANCODE_BREAK 0 /**< @brief indicates the scancode is a break code*/
#define KBC_SCANCODE_MAKE 1  /**< @brief indicates the scancode is a make code */

/* kbc ESC key scancodes */

#define ESC_KEY_MAKE 0x01  /**< @brief ESC key make code */
#define ESC_KEY_BREAK 0x81 /**< @brief ESC key break code */

/**@}*/

#endif
