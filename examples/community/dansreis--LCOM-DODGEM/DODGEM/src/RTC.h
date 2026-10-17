#ifndef __RTC_H
#define __RTC_H

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define RTC_ADDR_R 0x70
#define RTC_DATA_R 0x71
#define RTC_IRQ 8

typedef struct{
	unsigned long sec;
	unsigned long min;
	unsigned long hours;
	unsigned long day;
	unsigned long month;
	unsigned long year;
}DATA;

/**
 * @brief subscribe RTC interrupts
 *
 * @return -1 if failed, 0 otherwise
 */
int rtc_subscribe_int(int exclusive);

/**
 * @brief unsubscribe RTC interrupts
 *
 * @return -1 if failed, 0 otherwise
 */
int rtc_unsubscribe_int();

/**
 * @brief returns de information of a certain reg of RTC
 *
 * @param reg register to access
 *
 * @return the value of the register
 */
unsigned long read_rtc(unsigned long reg);




#endif
