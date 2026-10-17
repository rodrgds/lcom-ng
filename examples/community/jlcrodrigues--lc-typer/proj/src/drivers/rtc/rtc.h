#ifndef RTC_H
#define RTC_H

#include <lcom/lcf.h>

/** @defgroup driver rtc
 * @{
 *
 * Functions related to the real time clock.
 */

/* Registers and IRQ */
#define RTC_IRQ 8
#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

#define DAY_REG 0X7
#define MONTH_REG 0X8
#define YEAR_REG 0X9
#define SECOND_REG 0X0
#define MINUTE_REG 0X2
#define HOUR_REG 0X4

/* Register masks */

// Register A
#define UIP BIT(7)
#define RATE (BIT(0) | BIT(1) | BIT(2) | BIT(3))

// Register B
#define PIE BIT(6)
#define AIE BIT(5)
#define UIE BIT(4)

// Register C
#define IRQF BIT(7)
#define UF BIT(4)
#define PF BIT(6)

#define DATE_STR_SIZE 9
#define HOUR_STR_SIZE 9

/**
 * @brief Subscribe real time clock interrupts.
 * @param bit_no Receives the bit that will be set for rtc interrupts.
 * @return Returns 0 upon success, and 1 otherwise.
 */
int rtc_subscribe_int(uint8_t* bit_no);

/**
 * @brief Unsubscribe RTC interrupts.
 * @return Returns 0 upon success, and 1 otherwise.
 */
int rtc_unsubscribe_int(void);

/**
 * @brief Initializes the RTC. Call this function before starting to use it.
 * 
 * @return Returns 0 upon success, and 1 otherwise.
 */
int rtc_init(void);

/**
 * @brief Handle RTC interrupts.
 * For update interrupts, the data variables are updates and no
 * further action is needed.
 * 
 */
void rtc_ih(void);

/**
 * @brief Start a periodic counter with a rate of 500ms.
 * 
 * @return int Returns 0 upon success, non-zero otherwise.
 */
int rtc_start_counter(void);

/**
 * @brief Handle periodic interrupts.
 * 
 */
void rtc_handle_period(void);

/**
 * @brief Retrieve time elapsed, in seconds, since the counter was started.
 * 
 * @return int Time in seconds since rtc_start_counter was called.
 */
int rtc_get_time_elapsed(void);

/**
 * @brief Update the date variables from the rtc registers.
 * 
 * @return int 0 upon success, non-zero otherwise.
 */
int rtc_read_date_reg(void);

/**
 * @brief Update the hour variables from the rtc registers.
 * 
 * @return int 0 upon success, non-zero otherwise.
 */
int rtc_read_hour_reg(void);

/**
 * @brief Get the current date from the RTC.
 * 
 * @return char* Current date in dd/mm/yy string format.
 */
char* rtc_get_date_str(void);

/**
 * @brief Get the current hour from the RTC.
 * 
 * @return char* Current hour in hh:mm:ss string format.
 */
char* rtc_get_hour_str(void);

/**
 * @brief Read an rtc register.
 * 
 * @param reg Register number. From 0xA to 0xD.
 * @param byte Pointer variable to store read value.
 * @return int 0 upon success, non-zero otherwise.
 */
int (rtc_read_register)(uint8_t reg, uint8_t* byte);

/**
 * @brief Set an rtc register.
 * 
 * @param reg Register number. From 0xA to 0xD.
 * @param byte Value to be written.
 * @return int 0 upon success, non-zero otherwise.
 */
int (rtc_write_register)(uint8_t reg, uint8_t byte);

/**@}*/

#endif
