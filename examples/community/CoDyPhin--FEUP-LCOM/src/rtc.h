#ifndef SOURCE_CODE_RTC_H
#define SOURCE_CODE_RTC_H

#endif //SOURCE_CODE_RTC_H

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

/** @defgroup rtc RTC
 * @{
 * RTC functions
 */

typedef struct {
    uint32_t day; /**< current day */
    uint32_t month; /**< current month */
    uint32_t  year; /**< current year */
    uint32_t hour; /**< current hour */
    uint32_t  min; /**< current minutes */
    uint32_t sec; /**< current seconds */
} time_date;
/**
 * @brief Checks if the RTC has been updated
 * @return True if it has, False if not
 */
bool (checkUpdate)();

/**
 * @brief Converts to decimal values
 * @param no BCD value
 * @return Decimal value
 */
int bcd_to_dec(uint32_t no);

/**
 * @brief Updates the date and time
 * @param data Struct pointer to the RTC
 */
void (readData)(time_date* data);

/**
 * @@brief Gets the current RTC
 * @return Struct with the RTC
 */
time_date (getData)();
/** @} end of RTC */

