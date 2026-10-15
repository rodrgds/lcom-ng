#include "rtc.h"

#include <lcom/lcf.h>
#include <stdint.h>

static int rtc_hook_id = RTC_MASK;
static real_time_info time_info;
static uint8_t binary_mode;

void rtc_setup(void) {
    binary_mode = rtc_is_binary();
    rtc_update_time();
}

int rtc_subscribe_interrupts(void) {
    return sys_irqsetpolicy(IRQ_RTC, IRQ_REENABLE, &rtc_hook_id);
}

int rtc_unsubscribe_interrupts(void) {
    return sys_irqrmpolicy(&rtc_hook_id);
}

void rtc_ih(void) {
    uint8_t reg_c;
    rtc_output(REGISTER_C, &reg_c);
}

int rtc_output(uint8_t command, uint8_t *output) {
    if (sys_outb(REGISTER_INPUT, command) != 0) return 1;
    if (util_sys_inb(REGISTER_OUTPUT, output) != 0) return 1;
    return 0;
}

int rtc_is_updating(void) {
    uint8_t result;
    if (rtc_output(REGISTER_UPDATING, &result) != 0) return 1;
    return result & UPDATING;
}

int rtc_is_binary(void) {
    uint8_t result;
    if (rtc_output(REGISTER_COUNTING, &result) != 0) return 1;
    return result & BINARY;
}

int rtc_is_bcd(void) {
    return !rtc_is_binary();
}

uint8_t to_binary(uint8_t bcd_number) {
    return ((bcd_number >> 4) * 10) + (bcd_number & 0x0F);
}

int rtc_update_time(void) {
    if (rtc_is_updating() != 0) return 1;

    uint8_t output;

    if (rtc_output(SECONDS, &output) != 0) return 1;
    time_info.seconds = binary_mode ? output : to_binary(output);

    if (rtc_output(MINUTES, &output) != 0) return 1;
    time_info.minutes = binary_mode ? output : to_binary(output);

    if (rtc_output(HOURS, &output) != 0) return 1;
    time_info.hours = binary_mode ? output : to_binary(output);

    if (rtc_output(DAY, &output) != 0) return 1;
    time_info.day = binary_mode ? output : to_binary(output);

    if (rtc_output(MONTH, &output) != 0) return 1;
    time_info.month = binary_mode ? output : to_binary(output);

    if (rtc_output(YEAR, &output) != 0) return 1;
    time_info.year = binary_mode ? output : to_binary(output);

    return 0;
}

const real_time_info *rtc_get_time(void) {
    return &time_info;
}

int rtc_read_date(rtc_date *date) {
    if (date == NULL) return 1;
    rtc_setup();
    date->day   = time_info.day;
    date->month = time_info.month;
    date->year  = time_info.year;
    return 0;
}
