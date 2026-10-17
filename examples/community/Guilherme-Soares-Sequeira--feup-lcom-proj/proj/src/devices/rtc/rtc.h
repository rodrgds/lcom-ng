#ifndef RTC_H
#define RTC_H

/** @defgroup rtc rtc
 *  @{
 * 
 * Constants and functions used for the programming of the RTC
 */

#define RTC_IRQ 0x8 /**< @brief the IRQ line used for RTC interrupts */

#define RTC_ADDR_REG 0x70 /**< @brief the RTC register used for setting the RTC register to work with */
#define RTC_DATA_REG 0x71 /**< @brief the RTC register used for reading/writing data */

#define RTC_SECONDS_REG 0       /**< @brief the number of the RTC register that contains information about the seconds */
#define RTC_SECONDS_ALARM_REG 1 /**< @brief the number of the RTC register that contains information about the seconds portion of the alarm */
#define RTC_MINUTES_REG 2       /**< @brief the number of the RTC register that contains information about the minutes */
#define RTC_MINUTES_ALARM_REG 3 /**< @brief the number of the RTC register that contains information about the minutes portion of the alarm */
#define RTC_HOURS_REG 4         /**< @brief the number of the RTC register that contains information about the hours */
#define RTC_HOURS_ALARM_REG 5   /**< @brief the number of the RTC register that contains information about the hours portion of the alarm */

#define RTC_CTRL_REG_A 10 /**< @brief the number of the RTC control register A */
#define RTC_CTRL_REG_B 11 /**< @brief the number of the RTC control register B */
#define RTC_CTRL_REG_C 12 /**< @brief the number of the RTC control register C */
#define RTC_CTRL_REG_D 13 /**< @brief the number of the RTC control register D */

#define ENABLE_UPDATE_INTERRUPT BIT(4) /**< @brief mask for the bit used to enable Update-Ended interrupts */
#define BINARY_COUNTING_MODE BIT(2)    /**< @brief mask for the bit used to set the RTC counters in binary counting mode */
#define HOUR_RANGE_24H BIT(1)          /**< @brief mask for the bit used to set the RTC counters hour range to 0-24 */
#define DST BIT(0)                     /**< @brief mask for the bit used to enable Daylight Savings TIme */

#define IQRF BIT(7)            /**< @brief mask for the bit used to check for IQRF (ctrl reg C) */
#define UPDATE_INT_FLAG BIT(4) /**< @brief mask for the bit used to check if a given interrupt is an Update-Ended Interrupt */

/**
 * @brief subscribes to RTC interrupts and puts the given bit in bit_no
 *
 * @param[in] bit_no Bit number of the RTC interrupt messages
 */
int(rtc_subscribe_int)(uint8_t *bit_no);

/**
 * @brief unsubscribes RTC interrupts
 */
int(rtc_unsubscribe_int)();

/**
 *  @brief interrupt handler for the RTC
 */
void(rtc_int_handler)();

/**
 *  @brief sets up the RTC with some default operation values
 */
void(setup_update_interrupts)();

/**
 *  @brief reads the value present in the given RTC register
 *
 *  @param reg_no the register number
 *
 *  @return the value present in the given RTC register
 */
uint8_t(read_from_reg)(uint8_t reg_no);

/**
 *  @brief writes the given value to the given RTC register
 *
 *  @param reg_no the register number
 *  @param data the data to write
 */
void(write_to_reg)(uint8_t reg_no, uint8_t data);

/**
 *  @brief get current seconds
 *
 *  @return the current RTC seconds
 */
unsigned short(rtc_get_seconds)();

/**
 *  @brief get current minutes
 *
 *  @return the current RTC minutes
 */
unsigned short(rtc_get_minutes)();

/**
 *  @brief get current hours
 *
 *  @return the current RTC hours
 */
unsigned short(rtc_get_hours)();

/**
 *  @brief get a formatted representation of the current time
 *
 *  @return the current RTC time in a formatted string
 */
char *(rtc_get_formatted_time) ();

/**@}*/

#endif
