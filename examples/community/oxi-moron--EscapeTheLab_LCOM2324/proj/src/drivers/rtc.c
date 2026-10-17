#include "rtc.h"
#include "rtc_macros.h"

int rtc_hook_id = 0;
bool rtc_alarm = false;

int (rtc_write_to_reg) (uint32_t reg, uint8_t data) {
    if (sys_outb(RTC_ADDR_REG, reg) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }
    if (sys_outb(RTC_DATA_REG, data) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }
    return 0;
}

int (rtc_read_from_reg) (uint32_t reg, uint8_t* data) {
    if (sys_outb(RTC_ADDR_REG, reg) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }
    if (util_sys_inb(RTC_DATA_REG, data) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }
    return 0;
}

int (rtc_set_alarm) (uint8_t alarm_hours, uint8_t alarm_minutes, uint8_t alarm_seconds) {
    uint8_t reg_b, reg_a, hours, minutes, seconds;

    do {
        if (rtc_read_from_reg(RTC_REG_A, &reg_a) != 0) {
            printf("ERROR: %s\n", __func__);
            return 1;
        }
    } while ((reg_a & RTC_UIP) != 0);

    if (rtc_read_from_reg(RTC_REG_B, &reg_b) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }
    reg_b |= RTC_AIE | RTC_24H;

    if (rtc_write_to_reg(RTC_REG_B, reg_b) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    if (rtc_get_time(&hours, &minutes, &seconds) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    hours = to_binary(hours); minutes = to_binary(minutes); seconds = to_binary(seconds);
    uint32_t alarm_time = (hours * 3600 + minutes * 60 + seconds) + (alarm_hours * 3600 + alarm_minutes * 60 + alarm_seconds);
    hours = alarm_time / 3600;
    alarm_time -= hours * 3600;
    minutes = alarm_time / 60;
    alarm_time -= minutes * 60;
    seconds = alarm_time;
    hours = to_bcd(hours); minutes = to_bcd(minutes); seconds = to_bcd(seconds);

    do {
        if (rtc_read_from_reg(RTC_REG_A, &reg_a) != 0) {
            printf("ERROR: %s\n", __func__);
            return 1;
        }
    } while ((reg_a & RTC_UIP) != 0);

    if (rtc_write_to_reg(RTC_HOURS_ALARM, hours) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    if (rtc_write_to_reg(RTC_MINS_ALARM, minutes) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    if (rtc_write_to_reg(RTC_SECS_ALARM, seconds) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    return 0;
}

void (rtc_ih) () {
    uint8_t reg_c;
    rtc_read_from_reg(RTC_REG_C, &reg_c);

    if (reg_c & RTC_AF) rtc_alarm = true;
}

int (rtc_subscribe_int) (uint8_t* bit_no) {
    rtc_hook_id = *bit_no;
    if (sys_irqsetpolicy(RTC_IRQ_LINE, IRQ_REENABLE, &rtc_hook_id) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }
    return 0;
}

int (rtc_unsubscribe_int) () {
    if (sys_irqrmpolicy(&rtc_hook_id) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }
    return 0;
}

int (rtc_get_time) (uint8_t* hours, uint8_t* minutes, uint8_t* seconds) {
    if (rtc_read_from_reg(RTC_HOURS, hours) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    if (rtc_read_from_reg(RTC_MINS, minutes) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    if (rtc_read_from_reg(RTC_SECS, seconds) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    return 0;
}

int (rtc_disable_alarm) () {
    uint8_t reg_a, reg_b;
    do {
        if (rtc_read_from_reg(RTC_REG_A, &reg_a) != 0) {
            printf("ERROR: %s\n", __func__);
            return 1;
        }
    } while ((reg_a & RTC_UIP) != 0);

    if (rtc_read_from_reg(RTC_REG_B, &reg_b) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    reg_b &= RTC_AID;

    if (rtc_write_to_reg(RTC_REG_B, reg_b) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    return 0;
}

uint8_t (to_binary) (uint8_t bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0xF);
}

uint8_t (to_bcd) (uint8_t binary) {
    int bcd = 0;
    int factor = 1;

    while (binary != 0) {
        bcd += (binary % 10) * factor;
        binary /= 10;
        factor <<= 4;
    }

    return bcd;
}
