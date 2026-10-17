//header file

#ifndef __RTC_H
#define __RTC_H

/**
 * @brief Checks if the RTC time is updating
 * @return @return 1 if updates have ocurred, 0 otherwise
 */
int (isUpdating)();

/**
 * @brief Checks if RTC data is in Binary Coded Decimal (BCD)
 * @return 0 if no errors ocurred, 1 otherwise
 */
int (isBCD)();

uint32_t (convertDecimal)(uint32_t* bcd);

/**
 * @brief Gets the Date from the RTC
 * @param day day from the RTC will be stored here
 * @param month month from the RTC will be stored here
 * @param year year from the RTC will be stored here
 * @param hour hour from the RTC will be stored here
 * @param min minutes from the RTC will be stored here
 * @param sec seconds from the RTC will be stored here
 * @return 0 if no errors ocurred, 1 otherwise
 */
int getDate(uint32_t *day, uint32_t *month, uint32_t *year, uint32_t *hour, uint32_t *min, uint32_t *sec);

#endif /* __RTC_H */
