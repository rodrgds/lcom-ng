#ifndef _LCOM_TIMER_H_
#define _LCOM_TIMER_H_

#include <lcom/lcf.h>

/** @defgroup timer timer
 * @{
 * 
 * Additional functions used for programming the timer
 */

/**
 * @brief get the current counter valur for the timer
 *
 * @return the current counter value
 */
unsigned long(get_timer_get_counter)();

/**@}*/

#endif /* _LCOM_I8254_H */
