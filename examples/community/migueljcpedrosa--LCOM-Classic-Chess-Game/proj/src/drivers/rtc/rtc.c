#include "rtc.h"


int hook_id = 5;
int rtc_irq = 8;

rtc_timestamp_t current_time;
bool rtc_binary_mode;

int rtc_initialize_system() {
    rtc_binary_mode = rtc_in_binary_mode();
    if (rtc_binary_mode != 0) {
        if (rtc_update_current_time() != 0) {
            return 1;  
        }
    }
    return 0;  
}

int rtc_in_binary_mode(){
    uint8_t counting_status;
    if (rtc_read_register(11, &counting_status)) return 1;
    return counting_status & BIT(2);
}

int rtc_read_register(uint8_t command, uint8_t *output) {
    if (sys_outb(RTC_COMMAND_REGISTER, command) != 0) return 1;
    if (util_sys_inb(RTC_DATA_REGISTER, output) != 0) return 1;
    return 0;
}

int rtc_subscribe_interrupts(uint8_t *bit_no) {
    hook_id = 5;

    *bit_no = hook_id;

    return sys_irqsetpolicy(rtc_irq, IRQ_REENABLE, &hook_id);
}

int rtc_unsubscribe_interrupts() {
    return sys_irqrmpolicy(&hook_id);
}

uint8_t rtc_convert_bcd_to_binary(uint8_t bcd_number) {
    return ((bcd_number >> 4) * 10) + (bcd_number & 0xF);
}

bool rtc_currently_updating() {
    uint8_t update_status;
    if (rtc_read_register(11, &update_status)) return true;
        return update_status & BIT(7);
}

int rtc_update_time_unit(uint8_t register_address, uint8_t* time_component) {
    uint8_t rtc_register_value;
    if (rtc_read_register(register_address, &rtc_register_value) != 0) return 1;
    if (rtc_binary_mode) {
        *time_component = rtc_register_value;
    } else {
        *time_component = rtc_convert_bcd_to_binary(rtc_register_value);
    }
    return 0;
}

int rtc_update_current_time() {
    if (rtc_currently_updating()) return 1;

    if (rtc_update_time_unit(RTC_SECONDS, &current_time.seconds) != 0) return 1;
    if (rtc_update_time_unit(RTC_MINUTES, &current_time.minutes) != 0) return 1;
    if (rtc_update_time_unit(RTC_HOURS, &current_time.hours) != 0) return 1;
    if (rtc_update_time_unit(RTC_DAY, &current_time.day) != 0) return 1;
    if (rtc_update_time_unit(RTC_MONTH, &current_time.month) != 0) return 1;
    if (rtc_update_time_unit(RTC_YEAR, &current_time.year) != 0) return 1;

    return 0;
}

int rtc_ih(rtc_timestamp_t* timestamp) {
    if (rtc_update_current_time() != 0) return 1;
    *timestamp = current_time;
    return 0;
}

