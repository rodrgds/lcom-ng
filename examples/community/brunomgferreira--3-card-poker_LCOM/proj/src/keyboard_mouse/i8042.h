#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/**
 * @defgroup i8042 i8042
 * @{
 *
 * Constants and macros related to the i8042 PS/2 controller
 */

#define KBD_IRQ 1        /**< @brief Keyboard IRQ line */
#define KBD_HOOK_BIT 1   /**< @brief Bit used for hooking keyboard interrupts */
#define MOUSE_IRQ 12     /**< @brief Mouse IRQ line */
#define MOUSE_HOOK_BIT 2 /**< @brief Bit used for hooking mouse interrupts */

/* I/O port addresses */

#define KBC_STAT_REG 0x64 /**< @brief Status Register Port */
#define KBC_CTRL_REG 0x64 /**< @brief Control Register Port */
#define KBC_OUT_BUF 0x60  /**< @brief Output Buffer Port */
#define KBC_IN_BUF 0x60   /**< @brief Input Buffer Port */

/* KBC control */

#define PARITY_ERR BIT(7)  /**< @brief Parity Error Bit */
#define TIMEOUT_ERR BIT(6) /**< @brief Timeout Error Bit */
#define AUX BIT(5)          /**< @brief Auxiliary Device Bit */
#define INH BIT(4)          /**< @brief Inhibit Flag */
#define A2 BIT(3)           /**< @brief A2 Input Line */
#define SYS BIT(2)          /**< @brief System Flag */
#define IBF BIT(1)          /**< @brief Input Buffer Full */
#define OBF BIT(0)          /**< @brief Output Buffer Full */

/* KBC commands */

#define READ_COMMAND 0x20                  /**< @brief Read Command Byte */
#define WRITE_COMMAND 0x60                 /**< @brief Write Command Byte */
#define CHECK_KBC 0xAA                     /**< @brief Check KBC */
#define CHECK_KEYBOARD_INTERFACE 0xAB     /**< @brief Check Keyboard Interface */
#define DISABLE_KBD_INTERFACE 0xAD        /**< @brief Disable Keyboard Interface */
#define ENABLE_KBD_INTERFACE 0xAE         /**< @brief Enable Keyboard Interface */

/* KBC command byte */

#define DIS2 BIT(5) /**< @brief Disable Mouse Interface */
#define DIS BIT(4)  /**< @brief Disable Keyboard Interface */
#define INT2 BIT(1) /**< @brief Enable interrupt on OBF, from mouse */
#define INT BIT(0)  /**< @brief Enable interrupt on OBF, from keyboard */

/* Mouse Command Byte */

#define MOUSE_YOVFL BIT(7) /**< @brief Y Overflow */
#define MOUSE_XOVFL BIT(6) /**< @brief X Overflow */
#define MOUSE_MSBY BIT(5)  /**< @brief MSB Y Delta */
#define MOUSE_MSBX BIT(4)  /**< @brief MSB X Delta */
#define MOUSE_MB BIT(2)    /**< @brief Middle Button */
#define MOUSE_RB BIT(1)    /**< @brief Right Button */
#define MOUSE_LB BIT(0)    /**< @brief Left Button */

/* PS/2 Mouse Commands */

#define WRITE_BYTE_TO_MOUSE 0xD4    /**< @brief Write Byte To Mouse */
#define ENABLE_DATA_REPORTING 0xF4   /**< @brief Enable Data Reporting */
#define DISABLE_DATA_REPORTING 0xF5  /**< @brief Disable Data Reporting */

/* Mouse Returns */

#define ACK 0xFA  /**< @brief Acknowledge */
#define NACK 0xFE /**< @brief Not Acknowledge */
#define ERROR 0xFC /**< @brief Error */

/* Useful constants */

#define DELAY 2000 /**< @brief 20ms delay for KBD commands */
#define TRIES 10   /**< @brief Number of tries to read/write */

#define ESC_BREAK 0x81 /**< @brief Break code for the Escape key */
#define TWO_BYTES 0xE0 /**< @brief Indicator for a two-byte scancode */

/**@}*/

#endif /* _LCOM_I8042_H_ */
