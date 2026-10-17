#ifndef _TIMER_H_
#define _TIMER_H_

/** @defgroup timer timer
 * @{
 *
 * Functions to manage timer abstraction (getting timer interrupts)
 */

#define BIT(n) (0x01<<(n))

#define TIMER0_IRQ	        	0    /**< @brief Timer 0 IRQ line */

#define DELAY_US 		   20000 /*sets the delay variable*/

/**
 * @brief Subscribe timer 0 interrupts
 *
 * @return hookID on success, -1 otherwise
 */


int timer_subscribe_int(void );

/**
 * @brief Unsubscribe timer 0 interrupts
 *
 * @return 0
 */

int timer_unsubscribe_int();

#endif

/** @} end of timer*/
