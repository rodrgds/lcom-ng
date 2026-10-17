#include <lcom/lcf.h>
#include "rtc.h"
#include "utils.h"

int rtc_hook_id;
uint8_t day = 0;
uint8_t month = 0;
uint8_t year = 0;
int rtc_count = 0;

int rtc_subscribe_int(uint8_t *bit_no) {
    rtc_hook_id = RTC_IRQ;
    *bit_no = rtc_hook_id;

    if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &rtc_hook_id) == OK) {
        return 0;
    }
    else {
        printf("sys_irqsetpolicy() failed\n");
        return 1;
    }
}

int rtc_unsubscribe_int() {
    if (sys_irqrmpolicy(&rtc_hook_id) != OK) {
        printf("sys_irqmpolicy() failed\n");
        return 1;
    }
    return 0;
}

int write_cmd_rtc(uint8_t cmd) {
    if (sys_outb(RTC_ADDR_REG, cmd) != OK) {
        printf("Error in sys_outb()\n");
        return 1;
    }
    return 0;
}

int read_reg_rtc(uint8_t *st) {
    if (util_sys_inb(RTC_DATA_REG  , st) != OK) {
        printf("Error in sys_inb()\n");
        return 1;
    }
    return 0;
}

int enable_or_disable_rtc(bool enable) {
    uint8_t st = 0;
    uint8_t st1 = 0;
    uint8_t st2 = 0;
    if (write_cmd_rtc(RTC_REG_B) != OK) {
        printf("Error in write_cmd_rtc()\n");
        return 1;
    }
    if (read_reg_rtc(&st) != OK) {
        printf("Error in read_reg_rtc()\n");
        return 1;
    }

    if (enable == true) {
        write_cmd_rtc(RTC_REG_A);
        read_reg_rtc(&st2);
        st2 = st2 | BIT(1) | BIT(2) | BIT(3) | BIT(0);
        write_cmd_rtc(RTC_REG_A);
        sys_outb(RTC_DATA_REG,st2);
        st = st | RTC_REG_B_PIE;
        if (write_cmd_rtc(RTC_REG_C) != OK) {
            printf("Error in write_cmd_rtc()\n");
            return 1;
        }
        if (read_reg_rtc(&st1) != OK) {
            printf("Error in write_cmd_rtc()\n");
            return 1;
        }
        st1 = st | !RTC_REG_C_PF  | !RTC_REG_C_AF | !RTC_REG_C_UF;
        if (write_cmd_rtc(RTC_REG_C) != OK) {
            printf("Error in write_reg_rtc()\n");
            return 1;
        }
    }
    else { 
        st = st & !RTC_REG_B_PIE;
    }

    if (write_cmd_rtc(RTC_REG_B) != OK) {
        printf("Error in write_reg_rtc()\n");
        return 1;
    }
    sys_outb(RTC_DATA_REG, st);
    return 0;
}

bool (rtc_ih)() {
    bool result = false;
    uint8_t st = 0;
    if (write_cmd_rtc(RTC_REG_C) != OK) {
        printf("Error in write_cmd_rtc()\n");
        return false;
    }
    if (read_reg_rtc(&st) != OK) {
        printf("Error in read_reg_rtc()\n");
        return false;
    }
    day=rtc_read_day() - 6 * (rtc_read_day() >> 4);
    month=rtc_read_month() - 6 * (rtc_read_month() >> 4);
    year=rtc_read_year() - 6 * (rtc_read_year() >> 4);

    rtc_count++;
    if (rtc_count == 12) {
        result= true;
        rtc_count = 0;
    }
    return result;

}
    
int rtc_read_day() {
    uint8_t day;
    if (write_cmd_rtc(RTC_DAY_MONTH) != OK) {
        printf("Error in write_cmd_rtc()\n");
        return 1;
    }
    if (read_reg_rtc(&day) != OK) {
        printf("Error in read_reg_rtc");
        return 1;
    }
    return day;
}

int rtc_read_month() {
    uint8_t month;
    if (write_cmd_rtc(RTC_MONTH) != OK) {
        printf("Error in write_cmd_rtc()\n");
        return 1;
    }
    if (read_reg_rtc(&month) != OK) {
        printf("Error in read_reg_rtc");
        return 1;
    }
    return month;
}

int rtc_read_year() {
    uint8_t year;
    if (write_cmd_rtc(RTC_YEAR) != OK) {
        printf("Error in write_cmd_rtc()\n");
        return 1;
    }
    if (read_reg_rtc(&year) != OK) {
        printf("Error in read_reg_rtc");
        return 1;
    }
    return year;
}
