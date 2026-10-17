#include "rtc.h"
#include "auxfuncs.h"
#include "defines.h"

/** @defgroup Real_Time_Clock
 * @{
 *
 * Functions related to the use of the real-time clock in the project
 */

typedef struct {
  uint8_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hours;
  uint8_t minutes;
  uint8_t seconds;
} real_time_info;

int hook_id_rtc = RTC_MASK;
real_time_info info;
uint8_t binary;

/**
 * @brief Function to subscribe to RTC interrupts.
 *
 * This function subscribes to RTC interrupts by setting the interrupt policy to IRQ_REENABLE
 * and storing the hook_id_rtc identifier.
 *
 * @return 0 upon success, non-zero otherwise.
 */
int(rtc_subscribe_interrupts)() {
  sys_irqsetpolicy(RTC_IH, IRQ_REENABLE, &hook_id_rtc);

  return 0;
}

/**
 * @brief Function to unsubscribe from RTC interrupts.
 *
 * This function unsubscribes from RTC interrupts by removing the interrupt policy associated with hook_id_rtc.
 *
 * @return 0 upon success, non-zero otherwise.
 */
int(rtc_unsubscribe_interrupts)() {
  sys_irqrmpolicy(&hook_id_rtc);

  return 0;
}

/**
 * @brief Function to convert BCD (Binary-Coded Decimal) to binary.
 *
 * This function converts a BCD value to its binary equivalent.
 *
 * @param n_bcd The BCD value to be converted.
 * @return The binary equivalent of the BCD value.
 */
uint8_t(bcd_to_binary)(uint8_t n_bcd) {
  return ((n_bcd >> 4) * 10) + (n_bcd & 0x0F);
}

/**
 * @brief Function to check if RTC is in binary mode.
 *
 * This function checks if the RTC is in binary mode by reading the RTC control register.
 *
 * @return 1 if RTC is in binary mode, 0 if it's in BCD mode.
 */
int(rtc_binary)() {
  uint8_t ctrl;
  if (rtc_read_conf(RTC_REG_CTRL, &ctrl) != 0) {
    return 0;
  }

  if (BINARY & ctrl) {
    return 1;
  }

  return 0;
}

/**
 * @brief Function to check if RTC is in BCD mode.
 *
 * This function checks if the RTC is in BCD mode by calling rtc_binary() and negating the result.
 *
 * @return 1 if RTC is in BCD mode, 0 if it's in binary mode.
 */
int(rtc_bcd)() {
  return !rtc_binary();
}

/**
 * @brief Function to check if RTC is updating.
 *
 * This function checks if the RTC is currently updating by reading the RTC status register.
 *
 * @return 1 if RTC is updating, 0 otherwise.
 */
int(rtc_update)() {
  uint8_t ans;
  if (rtc_read_conf(UPDATING, &ans) != 0) {
    return 1;
  }
  if (UPDATING & ans) {
    return 1;
  }

  return 0;
}

/**
 * @brief Function to read RTC configuration register.
 *
 * This function reads the value of a specific RTC configuration register.
 *
 * @param cmd The command specifying which register to read.
 * @param ans Pointer to store the read value.
 * @return 0 on success, 1 on error.
 */
int(rtc_read_conf)(uint8_t cmd, uint8_t *ans) {
  if (sys_outb(RTC_ADDR_REG, cmd) != 0) {
    return 1;
  }

  if (aux_sys_inb(RTC_DATA_REG, ans) != 0) {
    return 1;
  }

  return 0;
}

/**
 * @brief Function to read current time from RTC.
 *
 * This function reads the current time from the RTC and stores it in the info structure.
 *
 * @return 0 on success, 1 on error.
 */
int(rtc_time)() {
  if (rtc_update() != 0) {
    return 1;
  }

  uint8_t ctrl;

  if (rtc_read_conf(RTC_REG_SEC, &ctrl) != 0) {
    return 1;
  }
  if (rtc_binary()) {
    info.seconds = ctrl;
  }
  else {
    info.seconds = bcd_to_binary(ctrl);
  }

  if (rtc_read_conf(RTC_REG_MIN, &ctrl) != 0) {
    return 1;
  }
  if (rtc_binary()) {
    info.minutes = ctrl;
  }
  else {
    info.minutes = bcd_to_binary(ctrl);
  }

  if (rtc_read_conf(RTC_REG_HOUR, &ctrl) != 0) {
    return 1;
  }
  if (rtc_binary()) {
    info.hours = ctrl;
  }
  else {
    info.hours = bcd_to_binary(ctrl);
  }

  if (rtc_read_conf(RTC_REG_DAY, &ctrl) != 0) {
    return 1;
  }
  if (rtc_binary()) {
    info.day = ctrl;
  }
  else {
    info.day = bcd_to_binary(ctrl);
  }

  if (rtc_read_conf(RTC_REG_MONTH, &ctrl) != 0) {
    return 1;
  }
  if (rtc_binary()) {
    info.month = ctrl;
  }
  else {
    info.month = bcd_to_binary(ctrl);
  }

  if (rtc_read_conf(RTC_REG_YEAR, &ctrl) != 0) {
    return 1;
  }
  if (rtc_binary()) {
    info.year = ctrl;
  }
  else {
    info.year = bcd_to_binary(ctrl);
  }

  return 0;
}

/**
 * @brief Function to initialize RTC.
 *
 * This function initializes the RTC by setting the binary flag and updating the time.
 *
 * @return 0 on success, 1 on error.
 */
int(rtc_init)() {
  binary = rtc_binary();
  rtc_update();
  return 0;
}

/**
 * @brief Function to check if it is daytime.
 *
 * This function checks if the current time is daytime (between 6:00 and 18:00).
 *
 * @return true if it is daytime, false otherwise.
 */
bool(isDaytime)() {
  rtc_time();
  uint8_t decimalHours = bcd_to_binary(info.hours);
  if (decimalHours >= 7 && decimalHours < 19) {
    return true;
  }
  return false;
}
