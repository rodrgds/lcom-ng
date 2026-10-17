#ifndef __RTC_H
#define __RTC_H

/** @defgroup rtc rtc
 * @{
 * Functions related to the rtc methods
 */

#include "macros_rtc.h"
#include <minix/drivers.h>
#include <stdlib.h>



/**
 * Struct with the date and hour parameters
 */
typedef struct{
	unsigned short seconds,minutes,hour,day, month,year;
}Date;

/*
 * Read the actual date
 * @param address address
 * @param result one of the struct parameters
 */
char readDate(unsigned char address, long *result);

/*
 * Write the date and the Score
 * @param score number of pipes
 */
int WriteScore(int score);

/** @} */

#endif /* __RTC_H */
