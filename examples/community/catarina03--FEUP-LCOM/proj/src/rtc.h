#include <lcom/lcf.h>
#pragma once
#include "game_structs.h"

//Macros

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71
#define SEC_REG 0
#define MIN_REG 2
#define HOUR_REG 4
#define DAY_REG 7
#define MONTH_REG 8
#define YEAR_REG 9
#define REG_A 0x0A
#define REG_B 0X0B
#define BINARY BIT(2)
#define UIP_SET BIT(7)
#define WAIT_UPDATE 244 


/**
 * @brief Check if the input from the rtc is written in binary or in bcd.
 * 
 * @return true - It's written in binary
 * @return false - It's written in bcd
 */
bool is_binary();

/**
 * @brief Checks if rtc is updating.
 * 
 * @return true - It's updating
 * @return false - It's not updating
 */
bool RTC_is_Updating();

/**
 * @brief Converts the int passed in the parameter from bcd to binary.
 * 
 * @param num - Number to be converted
 * @return int - The converted number
 */
int bcd_to_binary(int num);

/**
 * @brief Get the date object.
 * 
 * @return Date* - Date pointer to Date object
 */
Date * get_date();
