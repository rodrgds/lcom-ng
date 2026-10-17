#ifndef LCOM_I8042_H
#define LCOM_I8042_H

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 Keyboard Controller (KBC).
 * Used for interfacing with the PS/2 keyboard and mouse.
 */

/** @name General Constants */
/**@{*/
#define DELAY_US 20000        /**< @brief Delay used for polling (microseconds) */
#define READ_ATTEMPT_LIMIT 30 /**< @brief Maximum read attempts before timeout */
/**@}*/

/** @name IRQ Lines */
/**@{*/
#define KBD_IRQ 1    /**< @brief Keyboard IRQ line */
#define MOUSE_IRQ 12 /**< @brief Mouse IRQ line */
/**@}*/

/** @name KBC I/O Ports */
/**@{*/
#define KBC_STATUS_REGISTER 0x64   /**< @brief Status register (read-only) */
#define KBC_COMMAND_REGISTER 0x64  /**< @brief Command register (write-only) */
#define KBC_OUTPUT_BUFFER 0x60     /**< @brief Output buffer (read data from KBC) */
#define KBC_ARGUMENT_REGISTER 0x60 /**< @brief Write arguments to KBC commands */
#define KBC_RETURN_REGISTER 0x60   /**< @brief Read return data from KBC */
/**@}*/

/** @name KBC Status Register Bits */
/**@{*/
#define KBC_PARITY_BIT BIT(7)           /**< @brief Parity error */
#define KBC_TIMEOUT_BIT BIT(6)          /**< @brief Timeout error */
#define KBC_AUX_BIT BIT(5)              /**< @brief 1 if data is from mouse */
#define KBC_KEYBOARD_INHIBIT_BIT BIT(4) /**< @brief Keyboard inhibited */
#define KBC_COMMAND_DATA_BIT BIT(3)     /**< @brief 1 = last write was a command */
#define KBC_SYSTEM_FLAG BIT(2)          /**< @brief System Flag (should be set after POST) */
#define KBC_INPUT_BUFFER_FULL BIT(1)    /**< @brief Input buffer full */
#define KBC_OUTPUT_BUFFER_FULL BIT(0)   /**< @brief Output buffer full */
/**@}*/

/** @name KBC Commands */
/**@{*/
#define KBC_READ_COMMAND_BYTE 0x20  /**< @brief Command to read command byte */
#define KBC_WRITE_COMMAND_BYTE 0x60 /**< @brief Command to write command byte */
/**@}*/

/** @name Keyboard Specific Constants */
/**@{*/
#define KBD_INTERRUPTS_ON BIT(0) /**< @brief Enable keyboard interrupts in command byte */
#define TWO_BYTE_SCANCODE 0xE0   /**< @brief Prefix for two-byte scan codes */
#define ESC_BREAK_CODE 0x81      /**< @brief Break code for ESC key */
#define BREAK BIT(7)             /**< @brief Break code indicator bit (1 = break) */
/**@}*/

/** @name Mouse Communication Commands */
/**@{*/
#define MOUSE_WRITE_COMMAND 0xD4          /**< @brief Write byte to mouse */
#define MOUSE_ENABLE_DATA_REPORTING 0xF4  /**< @brief Enable data reporting */
#define MOUSE_DISABLE_DATA_REPORTING 0xF5 /**< @brief Disable data reporting */
#define MOUSE_SET_DEFAULTS 0xF6           /**< @brief Reset mouse to default settings */
#define MOUSE_RESET 0xFF                  /**< @brief Reset mouse */
#define MOUSE_RESEND 0xFE                 /**< @brief Resend last packet */
#define MOUSE_ACK 0xFA                    /**< @brief Mouse Acknowledgment byte */
#define MOUSE_NACK 0xFE                   /**< @brief Mouse Not Acknowledged (retry) */
#define MOUSE_ERROR 0xFC                  /**< @brief Mouse Error (fatal) */
/**@}*/

/** @name Mouse Packet Bit Masks (1st Byte) */
/**@{*/
#define MOUSE_LB BIT(0)         /**< @brief Left button pressed */
#define MOUSE_RB BIT(1)         /**< @brief Right button pressed */
#define MOUSE_MB BIT(2)         /**< @brief Middle button pressed */
#define MOUSE_SYNC BIT(3)       /**< @brief Sync bit: always 1 in the first byte of a valid packet */
#define MOUSE_X_SIGN BIT(4)     /**< @brief X sign bit */
#define MOUSE_Y_SIGN BIT(5)     /**< @brief Y sign bit */
#define MOUSE_X_OVERFLOW BIT(6) /**< @brief X overflow */
#define MOUSE_Y_OVERFLOW BIT(7) /**< @brief Y overflow */
/**@}*/

/**@}*/ // End of i8042 group

#endif /* LCOM_I8042_H */
