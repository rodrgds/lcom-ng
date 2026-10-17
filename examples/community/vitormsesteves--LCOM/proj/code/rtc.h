#pragma once

/** @defgroup rtc rtc
 * @{
 * Functions for rtc device
 */

#include "i8042.h"

#define RTC_ADDR_REG        0x70 /**< @brief RTC address register */
#define RTC_DATA_REG        0x71 /**< @brief RTC data register */

#define RTC_SECONDS         0x0 /**< @brief Seconds */
#define RTC_SECONDS_ALARM   0x1 /**< @brief Seconds alarm */
#define RTC_MINUTES         0x2 /**< @brief Minutes */
#define RTC_MINUTES_ALARM   0x3 /**< @brief Minutes alarm */
#define RTC_HOURS           0x4 /**< @brief Hours */
#define RTC_HOURS_ALARM     0x5 /**< @brief Hours alarm */
#define RTC_DAY             0x7 /**< @brief Day */
#define RTC_MONTH           0x8 /**< @brief Month */
#define RTC_YEAR            0x9 /**< @brief Year */
#define RTC_REG_A           0xA /**< @brief Register_A */
#define RTC_REG_B           0xB /**< @brief Register_B */
#define RTC_REG_C           0xC /**< @brief Register_C */
#define RTC_REG_D           0xD /**< @brief Register_D */

/* REGISTER_A BITS */

#define RTC_RS0_BIT         BIT(0) /**< @brief RS0 - rate selector */
#define RTC_RS1_BIT         BIT(1) /**< @brief RS1 - rate selector */
#define RTC_RS2_BIT         BIT(2) /**< @brief RS2 - rate selector */
#define RTC_RS3_BIT         BIT(3) /**< @brief RS3 - rate selector */
#define RTC_DV0_BIT         BIT(4) /**< @brief DV0 - control the counting chain */
#define RTC_DV1_BIT         BIT(5) /**< @brief DV1 - control the counting chain */
#define RTC_DV2_BIT         BIT(6) /**< @brief DV2 - control the counting chain */
#define RTC_UIP             BIT(7) /**< @brief UIP - update in progress */

/* REGISTER_B BITS */

#define RTC_DSE                 BIT(0) /**< @brief DSE - enable daylight savings time */
#define RTC_MODE       			BIT(1) /**< @brief MODE - 24_12 */
#define RTC_DM         	 		BIT(2) /**< @brief DM - Binary_BCD */
#define RTC_SQWE                BIT(3) /**< @brief SQWE - enable square-wave generation  */
#define RTC_UIE                 BIT(4) /**< @brief UIE - interrupt source */
#define RTC_AIE                 BIT(5) /**< @brief AIE - interrupt source */
#define RTC_PIE                 BIT(6) /**< @brief PIE - interrupt source */
#define RTC_SET                 BIT(7) /**< @brief SET -  inhibit updates */

/* REGISTER_C BITS */

#define RTC_UF          BIT(4) /**< @brief UF - Update interrupt pending */
#define RTC_AF          BIT(5) /**< @brief AF - Alarm interrupt pending */
#define RTC_PF          BIT(6) /**< @brief PF - Periodic interrupt pending */
#define RTC_IRQF        BIT(7) /**< @brief IRQF - IRQ line active */

/* REGISTER_D BITS */

#define RTC_VRT_BIT         BIT(7) /**< @brief VRT - Valid RAM/time */



#define RTC_IRQ             8 /**< @brief RTC IRQ  */

typedef struct {

	unsigned long seconds; // specifies the seconds
	unsigned long minutes; // specifies the minutes
	unsigned long hours; // specifies the hours

	unsigned long day; // specifies the day
	unsigned long month; // specifies the month
	unsigned long year; // specifies the year

} rtc_time_t;

/**
 * @brief Check alarm in register C
 *
 * @return Validation
 */
int rtc_check();
/**
 * @brief Write a alarm
 *
 * @param s Time to alarm in seconds
 */
void rtc_write_alarm(unsigned short s);
/**
 * @brief Enable alarm
 *
 */
void rtc_enable();
/**
 * @brief Disable alarm
 *
 */
void rtc_disable();
/**
 * @brief Subscribe RTC interrupt
 *
 * @return Validation
 */
int rtc_subscribe();
/**
 * @brief Unsubscribe RTC interrupt
 *
 */
void rtc_unsubscribe();

/**
 * @brief Current Date
 *
 * @return Current date;
 */
rtc_time_t rtc_current_date();
