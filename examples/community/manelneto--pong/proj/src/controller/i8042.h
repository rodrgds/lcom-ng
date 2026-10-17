#ifndef _I8042_H_
#define _I8042_H_

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 KBC
 */

#define KEYBOARD_IRQ 1      /**< @brief Keyboard IRQ line */
#define MOUSE_IRQ 12        /**< @brief Mouse IRQ line */
#define KBD_DELAY 20000     /**< @brief To be used with tickdelay(micros_to_ticks(KBD_DELAY)) */
#define KBC_MAX_ATTEMPTS 20 /**< @brief Maximum number of attempts */

/* I/O port addresses */

#define KBC_STATUS_REG 0x64 /**< @brief KBC status register */
#define KBC_CMD_REG 0x64    /**< @brief KBC command register */
#define KBC_ARG_REG 0x60    /**< @brief KBC arguments register */
#define KBC_OUT_BUF 0x60    /**< @brief KBC output buffer */

/* KBC commands */

#define KBC_READ_CMD 0x20           /**< @brief KBC read command byte */
#define KBC_WRITE_CMD 0x60          /**< @brief KBC write command byte */
#define KBC_CHECK_CMD 0xAA          /**< @brief KBC self-test (if OK, returns 0x55; if error, returns 0xFC) */
#define KBC_CHECK_KBD_CMD 0xAB      /**< @brief KBC check keyboard interface */
#define KBC_DISABLE_KBD_CMD 0xAD    /**< @brief KBC disable keyboard interface */
#define KBC_ENABLE_KBD_CMD 0xAE     /**< @brief KBC enable keyboard interface */
#define KBC_DISABLE_MOUSE_CMD 0xA7  /**< @brief KBC disable mouse interface */
#define KBC_ENABLE_MOUSE_CMD 0xA8   /**< @brief KBC enable mouse interface */
#define KBC_CHECK_MOUSE_CMD 0xA9    /**< @brief KBC check mouse interface */
#define KBC_WRITE_TO_MOUSE_CMD 0xD4 /**< @brief KBC write byte (argument) to mouse */

/* KBC arguments */

#define KBC_RESET 0xFF                  /**< @brief KBC mouse reset */
#define KBC_RESEND 0xFE                 /**< @brief KBC for serial communications errors */
#define KBC_SET_DEFAULTS 0xF6           /**< @brief KBC set default values */
#define KBC_DISABLE_DATA_REPORTING 0xF5 /**< @brief KBC disable data reporting (in stream mode, should be sent before any other command) */
#define KBC_ENABLE_DATA_REPORTING 0xF4  /**< @brief KBC enable data reporting (in stream mode only) */
#define KBC_SET_SAMPLE_RATE 0xF3        /**< @brief KBC set state sampling rate */
#define KBC_SET_REMOTE_MODE 0xF0        /**< @brief KBC send data on request only */
#define KBC_READ_DATA 0xEB              /**< @brief KBC send data packet request */
#define KBC_SET_STREAM_MODE 0xEA        /**< @brief KBC send data on events */
#define KBC_STATUS_REQUEST 0xE9         /**< @brief KBC get mouse configuration (3 bytes) */
#define KBC_SET_RESOLUTION 0xE8         /**< @brief KBC set resolution */
#define KBC_SET_SCALING_2 0xE7          /**< @brief KBC acceleration mode */
#define KBC_SET_SCALING_1 0xE6          /**< @brief KBC linear mode */

/* KBC command byte */

#define KBC_INT BIT(0)  /**< @brief KBC enable interrupt on OBF, from keyboard */
#define KBC_INT2 BIT(1) /**< @brief KBC enable interrupt on OBF, from mouse */
#define KBC_DIS BIT(4)  /**< @brief KBC disable keyboard interface */
#define KBC_DIS2 BIT(5) /**< @brief KBC disable mouse interface */

/* KBC acknowledgment byte */

#define KBC_ACK 0xFA   /**< @brief KBC everything OK */
#define KBC_NACK 0xFE  /**< @brief KBC if invalid byte (may be because of a serial communication error) */
#define KBC_ERROR 0xFC /**< @brief KBC second consecutive invalid byte */

/** Status Register Format */

#define KBC_OBF BIT(0)           /**< @brief KBC output buffer full */
#define KBC_IBF BIT(1)           /**< @brief KBC input buffer full */
#define KBC_SYS BIT(2)           /**< @brief KBC system flag: 0 if system in power-on reset, 1 if system already initialized */
#define KBC_A2 BIT(3)            /**< @brief KBC A2 input line: 0 if data byte, 1 if command byte */
#define KBC_INH BIT(4)           /**< @brief KBC inhibit flag */
#define KBC_AUX BIT(5)           /**< @brief KBC mouse data */
#define KBC_TIMEOUT_ERROR BIT(6) /**< @brief KBC timeout error - invalid data */
#define KBC_PARITY_ERROR BIT(7)  /**< @brief KBC parity error - invalid data */

/* Other - Keyboard */

