#pragma once

#define RTC_IRQ 8

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

#define RTC_SECONDS 0x00
#define RTC_SECONDS_ALARM 0x01
#define RTC_MINUTES 0x02
#define RTC_MINUTES_ALARM 0x03
#define RTC_HOURS 0x04
#define RTC_HOURS_ALARM 0x05
#define RTC_DAY_OF_WEEK 0x06
#define RTC_DAY_OF_MONTH 0x07
#define RTC_MONTH 0x08
#define RTC_YEAR 0x09

#define REGISTER_A 0x0A
#define REGISTER_B 0x0B
#define REGISTER_C 0x0C
#define REGISTER_D 0x0D

#define BIT(n) (0x01<<(n))

#define RTC_SET BIT(7)
#define RTC_PIE BIT(6)
#define RTC_AIE BIT(5)
#define RTC_UIE BIT(4)

#define RTC_PF BIT(6)
#define RTC_AF BIT(5)
#define RTC_UF BIT(4)

int (rtc_subscribe_int)(uint8_t *bit_no);

int (rtc_unsubscribe_int)();

int rtc_choose_reg(uint8_t reg);

int rtc_write_reg(uint8_t reg, uint8_t d);

int rtc_read_reg(uint8_t reg, uint8_t *d);

int rtc_enable_just_update_interrupts();

int rtc_backup();

int rtc_restore();

void rtc_initial_state_is_day();

void rtc_ih();
