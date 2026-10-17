#ifndef __RTC_H__
#define __RTC_H__


#include <stdint.h>
#include "../graphics/graphics.h"

// RTC I/O Ports and IRQ Line
#define RTC_IRQ 8       /** @brief RTC IRQ Line on IRQ0 from PIC2 */
#define RTC_ADDR_REG 0x70   /** @brief RTC Address Register Port */
#define RTC_DATA_REG 0x71   /** @brief RTC Data Register Port */

// RTC Register Addresses
#define SECS 0x00   /** @brief Seconds register */
#define SECS_ALARM 0x01   /** @brief Seconds alarm register */
#define MINS 0x02   /** @brief Minutes register */
#define MINS_ALARM 0x03   /** @brief Minutes alarm register */
#define HOURS 0x04   /** @brief Hours register */
#define HOURS_ALARM 0x05   /** @brief Hours alarm register */
#define DAY_WEEK 0x06   /** @brief Day of the week register */
#define DAY_MONTH 0x07   /** @brief Day of the month register */
#define MONTH 0x08   /** @brief Month register */
#define YEAR 0x09   /** @brief Year register */

// Register A (10)
#define REG_A 0x0A   /** @brief Register A address */
#define UIP BIT(7)   /** @brief Update In Progress (UIP) flag bit */
#define DV2 BIT(6)   /** @brief Divisor 2 bit */
#define DV1 BIT(5)   /** @brief Divisor 1 bit */
#define DV0 BIT(4)   /** @brief Divisor 0 bit */
#define RS3 BIT(3)   /** @brief Rate Selector 3 bit */
#define RS2 BIT(2)   /** @brief Rate Selector 2 bit */
#define RS1 BIT(1)   /** @brief Rate Selector 1 bit */
#define RS0 BIT(0)   /** @brief Rate Selector 0 bit */

// Register B (11)
#define REG_B 0x0B   /** @brief Register B address */
#define SET BIT(7)   /** @brief Set to inhibit updates bit */
#define PIE BIT(6)   /** @brief Periodic Interrupt Enable (PIE) bit */
#define AIE BIT(5)   /** @brief Alarm Interrupt Enable (AIE) bit */
#define UIE BIT(4)   /** @brief Update Interrupt Enable (UIE) bit */
#define SQWE BIT(3)   /** @brief Square Wave Enable (SQWE) bit */
#define DM BIT(2)   /** @brief Data Mode (1 - Binary, 0 - BCD) bit */
#define HR BIT(1)   /** @brief Set to 24-hour mode bit */
#define DSE BIT(0)   /** @brief Daylight Savings Enable (DSE) bit */

// Register C (12)
#define REG_C 0x0C   /** @brief Register C address */
#define IRQF BIT(7)   /** @brief IRQ Active (IRQF) bit */
#define PF BIT(6)   /** @brief Periodic Interrupt Pending (PF) bit */
#define AF BIT(5)   /** @brief Alarm Interrupt Pending (AF) bit */
#define UF BIT(4)   /** @brief Update Interrupt Pending (UF) bit */

// Register D (13)
#define REG_D 0x0D   /** @brief Register D address */
#define VRT BIT(7)   /** @brief Valid Time (VRT) bit */

/**
 * @brief Initializes the Real Time Clock (RTC) 
 *
 * @return int 1 if an error occurs, 0 otherwise
 */
int rtc_init();

/**
 * @brief Subscribes to RTC
 *
 *
 * @param bit_no Variable to store the bit mask for the RTC's IRQ line.
 * @return int 1 if an error occurs, 0 otherwise
 */
int rtc_subscribe_int(uint8_t *bit_no);

/**
 * @brief Unsubscribes from RTC interrupts.
 *
 * @return int 1 if an error occurs, 0 otherwise
 */
int rtc_unsubscribe_int();

/**
 * @brief Reads a value from a RTC register.
 *
 * @param reg The register address to read from.
 * @param data Variable to store the read value.
 * @return int 1 if an error occurs, 0 otherwise
 */
int rtc_read_reg(uint8_t reg, uint8_t *data);

/**
 * @brief Writes a value to a RTC register.
 *
 * @param reg The register address to write to.
 * @param data The value to write to the register.
 * @return int 1 if an error occurs, 0 otherwise
 */
int rtc_write_reg(uint8_t reg, uint8_t data);

/**
 * @brief Reads the hours from the RTC.
 *
 * @param hours Variable to store the read hours.
 * @return int 1 if an error occurs, 0 otherwise
 */
int read_hours(uint8_t *hours);

/**
 * @brief Reads the minutes from the RTC.
 *
 * @param minutes Variable to store the read minutes.
 * @return int 1 if an error occurs, 0 otherwise
 */
int read_minutes(uint8_t *minutes);

/**
 * @brief Reads the seconds from the RTC.
 *
 * @param seconds Variable to store the read seconds.
 * @return int 1 if an error occurs, 0 otherwise
 */
int read_seconds(uint8_t *seconds);

/**
 * @brief Reads the day from the RTC.
 *
 * @param day Variable to store the read day.
 * @return int 1 if an error occurs, 0 otherwise
 */
int read_day(uint8_t *day);

/**
 * @brief Reads the month from the RTC.
 *
 * @param month Variable to store the read month.
 * @return int 1 if an error occurs, 0 otherwise
 */
int read_month(uint8_t *month);

/**
 * @brief Reads the year from the RTC.
 *
 * @param year Variable to store the read year.
 * @return int 1 if an error occurs, 0 otherwise
 */
int read_year(uint8_t *year);

/**
 * @brief Reads the date (day, month, and year) from the RTC.
 *
 * @param day Variable to store the read day.
 * @param month Variable to store the read month.
 * @param year Variable to store the read year.
 * @return int 1 if an error occurs, 0 otherwise
 */
int read_date(uint8_t *day, uint8_t *month, uint8_t *year);

/**
 * @brief Reads the time (hours, minutes, and seconds) from the RTC.
 *
 * @param hours Variable to store the read hours.
 * @param minutes Variable to store the read minutes.
 * @param seconds Variable to store the read seconds.
 * @return int 1 if an error occurs, 0 otherwise
 */
int read_time(uint8_t *hours, uint8_t *minutes, uint8_t *seconds);

/**
 * @brief Draws the current time on the screen.
 *
 * @return int 1 if an error occurs, 0 otherwise
 */
int draw_time();

/**
 * @brief Draws the current date on the screen.
 *
 *
 * @return int 1 if an error occurs, 0 otherwise
 */
int draw_date();



#endif /* __RTC_H__ */
