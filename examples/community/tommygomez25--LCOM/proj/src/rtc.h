#pragma once
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "utils.h"
#include <math.h>

#define RTC_IRQ 8

#define RTC_ADDR_REG 0x70 //WRITE THE ADDRESS OF THE REGISTER TO RTC_ADDR_REG 
#define RTC_DATA_REG 0x71 //READ/WRITE ONE BYTE FROM/TO RTC_DATA_REG

#define RTC_SECONDS 0
#define RTC_MINUTES 2
#define RTC_HOURS 4
#define RTC_WEEK_DAY 6
#define RTC_DAY 7
#define RTC_MONTH 8
#define RTC_YEAR 9

#define RTC_A 10
#define RTC_UIP BIT(7)

#define RTC_B 11
#define RTC_SET BIT(7)
#define RTC_PIE BIT(6)
#define RTC_AIE BIT(5)
#define RTC_UIE BIT(4)
#define RTC_DM BIT(2)

#define RTC_C 12
#define RTC_PF BIT(6)
#define RTC_AF BIT(5)
#define RTC_UF BIT(4)

#define RTC_D 13

int (rtc_subscribe_int)(uint8_t *bit_no);

int (rtc_unsubscribe_int)();

void (rtc_ih)();

int rtc_read(uint8_t reg);

uint8_t convert_from_binary (uint8_t bcdNum);

void print_clock();

int wait_regA_rtc();
