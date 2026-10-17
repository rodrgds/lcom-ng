#ifndef __TIMER_H
#define __TIMER_H

#include "i8254.h"
/** @defgroup timer timer
 * @{
 *
 * Functions that handle the timer part of the project
 */

/**
 * @brief For each 60 interruptions of the timer, that equals a second, shows a message.
 *
 * @param time in seconds to be counted
 *
 * @return 0 if success, 1 if not
 */
int timer_int(unsigned long time);

#endif