#define KBD_BREAKCODE BIT(7)               /**< @brief Keyboard breakcode bit */
#define KBD_FIRST_OF_TWO_BYTES 0xE0        /**< @brief Keyboard first byte of two bytes scancode */
#define KBD_ESC_BREAKCODE 0x81             /**< @brief Keyboard ESC breakcode */

#define KBD_ARROW_UP_MAKECODE_LSB 0x48     /**< @brief Keyboard ARROW UP makecode LSB */
#define KBD_ARROW_DOWN_MAKECODE_LSB 0x50   /**< @brief Keyboard ARROW DOWN makecode LSB */
#define KBD_ARROW_LEFT_MAKECODE_LSB 0x4B   /**< @brief Keyboard ARROW LEFT makecode LSB */
#define KBD_ARROW_RIGHT_MAKECODE_LSB 0x4D  /**< @brief Keyboard ARROW RIGHT makecode LSB */
#define KBD_ARROW_UP_BREAKCODE_LSB 0xC8    /**< @brief Keyboard ARROW UP breakcode LSB */
#define KBD_ARROW_DOWN_BREAKCODE_LSB 0xD0  /**< @brief Keyboard ARROW DOWN breakcode LSB */
#define KBD_ARROW_LEFT_BREAKCODE_LSB 0xCB  /**< @brief Keyboard ARROW LEFT breakcode LSB */
#define KBD_ARROW_RIGHT_BREAKCODE_LSB 0xCD /**< @brief Keyboard ARROW RIGHT breakcode LSB */

#define KBD_W_MAKECODE 0x11                /**< @brief Keyboard W makecode */
#define KBD_A_MAKECODE 0x1E                /**< @brief Keyboard A makecode */
#define KBD_S_MAKECODE 0x1F                /**< @brief Keyboard S makecode */
#define KBD_D_MAKECODE 0x20                /**< @brief Keyboard D makecode */
#define KBD_W_BREAKCODE 0x91               /**< @brief Keyboard W breakcode */
#define KBD_A_BREAKCODE 0x9E               /**< @brief Keyboard A breakcode */
#define KBD_S_BREAKCODE 0x9F               /**< @brief Keyboard S breakcode */
#define KBD_D_BREAKCODE 0xA0               /**< @brief Keyboard D breakcode */

#define KBD_ENTER_MAKECODE 0x1C            /**< @brief Keyboard ENTER makecode */
#define KBD_SPACE_MAKECODE 0x9C            /**< @brief Keyboard SPACE makecode */
#define KBD_ENTER_BREAKCODE 0x39           /**< @brief Keyboard ENTER breakcode */
#define KBD_SPACE_BREAKCODE 0x9B           /**< @brief Keyboard SPACE breakcode */

#define KBD_1_MAKECODE 0x02                /**< @brief Keyboard 1 makecode */
#define KBD_2_MAKECODE 0x03                /**< @brief Keyboard 2 makecode */
#define KBD_3_MAKECODE 0x04                /**< @brief Keyboard 3 makecode */
#define KBD_1_BREAKCODE 0x82               /**< @brief Keyboard 1 breakcode */
#define KBD_2_BREAKCODE 0x83               /**< @brief Keyboard 2 breakcode */
#define KBD_3_BREAKCODE 0x84               /**< @brief Keyboard 3 breakcode */

#define KBD_E_MAKECODE 0x12                /**< @brief Keyboard E makecode */
#define KBD_M_MAKECODE 0x32                /**< @brief Keyboard M makecode */
#define KBD_H_MAKECODE 0x23                /**< @brief Keyboard H makecode */
#define KBD_P_MAKECODE 0x19                /**< @brief Keyboard P makecode */
#define KBD_Q_MAKECODE 0x10                /**< @brief Keyboard Q makecode */
#define KBD_E_BREAKCODE 0x92               /**< @brief Keyboard E breakcode */
#define KBD_M_BREAKCODE 0xB2               /**< @brief Keyboard M breakcode */
#define KBD_H_BREAKCODE 0xA3               /**< @brief Keyboard H breakcode */
#define KBD_P_BREAKCODE 0x99               /**< @brief Keyboard P makecode */
#define KBD_Q_BREAKCODE 0x90               /**< @brief Keyboard Q makecode */

/* Other - Mouse */

#define MOUSE_LB BIT(0)          /**< @brief Mouse Left Button */
#define MOUSE_RB BIT(1)          /**< @brief Mouse Right Button */
#define MOUSE_MB BIT(2)          /**< @brief Mouse Middle Button */
#define MOUSE_BYTE1 BIT(3)       /**< @brief 1 if byte 1 of mouse packet */
#define MOUSE_MSB_X_DELTA BIT(4) /**< @brief Mouse MSB X delta */
#define MOUSE_MSB_Y_DELTA BIT(5) /**< @brief Mouse MSB Y delta */
#define MOUSE_X_OVFL BIT(6)      /**< @brief Mouse X overflow */
#define MOUSE_Y_OVFL BIT(7)      /**< @brief Mouse Y overflow */

#define MOUSE_SET_MSB 0xFF00     /**< @brief To set the most significant byte as 11111111 (complement-to-two) */

/**@}*/

#endif /* _I8042_H_ */
