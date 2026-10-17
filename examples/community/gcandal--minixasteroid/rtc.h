#ifndef RTC_H_
#define RTC_H_

/** @defgroup rtc rtc
 * @{
 *
 * Functions for using rtc
 */

#define RTC_IRQ				8
#define RTC_ADDR_REG 		0x70
#define RTC_DATA_REF		0x71

#define SECONDS				0x00
#define SECONDS_ALARM		0x01
#define MINUTES				0x02
#define MINUTES_ALARM		0x03
#define HOURS				0x04
#define HOURS_ALARM			0x05
#define DAY_OF_THE_WEEK		0x06
#define DAY_OF_THE_MONTH	0x07
#define MONTH				0x08
#define YEAR				0x09
#define REG_B				0x0B
#define REG_C				0x0C

#define BIT(n) (0x01<<(n))

static int rtc_hook=4;

/**
 * @brief Subscribes rtc interrupts
 *
 * @return Return 0 upon success
 */
int rtc_subscribe_int();
/**
 * @brief Unsubscribes rtc interrupts
 *
 * @return Return 0 upon success
 */
int rtc_unsubscribe_int();
/**
 * @brief Converts binary coded decimal to decimal
 *
 * @param bcd Number in binary coded decimal
 *
 * @return Return number in decimal
 */
static int bcd_to_decimal(char bcd);
/**
 * @brief Converts decimal to binary coded decimal
 *
 * @param bcd Number in decimal
 *
 * @return Return number in binary coded decimal
 */
static char decimal_to_bcd(int decimal);
/**
 * @brief Interprets rtc interrupt
 *
 * @param readtime Defines if the alarm is set or not
 * @param timeslarm Pointer to the alarm structure
 * @param DELTA Time interval to program the alarm
 * @return Return 1 uppon success
 */
int rtc_int_handler(unsigned short readtime, unsigned short* timesalarm, int DELTA);
static void readTime(unsigned short *time, char port, unsigned long *stat);
static void choosePort(char port);
static void readPort(unsigned long *stat);
static void writePort(unsigned long stat);

/**@}*/

#endif /* RTC_H_ */
