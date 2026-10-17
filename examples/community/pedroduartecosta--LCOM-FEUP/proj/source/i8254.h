#ifndef __I8254_H_
#define __I8254_H_

/** @defgroup i8254 i8254
 * @{
 *
 * Constants for programming the i8254 Timer. Needs to be completed.
 */

#define TIMER_FREQ  1193182    /**< @brief clock frequency for timer in PC and AT */

#define BIT(n) (0x01<<(n))

#define TIMER0_IRQ	        0    /**< @brief Timer 0 IRQ line */


#define NOTIFICATION_KBD          1
#define NOTIFICATION_MOUSE  	  12
#define NOTIFICATION_TIMER        0
#define NOTIFICATION_RTC		  8

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

/* Counting mode: bit 0 */

#define TIMER_BCD			0x01   /**< @brief Count in BCD */
#define TIMER_BIN			0x00   /**< @brief Count in binary */

/* READ-BACK COMMAND FORMAT */

#define TIMER_RB_COUNT_         BIT(5)
#define TIMER_RB_STATUS_        BIT(4)
#define TIMER_RB_SEL(n)         BIT((n)+1)

//keyboard
#define OUT_BUF 		0x60
#define KBD_IRQ 		1
#define VAL_ESC 		0x81
#define STAT_REG		0x64
#define DELAY_US 		20000


//mouse

#define MOUSE_IRQ           12
#define STAT_REG            0x64
#define OUT_BUF             0x60
#define IBF                 0x02
#define OBF                 0x01
#define W_TO_MOUSE          0xD4
#define COMMAND_PORT        0x60
#define PAR_ERR             0x80
#define TO_ERR              0x40
#define DISABLE_STREAM      0xF5
#define ENABLE_SEND         0xF4
#define SET_STREAM          0xEA
#define STATUS_REQUEST      0xE9
#define ACK                 0xFA
#define ENABLE_MOUSE        0xA8

//RTC
#define RTC_IRQ 			8
#define RTC_ADDR_REG 		0x70
#define RTC_DATA_REG 		0x71
#define RTC_REG_A			10
#define RTC_REG_B			11
#define RTC_REG_C			12
#define RTC_UIP				BIT(7)

/**@}*/

#endif /* _LCOM_I8254_H */
