#include "src/include/controllers/rtc/rtc.h"
#include "src/include/utils.h"
#include <lcom/lcf.h>

static int rtc_hook_id = 5;
static rtc_time_t current_time = {0, 0, 0, 0, 0, 0};

int rtc_output(uint8_t command, uint8_t *output) {
    if (!output) return 1;
    
    if (sys_outb(RTC_ADDR_REG, command) != 0) return 1;
    if (util_sys_inb(RTC_DATA_REG, output) != 0) return 1;
    
    return 0;
}

int rtc_is_updating() {
    uint8_t result;
    if (rtc_output(RTC_UPDATE_STATUS, &result) != 0) return -1;
    return (result & RTC_UPDATE_IN_PROGRESS) ? 1 : 0;
}

int rtc_is_binary() {
    uint8_t status;
    if (rtc_output(RTC_COUNT_STATUS, &status) != 0) return -1;
    return (status & RTC_BINARY_MODE) ? 1 : 0;
}

uint8_t rtc_bcd_to_binary(uint8_t bcd_number) {
    return ((bcd_number >> 4) * 10) + (bcd_number & 0xF);
}

int rtc_read_time(rtc_time_t *time) {
    if (!time) return 1;
    
    int updating_check = rtc_is_updating();
    if (updating_check == -1) return 1;
    if (updating_check == 1) return 1; 
    
    int binary_mode = rtc_is_binary();
    if (binary_mode == -1) return 1;
    
    uint8_t temp;
    
    if (rtc_output(RTC_SECONDS, &temp) != 0) return 1;
    time->seconds = binary_mode ? temp : rtc_bcd_to_binary(temp);
    
    if (rtc_output(RTC_MINUTES, &temp) != 0) return 1;
    time->minutes = binary_mode ? temp : rtc_bcd_to_binary(temp);
    
    if (rtc_output(RTC_HOURS, &temp) != 0) return 1;
    time->hours = binary_mode ? temp : rtc_bcd_to_binary(temp);
    
    if (rtc_output(RTC_DAY, &temp) != 0) return 1;
    time->day = binary_mode ? temp : rtc_bcd_to_binary(temp);
    
    if (rtc_output(RTC_MONTH, &temp) != 0) return 1;
    time->month = binary_mode ? temp : rtc_bcd_to_binary(temp);
    
    if (rtc_output(RTC_YEAR, &temp) != 0) return 1;
    time->year = binary_mode ? temp : rtc_bcd_to_binary(temp);
    
    return 0;
}

int rtc_subscribe_int(uint8_t *bit_no) {
    if (!bit_no) return 1;
    
    *bit_no = BIT(rtc_hook_id);
    return sys_irqsetpolicy(IRQ_RTC, IRQ_REENABLE, &rtc_hook_id);
}

int rtc_unsubscribe_int() {
    return sys_irqrmpolicy(&rtc_hook_id);
}

void rtc_ih() {
    rtc_read_time(&current_time);
}

int rtc_init() {
    return rtc_read_time(&current_time);
}

int rtc_get_time(rtc_time_t *time) {
    if (!time) return 1;
    return rtc_read_time(time);
}

