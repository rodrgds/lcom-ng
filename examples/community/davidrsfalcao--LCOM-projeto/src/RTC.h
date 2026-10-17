#ifndef RTC_H
#define RTC_H
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8042.h"
#include "i8254.h"

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71
#define SECONDS 0x00
#define MINUTES 0x02
#define HOUR 0x04
#define DAY 0x07
#define MONTH 0x08
#define YEAR 0x09
#define REG_A 0x0A
#define REG_B 0x0B
#define REG_C 0x0C
/**
 * @brief turns a binary number into a decimal one
 * @param bcd the number that will be transform
 * @return the number,now on a decimal base
 */
int decimal (unsigned long bcd);
/**
 * @brief Subscribes and enables RTC's interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int RTC_subscribe_int();
/**
 * @brief Unsubscribes RTC's interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int RTC_unsubscribe_int();
/**
 * @brief sets the RTC's interrupts to a 24h format
 */
void Set_RTC();
/**
 * @breif cleans the buffer from REG_C
 */
void clean_buffer();
/**
 * @return the seconds of the actual date
 */
int segundos();
/**
 * @return the day of the actual date
 */
int dia();
/**
 * @return the month of the actual date
 */
int mes();
/**
 * @return the year of the actual date
 */
int ano();
/**
 * @return the hour of the actual date
 */
int horas();
/**
 * @return the minutes of the actual date
 */
int minutos();

#endif
