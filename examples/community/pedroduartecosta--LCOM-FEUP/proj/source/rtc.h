#ifndef __RTC_H
#define __RTC_H

static int hook_rtc=NOTIFICATION_RTC;

/** @defgroup RTC RTC
 * @{
 * functions regarding the rtc
 */

/**
 * @brief subscribes rtc interruptions
 *
 * @return rtc irq
 */
int rtc_subscribe();

/**
 * @brief unsubscribes rtc interruptions
 *
 * @return rtc irq
 */

int rtc_unsubscribe();

/**
 * @brief waits until it receives a valid rtc
 *
 */

void wait_valid_rtc(void);

/**
 * @brief reads rtc date and saves it no a string
 *
 * @param temporary string where date will be saved
 */
void display_date(char temp[]);


/**@}*/
#endif
