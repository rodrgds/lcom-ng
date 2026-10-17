#pragma once

#define RTC_SEC	0
#define RTC_MIN	2
#define RTC_HOUR 4
#define RTC_DAY 7
#define RTC_MONTH 8
#define RTC_YEAR 9
#define REGA 10
#define REGB 11
#define REGC 12
#define REGD 13
#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71
#define RTC_REGA_UIP BIT(7)
#define RTC_REGB_DATAMODE BIT(2)

/**
 * @brief Converts values from bcd to binary.
 * @param bcd Value in bcd to be converted to binary
 */
void BCDtoBinary(unsigned long *bcd);

/**
 * @brief Gets the current date from the RTC in binary.
 * @param day Will be updated by the RTC
 * @param month Will be updated by the RTC
 * @param year Will be updated by the RTC
 * @param hour Will be updated by the RTC
 * @param minute Will be updated by the RTC
 * @param seconds Will be updated by the RTC
 */
void getDate(unsigned long *day, unsigned long *month, unsigned long *year, unsigned long *hour, unsigned long* minute, unsigned long* seconds);


