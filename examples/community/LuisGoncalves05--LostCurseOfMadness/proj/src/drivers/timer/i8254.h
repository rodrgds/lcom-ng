#ifndef I8254_H
#define I8254_H

#include <lcom/lcf.h>

/** @defgroup i8254 i8254
 * @{
 *
 * Constants for programming the i8254 Timers.
 */

/** @name Timer Frequency & IRQ Line */
/**@{*/
#define TIMER_FREQ 1193182 /**< @brief Clock frequency for timer in PC and AT (Hz) */
#define TIMER0_IRQ 0       /**< @brief Timer 0 IRQ line (IRQ 0) */
#define TIMER 0            /**< @brief Timer 0 (used for the game) */
/**@}*/

/** @name Timer I/O Port Addresses */
/**@{*/
#define TIMER_0 0x40    /**< @brief Timer 0 count register */
#define TIMER_1 0x41    /**< @brief Timer 1 count register */
#define TIMER_2 0x42    /**< @brief Timer 2 count register */
#define TIMER_CTRL 0x43 /**< @brief Timer control register */
/**@}*/

/** @name Timer Control */
/**@{*/
#define TIMER_COUNTING_MODE 0x7 /**< @brief Control word to set the counting mode */
/**@}*/

/** @name Timer Selection: bits 7 and 6 */
/**@{*/
#define TIMER_SEL0 0x00                /**< @brief Select Timer 0 */
#define TIMER_SEL1 BIT(6)              /**< @brief Select Timer 1 */
#define TIMER_SEL2 BIT(7)              /**< @brief Select Timer 2 */
#define TIMER_RB_CMD (BIT(7) | BIT(6)) /**< @brief Read-back command */
/**@}*/

/** @name Register Selection: bits 5 and 4 */
/**@{*/
#define TIMER_LSB BIT(4)                      /**< @brief Access mode: LSB only */
#define TIMER_MSB BIT(5)                      /**< @brief Access mode: MSB only */
#define TIMER_LSB_MSB (TIMER_LSB | TIMER_MSB) /**< @brief Access mode: LSB followed by MSB */
/**@}*/

/** @name Operating Mode: bits 3, 2 and 1 */
/**@{*/
#define TIMER_RATE_GEN BIT(2)            /**< @brief Mode 2: Rate generator */
#define TIMER_SQR_WAVE (BIT(2) | BIT(1)) /**< @brief Mode 3: Square wave generator */
/**@}*/

/** @name Counting Mode: bit 0 */
/**@{*/
#define TIMER_BIN 0x00 /**< @brief Binary counting mode (16-bit) */
#define TIMER_BCD 0x01 /**< @brief BCD counting mode (4-digit BCD) */
/**@}*/

/** @name Read-Back Command Format */
/**@{*/
#define TIMER_RB_COUNT_ BIT(5)             /**< @brief Read-back: request count value */
#define TIMER_RB_STATUS_ BIT(4)            /**< @brief Read-back: request status */
#define TIMER_RB_SEL(n) BIT((n) + 1)       /**< @brief Select timer n (0-2) for read-back */
#define TIMER_RB_COMMAND (BIT(7) | BIT(6)) /**< @brief Read-back command mask */
/**@}*/

/**@}*/ // End of i8254 group

#endif /* LCOM_I8254_H */
