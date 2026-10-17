#ifndef _RTC_H
#define _RTC_H

/** @file rtc.h
 * @brief file containing the constants and functions for the RTC controller
 */

/** @defgroup rtc rtc
 * @{
 *
 * @brief Constants and functions for programming the RTC.
 */

#define BIT(n) 			(0x01<<(n))

#define RTC_IRQ			8
#define RTC_HOOK_ID		8

/* ports */
#define RTC_ADDR_REG	0x70
#define RTC_DATA_REG	0x71

/* registers */
#define RTC_SECONDS		0
#define RTC_SECS_ALARM	1
#define RTC_MINUTES		2
#define RTC_MINS_ALARM	3
#define RTC_HOURS		4
#define RTC_HORS_ALARM	5
#define RTC_WEEK_DAY	6
#define RTC_MONTH_DAY	7
#define RTC_MONTH		8
#define RTC_YEAR		9
#define RTC_REGISTER_A	10
#define RTC_REGISTER_B	11
#define RTC_REGISTER_C	12
#define RTC_REGISTER_D	13

/*bits of register A*/
#define UIP_REGISTER_A    BIT(7)
#define DV2_REGISTER_A    BIT(6)
#define DV1_REGISTER_A    BIT(5)
#define DV0_REGISTER_A    BIT(4)
#define RS3_REGISTER_A    BIT(3)
#define RS2_REGISTER_A    BIT(2)
#define RS1_REGISTER_A    BIT(1)
#define RS0_REGISTER_A    BIT(0)

/**@{
 * @brief DateTime info
 */
typedef struct {
	unsigned short seconds;			/**< @brief seconds of the current time */
	unsigned short minutes;			/**< @brief minutes of the current time */
	unsigned short hours;			/**< @brief hours of the current time */
	unsigned short day;				/**< @brief day of the current date */
	unsigned short month;			/**< @brief month of the current date */
	unsigned short year;			/**< @brief year of the current date */
} DateTime;
/** @}*/

/**@{
 * @brief RTC info
 */
typedef struct {
	unsigned long registers[10];	/**< @brief rtc register contents */
	DateTime *datetime;				/**< @brief rtc current date and time */
} RTC;
/** @}*/

/**
 * @brief creates a new DateTime structure
 * @return pointer to the newly created DateTime
 */
DateTime *create_datetime();

/**
 * @brief destroys a previously created datetime
 * @param datetime to be destroyed
 */
void destroy_datetime(DateTime *datetime);

/**
 * @brief creates a new RTC structure
 * @return pointer to the created RTC
 */
RTC *create_rtc();

/**
 * @brief destroys a previously created RTC
 * @param rtc to be destroyed
 */
void destroy_rtc(RTC *rtc);

/**
 * @brief obtains the RTC data from its registers
 * @param rtc whose date we want to obtain
 */
void get_data_rtc(RTC *rtc);

/**@}*/

#endif /*_RTC_H*/
