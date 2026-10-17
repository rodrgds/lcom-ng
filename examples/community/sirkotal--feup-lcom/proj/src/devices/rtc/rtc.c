#include "rtc.h"

int rtc_hook_id = 4;
rtc_time_info time_info; 
uint8_t binary_mode; 

/**
 * @brief Converts a BCD number into binary
 * 
 * @return Returns the BCD number in binary
 */
uint16_t bcd_to_bin(uint16_t bcd) {
  uint16_t bin = 0;

  bin = ((bcd >> 4) * 10) + (bcd & 0xF);

  return bin;
}

int (rtc_subscribe_int)(uint8_t *bit_no) {
    *bit_no = BIT(rtc_hook_id);
    return sys_irqsetpolicy(IRQ_RTC, IRQ_REENABLE, &rtc_hook_id);
}

int (rtc_unsubscribe_int)() {
    return sys_irqrmpolicy(&rtc_hook_id);
}

// might be needed later
/* void rtc_ih() {
    int cause;
    unsigned long regA;

    sys_outb(RTC_ADDR_REG, RTC_REG_C);
    cause = sys_inb(RTC_DATA_REG, &regA);

    if (cause & UF)
        handle_update_int();
    if (cause & AF)
        handle_alarm_int();
    if (cause & PF)
        handle_periodic_int();
} */

int (rtc_output)(uint8_t command, uint8_t *output) {
    if (sys_outb(RTC_ADDR_REG, command)) {
        return 1;
    }

    if (util_sys_inb(RTC_DATA_REG, output)) {
        return 1;
    }
    
    return 0;
}

void (rtc_set)() {
    binary_mode = rtc_check_binary();
    update_rtc_time();
}

int (rtc_check_binary)() {
    uint8_t result;
    if (rtc_output(RTC_REG_B, &result)) {
        return 1;
    }
	return result & BINARY;
}

int (rtc_is_updating()) {
    uint8_t res;
    if (rtc_output(RTC_REG_A, &res)) {
        return 1;
    }

	return (res & IRQF);
}

int (update_rtc_time()) {
    uint8_t output;
    if (rtc_is_updating()) {
        return 1;
    }

    if (rtc_output(RTC_SECONDS_REG, &output)) {
        return 1;
    }

    if (rtc_output(RTC_MINUTES_REG, &output)) {
        return 1;
    }

    if (rtc_output(RTC_HOURS_REG, &output)) {
        return 1;
    }

    if (rtc_output(RTC_MONTH_DATE, &output)) {
        return 1;
    }

    if (rtc_output(RTC_MONTH, &output)) {
        return 1;
    }

    if (rtc_output(RTC_YEAR, &output)) {
        return 1;
    }

    time_info.seconds = binary_mode ? output : bcd_to_bin(output);
    time_info.minutes = binary_mode ? output : bcd_to_bin(output);
    time_info.hours = binary_mode ? output : bcd_to_bin(output);
    time_info.day = binary_mode ? output : bcd_to_bin(output);
    time_info.month = binary_mode ? output : bcd_to_bin(output);
    time_info.year = binary_mode ? output : bcd_to_bin(output);

    return 0;
}
