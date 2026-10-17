#include "rtc.h"

/** @defgroup rtc rtc
 * @{
 *
 * Functions for real time clock
 */

rtc_info time_info; 
int hook_id_rtc = 5;

/**
 * @brief Subscribe the RTC interrupts.
 * @return 0 if success and 1 if error.
 */
int rtc_subscribe_int() {
    return sys_irqsetpolicy(IRQ_RTC, IRQ_REENABLE, &hook_id_rtc);
}

/**
 * @brief Unsubscribe the RTC interrupts.
 * @return 0 if success and 1 if error.
 */
int rtc_unsubscribe_int() {
    return sys_irqrmpolicy(&hook_id_rtc);
}

/**
 * @brief Reads a value from the RTC.
 * @param command The RTC register to read from.
 * @param out Pointer to the variable where the read value will be.
 * @return 0 if success and 1 if error.
 */
int rtc_read(uint8_t command, uint8_t *out) {
    if (sys_outb(INPUT_REG, command) != 0) 
        return 1;
    if (util_sys_inb(OUTPUT_REG, out) != 0) 
        return 1;
    return 0;
}

/**
 * @brief Sees if an RTC update is in progress or not.
 * @return 1 if an update is in progress and 0 if it isn't.
 */
int rtc_in_progress() {
    uint8_t res;
    if (rtc_read(UPDATE_REG, &res) != 0) 
        return 1;
    return res & UPDATE;
}

/**
 * @brief Sees if RTC is in binary counting mode.
 * @return 1 if RTC is in binary mode and 0 if it isn't.
 */
int rtc_binary_count() {
    uint8_t res;
    if (rtc_read(COUNT_REG, &res) != 0) 
        return 1;
    return res & BINARY;
}

/**
 * @brief Updates the rtc_info variable time_info with the current time from the RTC.
 * @return 0 if success and 1 if error.
 */
int rtc_update_time_info() {
    if (rtc_in_progress() != 0)
         return 1;
    uint8_t out;

    if (rtc_read(RTC_SEC, &out) != 0) 
        return 1;
    time_info.seconds = rtc_binary_count() ? out : ((out >> 4) * 10) + (out & 0xF);

    if (rtc_read(RTC_MIN, &out) != 0) 
        return 1;
    time_info.minutes = rtc_binary_count() ? out : ((out >> 4) * 10) + (out & 0xF);

    if (rtc_read(RTC_HOURS, &out) != 0) 
        return 1;
    time_info.hours = rtc_binary_count() ? out : ((out >> 4) * 10) + (out & 0xF);

    if (rtc_read(RTC_DAY, &out) != 0) 
        return 1;
    time_info.day = rtc_binary_count() ? out : ((out >> 4) * 10) + (out & 0xF);

    if (rtc_read(RTC_MONTH, &out) != 0) 
        return 1;
    time_info.month = rtc_binary_count() ? out : ((out >> 4) * 10) + (out & 0xF);

    if (rtc_read(RTC_YEAR, &out) != 0) 
        return 1;
    time_info.year = rtc_binary_count() ? out : ((out >> 4) * 10) + (out & 0xF);

    return 0;
}

/**
 * @brief Retrieves the current game time.
 * @param hours Pointer to the variable that will store the current hours.
 * @param minutes Pointer to the variable that will store the current minutes.
 * @param seconds Pointer to the variable that will store the current seconds.
 * @return 0 if success and 1 if error.
 */
int (get_game_time) (uint8_t *hours, uint8_t *minutes, uint8_t *seconds) {
    rtc_binary_count();
    if (rtc_update_time_info() != 0) {
        return 1;
    }
    *hours = time_info.hours;
    *minutes = time_info.minutes;
    *seconds = time_info.seconds;
    return 0;
}

/**
 * @brief Shows the current game time.
 */
void (display_game_time)() {
    uint8_t hours, minutes, seconds;
    get_game_time(&hours, &minutes, &seconds);

    printf("Game time: %02d:%02d:%02d\n", hours, minutes, seconds);
}

