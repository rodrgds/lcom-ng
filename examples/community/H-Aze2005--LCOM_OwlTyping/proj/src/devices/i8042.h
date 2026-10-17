/**
 * @file i8042.h
 * @brief Constants for the i8042 keyboard and mouse controller.
 *
 * Defines IRQs, I/O port addresses, status bits, and command bytes
 * for interfacing with the i8042 controller used by keyboard and mouse devices.
 */


#ifndef _I8042_H_
#define _I8042_H_

/** @name Keyboard Controller Constants */
/**@{ */
#define KBD_IRQ 1                    /**< Keyboard IRQ line */
#define KBC_STATUS 0x64              /**< KBC status register port */
#define KBC_OBF BIT(0)               /**< Output buffer full bit */
#define KBC_OUT_BUFFER 0x60          /**< KBC output buffer port */
#define KBC_PAR_ERR BIT(7)           /**< Parity error bit */
#define KBC_TO_ERR BIT(6)            /**< Timeout error bit */
/**@} */


/** @name Mouse Controller Constants */
/**@{ */
#define MOUSE_IRQ 12                     /**< Mouse IRQ line */
#define MOUSE_WRITE_BYTE 0xD4            /**< Command to write byte to mouse */
#define MOUSE_ENABLE_DATA_REPORTING 0xF4 /**< Enable mouse data reporting */
#define MOUSE_DISABLE_DATA_REPORTING 0xF5/**< Disable mouse data reporting */
#define MOUSE_ACK 0xFA                   /**< Mouse acknowledge byte */
/**@} */

#endif
