#ifndef __RTC_H
#define __RTC_H

#include <minix/sysutil.h>
#include <lcom/lcf.h>


typedef struct {
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} rtc_info;

#define INPUT_REG           0x70
#define OUTPUT_REG          0x71

#define UPDATE_REG          10
#define COUNT_REG           11
#define UPDATE              BIT(7)
#define BINARY              BIT(2)

#define IRQ_RTC             8
#define RTC_SEC             0
#define RTC_MIN             2
#define RTC_HOURS           4   
#define RTC_DAY             7
#define RTC_MONTH           8
#define RTC_YEAR            9

int (rtc_subscribe_int)();

int (rtc_unsubscribe_int)();

int (rtc_read) (uint8_t command, uint8_t *output);

int (rtc_in_progress)();

int (rtc_binary_count)();

int (rtc_update_time_info) ();

int (get_game_time) (uint8_t *hours, uint8_t *minutes, uint8_t *seconds);

void (display_game_time)();

#endif /* __RTC_H */

