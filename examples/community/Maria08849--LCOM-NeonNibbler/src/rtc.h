#ifndef _LCOM_RTC_H_
#define _LCOM_RTC_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>

int(rtc_init)();
int(rtc_read_conf)(uint8_t cmd, uint8_t *output);
int(rtc_time)();
int(rtc_update)();
uint8_t(bcd_to_binary)(uint8_t n_bcd);
int(rtc_bcd)();
int(rtc_binary)();
int(rtc_unsubscribe_interrupts)();
int(rtc_subscribe_interrupts)();
bool(isDaytime)();

#endif /* _LCOM_RTC_H_ */
