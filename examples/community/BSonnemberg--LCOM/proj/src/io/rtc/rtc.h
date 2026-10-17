#pragma once

#include <lcom/lcf.h>
#include <stdint.h>

#define RTC_IRQ 8

#define RTC_REG_A 10
#define RTC_REG_B 11

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

typedef struct {
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} date_t;

/**
 * @brief Read contents of an RTC register
 * @param reg register to be read
 * @param r placeholder for output result
 * @return 0 if successful, non-0 otherwise
*/
int rtc_read(uint8_t reg, uint8_t *r);

/**
 * @brief Update current date & time
 * @return 0 if successful, non-0 otherwise
*/
int update_datetime();

/**
 * Convert a BCD to a binary number
 * @param bcd bcd to be converted
 * @return result of conversion
*/
uint8_t to_binary(uint8_t bcd);
