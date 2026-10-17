#ifndef __RTC_H
#define __RTC_H

/** @defgroup rtc RTC
 *  @{
 *
 *  RTC related functions
 */

/** @brief Get bit in n position. */
#define BIT(n) (0x01<<(n))

/** @brief RTC's IRQ */
#define RTC_IRQ				8

/** @brief RTC's add register */
#define RTC_ADDR_REG		0x70

/** @brief RTC's data register */
#define RTC_DATA_REG		0x71

/** @brief RTC's seconds register */
#define RTC_REG_SECONDS		0

/** @brief RTC's seconds register (alarm) */
#define RTC_REG_SECONDS_A 	1

/** @brief RTC's minutes register */
#define RTC_REG_MINUTES		2

/** @brief RTC's minutes register (alarm) */
#define RTC_REG_MINUTES_A	3

/** @brief RTC's hours register */
#define RTC_REG_HOURS		4

/** @brief RTC's hours register (alarm) */
#define RTC_REG_HOURS_A		5

/** @brief RTC's day of the week register */
#define RTC_REG_DAY_WEEK	6

/** @brief RTC's day register */
#define RTC_REG_DAY			7

/** @brief RTC's month register */
#define RTC_REG_MONTH		8

/** @brief RTC's year register */
#define RTC_REG_YEAR		9

/** @brief RTC's A register */
#define RTC_REG_A			10

/** @brief RTC's B register */
#define RTC_REG_B			11

/** @brief RTC's C register */
#define RTC_REG_C			12

/** @brief RTC's D register */
#define RTC_REG_D			13

/** @brief RTC's update interrupt pending */
#define RTC_UF 				BIT(4)

/** @brief RTC's alarm interrupt pending */
#define RTC_AF 				BIT(5)

/** @brief RTC's periodic interrupt pending */
#define RTC_PF 				BIT(6)

/** RTC is an "object" that contains all the data concerning the time.
 *  It keeps track of the hours, minutes, seconds, days, months and years.
 */
typedef struct {
	unsigned long y;	/**< Year */
	unsigned long m;	/**< Month */
	unsigned long d;	/**< Day */

	unsigned long h;	/**< Hour */
	unsigned long min;	/**< Minute */
	unsigned long s;	/**< Second */

} rtc_t;

/**
 *  @brief Subscribes RTC's interruptions.
 *  @return Returns an ID if successful and -1 otherwise.
 */
int rtc_subscribe_int();

/**
 *  @brief Unsubscribes RTC's interruptions.
 *  @return Returns 0 if successful and 1 otherwise.
 */
int rtc_unsubscribe_int();

/**
 *  @brief RTC's interruption handler.
 *  @return Returns 0 if successful and 1 otherwise.
 */
int rtc_int_handler();

/**
 *  @brief Enables update interruptions.
 */
void enable_update_int();

/**
 *  @brief Reads from a register.
 *  @param reg Register to read from.
 *  @return Returns the response from the register.
 */
unsigned long rtc_readRegister(unsigned long reg);

/**
 *  @brief Writes to a register.
 *  @param reg Register to write to.
 *  @param param Parameter to write.
 */
void rtc_writeRegister(unsigned long reg, unsigned long param);

/**
 *  @brief Handles an update interruption.
 */
void handle_update_int();

/** @brief Checks the current RTC's status.
 *  @return Returns the current RTC's status.
 */
rtc_t checkClock();

/**
 *  @brief Converts from BCD to binary.
 *  @param bcd BCD to convert.
 *  @return Returns BCD in binary.
 */
unsigned long convertBinary(unsigned long bcd);

/** @} end of rtc */

#endif
