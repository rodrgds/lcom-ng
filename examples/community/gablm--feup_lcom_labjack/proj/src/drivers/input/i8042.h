#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @ingroup input
 * @{
 *
 * @brief Constants for the i8042 keyboard and mouse controller.
 */

#define IRQ_KEYBOARD 1 /**< @brief IRQ line for the keyboard */
#define IRQ_MOUSE 12 /**< @brief IRQ line for the mouse */

#define DELAY_US 20000 /**< @brief Delay for polling the controller, in microseconds */

#define MAKECODE_BIT BIT(7) /**< @brief Bit mask to identify make codes */
#define TWOBITS 0xE0 /**< @brief Byte indicating two-byte scan code */

#define STAT_REG 0x64 /**< @brief Status register port */
#define OUT_BUF 0x60 /**< @brief Output buffer port */

#define READ_CMD    0x20 /**< @brief Command to read the command byte */
#define WRITE_CMD   0x60  /**< @brief Command to write the command byte */
#define WRITE_CMD_MOUSE 0xD4 /**< @brief Command to write to the mouse */

#define PARITY_BIT BIT(7) /**< @brief Parity error bit in the status register */
#define TIMEOUT_BIT BIT(6) /**< @brief Timeout error bit in the status register */
#define MOUSE_BIT BIT(5) /**< @brief Mouse data bit in the status register */

#define FULL_INPUT BIT(1) /**< @brief Full input buffer bit in the status register */
#define FULL_OUTPUT BIT(0) /**< @brief Full output buffer bit in the status register */

#define ENABLE_INT BIT(0) /**< @brief Enable interrupts bit in the command byte */

#define MOUSE_CTRL_LB BIT(0) /**< @brief Mouse left button pressed bit in the packet byte */
#define MOUSE_CTRL_RB BIT(1) /**< @brief Mouse right button pressed bit in the packet byte */
#define MOUSE_CTRL_MB BIT(2) /**< @brief Mouse middle button pressed bit in the packet byte */
#define MOUSE_CTRL_X_SIG BIT(4) /**< @brief Mouse X sign bit in the packet byte */
#define MOUSE_CTRL_Y_SIG BIT(5) /**< @brief Mouse Y sign bit in the packet byte */
#define MOUSE_CTRL_X_OFW BIT(6) /**< @brief Mouse X overflow bit in the packet byte */
#define MOUSE_CTRL_Y_OFW BIT(7) /**< @brief Mouse Y overflow bit in the packet byte */

#define MOUSE_DATA_REPORT_ENABLE 0xF4  /**< @brief Command to enable mouse data reporting */
#define MOUSE_DATA_REPORT_DISABLE 0xF5 /**< @brief Command to disable mouse data reporting */

/** @} */

#endif /* _LCOM_I8042_H */
