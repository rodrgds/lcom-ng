#ifndef RTC_MACROS
#define RTC_MACROS

/** @brief Address register for accessing RTC. */
#define RTC_ADDR_REG 0x70

/** @brief Data register for accessing RTC. */
#define RTC_DATA_REG 0x71

/** @brief IRQ associated with RTC. */
#define RTC_IRQ 8

/** @brief RTC register addresses. */
#define RTC_SECONDS            0
#define RTC_SECONDS_ALARM      1
#define RTC_MINUTES            2
#define RTC_MINUTES_ALARM      3
#define RTC_HOURS              4
#define RTC_HOURS_ALARM        5
#define RTC_DAY_OF_WEEK        6
#define RTC_DAY_OF_MONTH       7
#define RTC_MONTH              8
#define RTC_YEAR               9
#define RTC_REGISTER_A        10
#define RTC_REGISTER_B        11
#define RTC_REGISTER_C        12
#define RTC_REGISTER_D        13

#endif
