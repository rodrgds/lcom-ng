#ifndef _LCOM_I8254_H_
#define _LCOM_I8254_H_

#include <lcom/lcf.h>

/** @defgroup i8254 i8254
 * @{
 *
 * Constants for programming the i8254 Timer. Needs to be completed.
 */

#define TIMER_FREQ 1193182 /**< @brief clock frequency for timer in PC and AT */
#define TIMER_COUNT_MIN 19 /**< @brief minimal count value that can be loaded */
#define TIMER0_IRQ 0 /**< @brief Timer 0 IRQ line */

/* I/O port addresses */

#define TIMER_0    0x40 /**< @brief Timer 0 count register */
#define TIMER_1    0x41 /**< @brief Timer 1 count register */
#define TIMER_2    0x42 /**< @brief Timer 2 count register */
#define TIMER_CTRL 0x43 /**< @brief Control register */
#define TIMER_PORT(n) (0x40 | n) /**< @brief Timer n count register*/

#define SPEAKER_CTRL 0x61 /**< @brief Register for speaker control  */

/* Timer control */

/* Timer selection: bits 7 and 6 */

#define TIMER_SEL0   0x00              /**< @brief Control Word for Timer 0 */
#define TIMER_SEL1   BIT(6)            /**< @brief Control Word for Timer 1 */
#define TIMER_SEL2   BIT(7)            /**< @brief Control Word for Timer 2 */
#define TIMER_RB_CMD (BIT(7) | BIT(6)) /**< @brief Read Back Command */

/* Register selection: bits 5 and 4 */

#define TIMER_LSB     BIT(4)                        /**< @brief Initialize Counter LSB only */
#define TIMER_MSB     BIT(5)                        /**< @brief Initialize Counter MSB only */
#define TIMER_LSB_MSB (TIMER_LSB | TIMER_MSB)       /**< @brief Initialize LSB first and MSB afterwards */
#define TIMER_GET_LSB_MSB (TIMER_LSB | TIMER_MSB)   /**< @brief Get the initialize mode from the status byte*/
#define ENSURE_MSB_ZERO(mode) ((mode == 6) ? 2 : (mode == 7) ? 3 : mode) /**< @brief Ensure that msb is 0 when mode is 2 or 3*/


/* Operating mode: bits 3, 2 and 1 */

#define TIMER_SQR_WAVE (BIT(2) | BIT(1))            /**< @brief Mode 3: square wave generator */
#define TIMER_RATE_GEN BIT(2)                       /**< @brief Mode 2: rate generator */
#define TIMER_GET_MODE (BIT(1) | BIT(2) | BIT(3))   /**< @brief Get mode from the status byte*/

/* Counting mode: bit 0 */

#define TIMER_BCD 0x01 /**< @brief Count in BCD */
#define TIMER_BIN 0x00 /**< @brief Count in binary */
#define TIMER_GET_BASE BIT(0) /**< @brief Bit associated with the mode */

/* READ-BACK COMMAND FORMAT */

#define TIMER_RB_COUNT_  BIT(5)         /**< @brief Activate the bit for reading the count, not going to be read*/
#define TIMER_RB_STATUS_ BIT(4)         /**< @brief Activate the bit for reading the status, not going to be read*/
#define TIMER_RB_SEL(n)  BIT((n) + 1)   /**< @brief Prepare the timer in the control word*/

/**@}*/

#endif /* _LCOM_I8254_H */
