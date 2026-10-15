#ifndef _PROJ_I8042_H_
#define _PROJ_I8042_H_

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 Keyboard Controller.
 */

#define KBD_IRQ 1          /**< @brief Keyboard IRQ line */
#define MOUSE_IRQ 12       /**< @brief Mouse IRQ line */
#define KBC_MAX_ATTEMPTS 5 /**< @brief Maximum amount of attempts to read KBC data */
#define DELAY_US 20000     /**< @brief KBC response time */

/* I/O port addresses */

#define KBC_OUT 0X60 /**< @brief KBC output buffer port */
#define KBC_IN 0X64  /**< @brief KBC input buffer port */
#define KBC_ST 0X64  /**< @brief KBC status register port */
#define KBC_ARG 0x60 /**< @brief KBC arguments input buffer */

/* Scancode-relaed */

#define MAKE_CODE BIT(7) /**< @brief Make code bit mask */
#define CODE_HEADER 0xE0 /**< @brief Byte that indicates if the scancode has 2 bytes */
#define BREAK_ESC 0x81   /**< @brief ESC key breakcode */

/* Status byte format */

#define KBC_PARITY_ERROR BIT(7)  /**< @brief Parity error bit mask in status register */
#define KBC_TIMEOUT_ERROR BIT(6) /**< @brief Timeout error bit mask in status register */
#define KBC_AUX BIT(5)           /**< @brief Mouse data */
#define KBC_INH BIT(4)           /**< @brief Inhibit flag */
#define KBC_FULL_IBF BIT(1)      /**< @brief Input buffer full bit mask in status register */
#define KBC_FULL_OBF BIT(0)      /**< @brief Output buffer full bit mask in status register */

/* Keyboard-Related KBC Commands for PC-AT/PS2 */

#define KBC_READ_CMD 0x20  /**< @brief Read Command Byte */
#define KBC_WRITE_CMD 0x60 /**< @brief Write Command Byte */
#define KBC_CHK 0xAA       /**< @brief KBC command to self-check */
#define KBC_CHK_KBD_I 0xAB /**< @brief KBC command to check keyboard interface */
#define KBC_DIS_KBD_I 0XAD /**< @brief KBC command to disable KBD interface */
#define KBC_EN_KBD_I 0XAE  /**< @brief KBC command to enable KBD interface */

/* Mouse-Related KBC Commands */

#define MOUSE_READ_CMD 0x20   /**< @brief Read Command Byte */
#define MOUSE_WRITE_CMD 0x60  /**< @brief Write Command Byte */
#define MOUSE_DIS 0xA7        /**< @brief Disable Mouse */
#define MOUSE_EN 0xA8         /**< @brief Enable Mouse */
#define MOUSE_CHECK 0xA9      /**< @brief Check Mouse Interface */
#define MOUSE_WRITE_BYTE 0xD4 /**< @brief Write Byte to Mouse */

/* Arguments of Write Byte to Mouse Command */

#define MOUSE_RESET 0xFF            /**< @brief Mouse reset */
#define MOUSE_RESEND 0xFE           /**< @brief For serial communications errors */
#define MOUSE_SET_DEFAULTS 0xF6     /**< @brief Set default values */
#define MOUSE_DIS_DATA_REPORTS 0xF5 /**< @brief Disable data reporting (in stream mode, should be sent before any other command) */

#define MOUSE_EN_DATA_REPORTS 0xF4 /**< @brief Enable data reporting (in stream mode only) */
#define MOUSE_SET_SAMPLE_RATE 0xF3 /**< @brief Set state sampling rate */
#define MOUSE_SET_REMOTE_MODE 0xF0 /**< @brief Set remote mode (send data on request only) */
#define MOUSE_READ_DATA 0xEB       /**< @brief Send data packet request */
#define MOUSE_SET_STREAM_MODE 0xEA /**< @brief Set stream mode (send data on events) */
#define MOUSE_ST_REQ 0xE9          /**< @brief Get mouse configuration (3 bytes) */
#define MOUSE_SET_RES 0xE8         /**< @brief Set resolution */
#define MOUSE_SET_SCALING_ACC 0xE7 /**< @brief Set scaling 2:1 (acceleration mode) */
#define MOUSE_SET_SCALING_LIN 0xE6 /**< @brief Set scaling 1:1 (linear mode) */

/* Mouse acknowledgment bytes */

#define MOUSE_ACK 0xFA  /**< @brief Everything OK */
#define MOUSE_NACK 0xFE /**< @brief Invalid byte */
#define MOUSE_ERR 0xFC  /**< @brief Second consecutive invalid byte */

/* KBC Command Byte */

#define KBC_DIS2 BIT(5) /**< @brief Disable mouse interface */
#define KBC_DIS BIT(4)  /**< @brief Disable keyboard interface */
#define KBC_INT2 BIT(1) /**< @brief Enable interrupt on OBF, from mouse */
#define KBC_INT BIT(0)  /**< @brief Enable interrupt on OBF, from keyboard */

/* Mouse Data Packet bytes */

#define MOUSE_LB BIT(0)    /**< @brief Left button pressed */
#define MOUSE_RB BIT(1)    /**< @brief Right button pressed */
#define MOUSE_MB BIT(2)    /**< @brief Middle button pressed */
#define MOUSE_XSIGN BIT(4) /**< @brief Relative displacement MSBit X axis */
#define MOUSE_YSIGN BIT(5) /**< @brief Relative displacement MSBit Y axis */
#define MOUSE_XOV BIT(6)   /**< @brief Flag that the mouse X displacement is too large to \
  be represented as a 9-bit 2-complement integer */
#define MOUSE_YOV BIT(7)   /**< @brief Flag that the mouseY displacement is too large to \
  be represented as a 9-bit 2-complement integer */

/**@}*/

#endif /* _PROJ_I8042_H_ */
