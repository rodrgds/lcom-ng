#ifndef _LCOM_I8254_H_
#define _LCOM_I8254_H_

/** @defgroup i8254 i8254
 * @{
 *
 * Constants for programming the i8254 Timer. Needs to be completed.
 */

#define BIT(n) (0x01<<n)

#define KBC_IRQ				1
#define MOUSE_IRQ			12
#define STAT_REG 			0x64
#define KBC_CMD_REG 		0x64
#define IBF 				BIT(1)
#define WAIT_KBC			20000
#define WAIT_FIRST			300000
#define KBC_READ_CMD		0x20
#define OBF					BIT(0)
#define OUT_BUF				0x60
#define DATA_REG			0x60
#define PAR_ERR				BIT(7)
#define TO_ERR				BIT(6)
#define KBC_LEDS_CMD		0xED
#define KBC_LED_CAPS		BIT(2)
#define KBC_LED_NUMLOCK		BIT(1)
#define KBC_LED_SCRLOCK		BIT(0)
#define ENABLE_MOUSE_CMD	0xA8
#define RESET_MOUSE_CMD		0xFF
#define WRITE_MOUSE			0xD4
#define ACK					0xFA
#define WAIT_TIME			500000
#define MOUSE_OK			0xAA
#define MOUSE_DEVICE_ID		0
#define ENABLE_PACKETS 		0xF4
#define ENABLE_STREAM 		0xEA
#define DISABLE_STREAM 		0xF5
#define CMD_REG 			0x64
#define DELAY_US 20000
#define MOUSE_YSIGN BIT(5)
#define MOUSE_XSIGN BIT(4)
#define MOUSE_YOVFL BIT(7)
#define MOUSE_XOVFL BIT(6)

#define TIMER_FREQ  1193182    /**< @brief clock frequency for timer in PC and AT */

#define TIMER0_IRQ	        0    /**< @brief Timer 0 IRQ line */

/* I/O port addresses */

#define TIMER_0			0x40 /**< @brief Timer 0 count register */
#define TIMER_1			0x41 /**< @brief Timer 1 count register */
#define TIMER_2			0x42 /**< @brief Timer 2 count register */
#define TIMER_CTRL		0x43 /**< @brief Control register */

#define SPEAKER_CTRL		0x61 /**< @brief Register for speaker control  */

/* Timer control */

/* Timer selection: bits 7 and 6 */

#define TIMER_SEL0		0x00     /**< @brief Control Word for Timer 0 */
#define TIMER_SEL1		BIT(6)   /**< @brief Control Word for Timer 1 */
#define TIMER_SEL2		BIT(7)   /**< @brief Control Word for Timer 2 */
#define TIMER_RB_CMD		(BIT(7)|BIT(6))  /**< @brief Read Back Command */

/* Register selection: bits 5 and 4 */

#define TIMER_LSB		BIT(4)  /**< @brief Initialize Counter LSB only */
#define TIMER_MSB		BIT(5)  /**< @brief Initialize Counter MSB only */
#define TIMER_LSB_MSB		(TIMER_LSB | TIMER_MSB) /**< @brief Initialize LSB first and MSB afterwards */ 

/* Operating mode: bits 3, 2 and 1 */

#define TIMER_SQR_WAVE		(BIT(2)|BIT(1)) /**< @brief Mode 3: square wave generator */
#define TIMER_RATE_GEN		BIT(2)          /**< @brief Mode 2: rate generator */
#define TIMER_READ0 BIT(1)
#define TIMER_READ1 BIT(2)
#define TIMER_READ2 BIT(3)

/* Counting mode: bit 0 */

#define TIMER_BCD			0x01   /**< @brief Count in BCD */
#define TIMER_BIN			0x00   /**< @brief Count in binary */

/**@}*/

#endif /* _LCOM_I8254_H */
