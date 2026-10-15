/// @file rtc.c
/// @brief Real Time Clock (RTC) interaction functions for reading system time.

#include "rtc.h"

/// @brief Interrupt hook ID for the RTC
int rtc_hook_id = RTC_MASK;

/// @brief Stores the current time information
real_time_info time_info;

/// @brief 1 if RTC is in binary mode, 0 if in BCD
uint8_t binary_mode;

int rtc_subscribe_int() {
    return sys_irqsetpolicy(IRQ_RTC, IRQ_REENABLE, &rtc_hook_id);
}

int rtc_unsubscribe_int() {
    return sys_irqrmpolicy(&rtc_hook_id);
}

int rtc_output(uint8_t command, uint8_t *output) {
    if (sys_outb(REGISTER_INPUT, command) != 0) return 1;
    if (util_sys_inb(REGISTER_OUTPUT, output) != 0) return 1;
    return 0;
}

int rtc_is_updating() {
    uint8_t result;
    if (rtc_output(REGISTER_UPDATING, &result)) return 1;
    return result & UPDATING;
}

int rtc_is_binary() {
    uint8_t result;
    if (rtc_output(REGISTER_COUNTING, &result)) return 1;
    return result & BINARY;
}

uint8_t to_binary(uint8_t bcd_number) {
    return ((bcd_number >> 4) * 10) + (bcd_number & 0xF);
}

int rtc_update_time() {

    if (rtc_is_updating() != 0) return 1;
    uint8_t output;

    // segundos
    if (rtc_output(SECONDS, &output) != 0) return 1;
    time_info.seconds = binary_mode ? output : to_binary(output);

    // minutos
    if (rtc_output(MINUTES, &output) != 0) return 1;
    time_info.minutes = binary_mode ? output : to_binary(output);

    // horas
    if (rtc_output(HOURS, &output) != 0) return 1;
    time_info.hours = binary_mode ? output : to_binary(output);

    // dia
    if (rtc_output(DAY, &output) != 0) return 1;
    time_info.day = binary_mode ? output : to_binary(output);

    // mês
    if (rtc_output(MONTH, &output) != 0) return 1;
    time_info.month = binary_mode ? output : to_binary(output);

    // ano
    if (rtc_output(YEAR, &output) != 0) return 1;
    time_info.year = binary_mode ? output : to_binary(output);

    return 0;
}

