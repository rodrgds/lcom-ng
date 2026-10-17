#include <lcom/lcf.h>

/**
 * @brief port loaded with the address of the RTC register to be accessed
 * 
 */
#define RTC_ADDR_REG    0x70

/**
 * @brief port used to transfer the data to/from the RTC’s register accessed
 * 
 */
#define RTC_DATA_REG    0x71

/**
 * @brief Converts Binary-coded decimal to decimal
 * 
 * @param hex Binary-coded decimal
 * @return int Result of convertion
 */
int bcd_decimal(uint8_t hex);

/**
 * @brief Gets the day of current date
 * 
 * @return int Current day
 */
int getDayRTC();

/**
 * @brief Gets the month of current date
 * 
 * @return int Current month
 */
int getMonthRTC();

/**
 * @brief Gets the year of current date
 * 
 * @return int Current year
 */
int getYearRTC();

/**
 * @brief Gets the seconds of current time
 * 
 * @return int Current seconds
 */
int getSecondsRTC();

/**
 * @brief Gets the minutes of current time
 * 
 * @return int Current minutes
 */
int getMinutesRTC();

/**
 * @brief Gets the hours of current time
 * 
 * @return int Current hours
 */
int getHoursRTC();
