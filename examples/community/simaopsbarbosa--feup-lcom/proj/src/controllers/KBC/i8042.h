/**
 * @file i8042.h
 * @brief Constants and macros for the i8042.
 *
 * Defines IRQ lines, port addresses, command bytes, key codes, and mouse packet bits for the i8042 controller.
 */

#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 Keyboard and Mouse Controller.
 */

#define KEYBOARD_IRQ 1 /**< @brief Keyboard IRQ line */
#define MOUSE_IRQ 12   /**< @brief Mouse IRQ line */
#define WAIT_KBC 20000 /**< @brief Wait time for KBC operations (in microseconds) */
#define ATTEMPTS 10    /**< @brief Number of attempts for KBC operations */

/* KBC */
#define KBC_IN_CMD 0x64   /**< @brief KBC input and status register port */
#define KBC_OUT_CMD 0x60  /**< @brief KBC output and write command port */
#define KBC_READ_CMD 0x20 /**< @brief KBC read command byte */

/* keyboard */
#define MAKE_CODE BIT(7)  /**< @brief Bit mask for make/break code detection */
#define TWO_BYTES 0xE0    /**< @brief Prefix for two-byte scancodes */
#define ENABLE_INT BIT(0) /**< @brief Bit mask to enable keyboard interrupts in command byte */

#define ESC_KEY 0x81      /**< @brief Scancode for ESC key release */
#define UP_ARROW 0x48     /**< @brief Scancode for up arrow key */
#define DOWN_ARROW 0x50   /**< @brief Scancode for down arrow key */
#define ENTER_KEY 0x1C    /**< @brief Scancode for enter key */
#define SPACE_KEY 0x39    /**< @brief Scancode for space key */

/* mouse */
#define MOUSE_LB BIT(0)              /**< @brief Mouse left button bit mask */
#define MOUSE_RB BIT(1)              /**< @brief Mouse right button bit mask */
#define MOUSE_MB BIT(2)              /**< @brief Mouse middle button bit mask */
#define IS_FIRST_BYTE BIT(3)         /**< @brief Mouse packet first byte flag */
#define MSB_X_DELTA BIT(4)           /**< @brief Mouse X delta sign bit */
#define MSB_Y_DELTA BIT(5)           /**< @brief Mouse Y delta sign bit */
#define MOUSE_X_OVERFLOW BIT(6)      /**< @brief Mouse X overflow bit */
#define MOUSE_Y_OVERFLOW BIT(7)      /**< @brief Mouse Y overflow bit */
#define MOUSE_WRITE_BYTE 0xD4        /**< @brief Command to write a byte to the mouse */
#define ACKNOWLEDGMENT 0xFA          /**< @brief Mouse acknowledgment byte */
#define MOUSE_DISABLE_DATA_REPORT 0xF5 /**< @brief Command to disable mouse data reporting */
#define MOUSE_ENABLE_DATA_REPORT 0xF4  /**< @brief Command to enable mouse data reporting */

/**@}*/

#endif /* _LCOM_I8042_H_ */
