#ifndef _LCOM_I8254_H_
#define _LCOM_I8254_H_

#include <lcom/lcf.h>

/** @defgroup i8254 i8254
 * @{
 *
 * Constants for programming the i8254 Timer. Needs to be completed.
 */

/**< @brief minimum frequency value for timer */
#define TIMER_MIN_FREQ (TIMER_FREQ / UINT16_MAX) + ((TIMER_FREQ % UINT16_MAX) ? 1 : 0)
#define TIMER_FREQ     1193182 /**< @brief clock frequency for timer in PC and AT */
#define TIMER0_IRQ     0       /**< @brief Timer 0 IRQ line */

/* I/O port addresses */

#define TIMER_0    0x40 /**< @brief Timer 0 count register */
#define TIMER_1    0x41 /**< @brief Timer 1 count register */
#define TIMER_2    0x42 /**< @brief Timer 2 count register */
#define TIMER_CTRL 0x43 /**< @brief Control register */

#define SPEAKER_CTRL 0x61 /**< @brief Register for speaker control  */

/* Timer control */

/* Timer selection: bits 7 and 6 */

#define TIMER_SEL0   0x00              /**< @brief Control Word for Timer 0 */
#define TIMER_SEL1   BIT(6)            /**< @brief Control Word for Timer 1 */
#define TIMER_SEL2   BIT(7)            /**< @brief Control Word for Timer 2 */
#define TIMER_RB_CMD (BIT(7) | BIT(6)) /**< @brief Read Back Command */

/* Register selection: bits 5 and 4 */

#define TIMER_LSB       BIT(4)                  /**< @brief Initialize Counter LSB only */
#define TIMER_MSB       BIT(5)                  /**< @brief Initialize Counter MSB only */
#define TIMER_LSB_MSB   (TIMER_LSB | TIMER_MSB) /**< @brief Initialize LSB first and then MSB */
#define TIMER_INIT_MODE 0x30                    /**< @brief 00110000 (bits 4 and 5) */

/* Operating mode: bits 3, 2 and 1 */

#define TIMER_SQR_WAVE (BIT(2) | BIT(1)) /**< @brief Mode 3: square wave generator */
#define TIMER_RATE_GEN BIT(2)            /**< @brief Mode 2: rate generator */
#define TIMER_INCOMP_2 6                 /**< @brief value (of incompatibility) for mode 2 */
#define TIMER_INCOMP_3 7                 /**< @brief value (of incompatibility) for mode 3 */
#define TIMER_OP_MODE  0x0E              /**< @brief 00001110 (bits 1, 2 and 3) */

/* Counting mode: bit 0 */

#define TIMER_BCD 0x01 /**< @brief Count in BCD */
#define TIMER_BIN 0x00 /**< @brief Count in binary */

/* READ-BACK COMMAND FORMAT */

#define TIMER_RB_COUNT_  BIT(5)       /**< @brief Read-back count bit */
#define TIMER_RB_STATUS_ BIT(4)       /**< @brief Read-back status bit */
#define TIMER_RB_SEL(n)  BIT((n) + 1) /**< @brief Read-back select bit */

/**@}*/

#endif /* _LCOM_I8254_H */
