//
// Created by joao on 24-04-2024.
//

#ifndef SHARED_RTC_H
#define SHARED_RTC_H
#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#define IRQ_RTC 8

#define SECONDS 0x00
#define MINUTES 0x02
#define HOURS 0x04
#define DAY_OF_THE_WEEK 0x06
#define DAY_OF_THE_MONTH  0x07
#define MONTH  0x08
#define YEAR  0x09
#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

#define UPDATING BIT(7)

#define REGISTER_A 10;
#define REGISTER_B 11;
/**
 * @brief Represents the time in real life.
 *  
*/
typedef struct {
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t day_week;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} Time_Irl;
int read_rtc(uint8_t command, uint8_t *st);
int rtc_update();
int rtc_update_time();
int rtc_subscribe_int();
int rtc_unsubscribe_int();

#endif //SHARED_RTC_H
