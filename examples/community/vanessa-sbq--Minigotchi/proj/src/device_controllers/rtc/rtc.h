#ifndef _LCOM_RTC_H_
#define _LCOM_RTC_H_

#include <stdbool.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>
#include <stdio.h>

#include "../utils.h"

typedef struct {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t day_of_week;
    uint8_t day_of_month;
    uint8_t month;
    uint8_t year;
    uint8_t reg_a;
    uint8_t reg_b;
    uint8_t reg_c;
} RTC_Config;

#include "rtc.h"
#include "rtc_macros.h"

void rtc_ih();

int bcd_to_decimal(uint8_t * number);
int binary_to_bcd(uint8_t * number);

int rtc_subscribe_int(uint32_t *bit_no);
int enable_aie_int();

int rtc_unsubscribe_int();
int disable_pie_int();
int disable_aie_int();
int disable_uie_int();

RTC_Config rtc_get_config(void);
bool rtc_is_updating();

int rtc_read_register(uint8_t reg, uint8_t * byte);
int rtc_clear_flags();
int rtc_write_to_port(uint8_t port, uint8_t reg);
int rtc_write_reg(uint8_t reg, uint8_t byte);
int rtc_set_alarm(uint8_t sec, uint8_t min, uint8_t hour);

bool rtc_get_interrupt_was_alarm();
bool rtc_get_interrupt_was_periodic();
bool rtc_get_interrupt_was_update();

void rtc_set_interrupt_alarm_processed();
void rtc_set_interrupt_periodic_processed();
void rtc_set_interrupt_update_processed();

#endif
