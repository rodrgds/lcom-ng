#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "rtc.h"

int rtc_hook_id = RTC_HOOK_BIT;
time_info rtc_info;
uint8_t binary_mode;

void (rtc_setup)() {
  uint8_t reg_A;
  uint8_t reg_B;

  uint8_t tries = 10;
  do {

    if(rtc_read_register(STAT_REG_A, &reg_A))
      return;
    
    tries--;
    if(!tries) return;
  
  } while(reg_A & UIP);

  if(rtc_read_register(STAT_REG_B, &reg_B))
    return;

  binary_mode = reg_B & BINARY;

  rtc_read_time();
}

int (rtc_subscribe_int)() {
  if(sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE, &rtc_hook_id)) 
    return 1;
  return 0;
}

int (rtc_unsubscribe_int)() {
  if(sys_irqrmpolicy(&rtc_hook_id))
    return 1;
  return 0;
}

int (rtc_read_register)(uint8_t port, uint8_t *data) {
  if (sys_outb(SELECT_REG, port))
    return 1;
  if (util_sys_inb(REG_OUTPUT, data))
    return 1;
  return 0;
}

uint8_t (to_binary)(uint8_t bcd_number) {
  return ((bcd_number >> 4) * 10) + (bcd_number & 0xF);
}

int (rtc_read_time)() {

  uint8_t res;

  if (rtc_read_register(SECONDS, &res))
    return 1;
  rtc_info.seconds = binary_mode ? res : to_binary(res);

  if (rtc_read_register(MINUTES, &res))
    return 1;
  rtc_info.minutes = binary_mode ? res : to_binary(res);

  if (rtc_read_register(HOURS, &res))
    return 1;
  rtc_info.hours = binary_mode ? res : to_binary(res);

  if (rtc_read_register(DAY, &res))
    return 1;
  rtc_info.day = binary_mode ? res : to_binary(res);

  if (rtc_read_register(MONTH, &res))
    return 1;
  rtc_info.month = binary_mode ? res : to_binary(res);

  if (rtc_read_register(YEAR, &res))
    return 1;
  rtc_info.year = binary_mode ? res : to_binary(res);

  return 0;
}

// Function to check if a year is a leap year
bool (is_leap_year)(uint16_t year) {
  if (year % 4 == 0) {
    if (year % 100 == 0) {
      if (year % 400 == 0) return true;
      else return false;
    } else return true;
  } else return false;
}

// Function to get the number of days in a month
uint8_t (get_days_in_month)(uint8_t month, uint16_t year) {
  switch (month) {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
      return 31;
    case 4: case 6: case 9: case 11:
      return 30;
    case 2:
      return is_leap_year(year) ? 29 : 28;
    default:
      return 0; // Invalid month
  }
}

void (rtc_increment_time_by_one_second)() {
  rtc_info.seconds++;
  if (rtc_info.seconds == 60) {
    rtc_info.seconds = 0;
    rtc_info.minutes++;
    if (rtc_info.minutes == 60) {
      rtc_info.minutes = 0;
      rtc_info.hours++;
      if (rtc_info.hours == 24) {
        rtc_info.hours = 0;
        rtc_info.day++;
        uint8_t days_in_month = get_days_in_month(rtc_info.month, rtc_info.year);
        if (rtc_info.day > days_in_month) {
          rtc_info.day = 1;
          rtc_info.month++;
          if (rtc_info.month == 13) {
            rtc_info.month = 1;
            rtc_info.year++;
          }
        }
      }
    }
  }
}

void (increment_time_by_one_day)(time_info *date_time) {
    // Get the number of days in the current month
    uint8_t days_in_current_month = get_days_in_month(date_time->month, date_time->year);

    // Increment the day
    date_time->day++;

    // Check if the day exceeds the number of days in the current month
    if (date_time->day > days_in_current_month) {
        date_time->day = 1; // Reset the day to 1

        // Increment the month
        date_time->month++;

        // Check if the month exceeds 12 (December)
        if (date_time->month > 12) {
            date_time->month = 1; // Reset the month to January

            // Increment the year
            date_time->year++;
        }
    }
}

char* (time_left_to_target_time)(time_info target_time) {
    int seconds_diff = target_time.seconds - rtc_info.seconds;
    int minutes_diff = target_time.minutes - rtc_info.minutes;
    int hours_diff = target_time.hours - rtc_info.hours;
    int days_diff = target_time.day - rtc_info.day;
    int months_diff = target_time.month - rtc_info.month;
    int years_diff = target_time.year - rtc_info.year;

    if (seconds_diff < 0) {
        seconds_diff += 60;
        minutes_diff--;
    }
    if (minutes_diff < 0) {
        minutes_diff += 60;
        hours_diff--;
    }
    if (hours_diff < 0) {
        hours_diff += 24;
        days_diff--;
    }
    if (days_diff < 0) {
        if (months_diff <= 0) {
            years_diff--;
            months_diff += 12;
        }
        months_diff--;
        days_diff += get_days_in_month((rtc_info.month + months_diff) % 12, rtc_info.year + years_diff);
    }
    if (months_diff < 0) {
        months_diff += 12;
        years_diff--;
    }

    // Check if the time left is zero or negative
    if (years_diff < 0 || (years_diff == 0 && months_diff == 0 && days_diff == 0 && hours_diff == 0 && minutes_diff == 0 && seconds_diff <= 0)) {
        return "Get reward";
    }

    // Allocate space for the resulting string
    char *time_left_str = malloc(100);

    // Return the number of days if the difference is greater than 2 days
    if (years_diff > 0 || months_diff > 0 || days_diff > 2) {
        snprintf(time_left_str, 100, "%d days", years_diff * 365 + months_diff * 30 + days_diff); // Approximate conversion
    } else {
        // Otherwise, return the time left in hours:minutes:seconds
        snprintf(time_left_str, 100, "%02d:%02d:%02d", hours_diff, minutes_diff, seconds_diff);
    }

    return time_left_str;
}
