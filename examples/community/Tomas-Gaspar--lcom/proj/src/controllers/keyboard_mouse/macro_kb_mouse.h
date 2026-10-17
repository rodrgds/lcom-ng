#ifndef _MACRO_KB_MOUSE_H_
#define _MACRO_KB_MOUSE_H_

#include <lcom/lcf.h>


/**
 * @ingroup Device_Drivers
 * 
 * @defgroup kb_mouse Keyboard / Mouse
 * 
 * @brief Device driver module for the keyboard and the mouse.
 * 
 * This module provides all the necessary functionality to interact with the keyboard controller,
 * configuring the keyboard and the mouse, and getting data from them.
 * 
 * @{
*/

/* IRQ */
#define TIMER0_IRQ 0                        /**< @brief Timer 0 IRQ line*/
#define KB_IRQ 1                            /**< @brief Keyboard IRQ line*/
#define MS_IRQ 12                           /**< @brief Mouse IRQ line*/

/* Keybord Controller */
#define KBC_IN_BUF 0x60                     /**< @brief Keyboard Controller Input Buffer register. */
#define KBC_OUT_BUF 0x60                    /**< @brief Keyboard Controller Output Buffer register. */
#define KBC_ST_REG 0x64                     /**< @brief Keyboard Controller Status Register. */
#define KBC_CMD_REG 0x64                    /**< @brief Keyboard Controller Command Register. */

/* KBC commands */
#define READ_CMD_BYTE 0x20                  /**< @brief Command to read the command byte from the Keyboard Controller. */
#define WRITE_CMD_BYTE 0x60                 /**< @brief Command to write the command byte to the Keyboard Controller. */

/* Status Register */
#define KBC_PAR_TO_ERROR BIT(7) | BIT(6)   /**< @brief Keyboard Controller Status Register - Parity or Timeout Error. */
#define KBC_IBF BIT(1)                     /**< @brief Keyboard Controller Status Register - Input Buffer Full. */
#define KBC_OBF BIT(0)                     /**< @brief Keyboard Controller Status Register - Output Buffer Full. */
#define KBC_AUX BIT(5)                     /**< @brief Keyboard Controller Status Register - Mouse Data. */

/* KBC Scancodes */
#define ESC_MAKE 0x01                       /**< @brief Keyboard Scancode - Make Code for the Escape Key. */
#define ESC_BREAK 0x81                      /**< @brief Keyboard Scancode - Break Code for the Escape Key. */
#define BACKSPACE_MAKE 0x0E                 /**< @brief Keyboard Scancode - Make Code for the Backspace Key. */
#define ENTER_MAKE 0x1C                     /**< @brief Keyboard Scancode - Make Code for the Enter Key. */

#define TWO_BYTE_SC 0xE0                    /**< @brief Two-Byte Scancode prefix. */

#define DELAY_US 20000                      /**< @brief Delay in microseconds for Keyboard Controller operations. */

/* Mouse Controller */
#define MS_ACK 0xFA                         /**< @brief Mouse Acknowledgment. */
#define MS_NACK 0xFE                        /**< @brief Mouse Not Acknowledged. */
#define MS_ERROR 0xFC                       /**< @brief Mouse Error. */

#define MS_LB BIT(0)                        /**< @brief Mouse Left Button. */
#define MS_RB BIT(1)                        /**< @brief Mouse Right Button. */
#define MS_MB BIT(2)                        /**< @brief Mouse Middle Button. */
#define MS_X_MSB BIT(4)                     /**< @brief Mouse X Most Significant Bit (sign bit). */
#define MS_Y_MSB BIT(5)                     /**< @brief Mouse Y Most Significant Bit (sign bit). */
#define MS_X_OVF BIT(6)                     /**< @brief Mouse X Overflow. */
#define MS_Y_OVF BIT(7)                     /**< @brief Mouse Y Overflow. */

#define MS_READ_CMD_BYTE 0x20               /**< @brief Command to read the mouse command byte from the keyboard controller. */  
#define MS_WRITE_CMD_BYTE 0xD4              /**< @brief Command to write the mouse command byte to the keyboard controller. */
#define MS_ENABLE_REP 0xF4                  /**< @brief Command to enable mouse data reporting. */
#define MS_DISABLE_REP 0xF5                 /**< @brief Command to disable mouse data reporting. */

#define MS_FIRST_BYTE BIT(3)                /**< @brief The first mouse packet always has bit 3 set to 1*/

/** }@ */

#endif
