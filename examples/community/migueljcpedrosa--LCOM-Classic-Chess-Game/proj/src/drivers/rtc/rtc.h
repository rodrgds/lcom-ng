/**
 * @file rtc
 * @brief This file has the implementation of the functions related to the RTC.
 */

#ifndef _RTC_H_
#define _RTC_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include <stdbool.h>
#include "mrtc.h"

/**
 * @brief Struct that represents the rtc timestamp.
 *
 * This struct contains the year, month, day, hours, minutes, and seconds.
 */
typedef struct {
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} rtc_timestamp_t;

/**
 * @brief Checks if the RTC is in binary mode.
 *
 * This function reads the Register B of the RTC and checks the second bit 
 * which determines if the RTC is in binary mode.
 * 
 * @return Returns 0 if the RTC is in BCD mode, and non-zero if it's in binary mode.
 */
int rtc_in_binary_mode();

/**
 * @brief Initializes the RTC system.
 *
 * This function checks if the RTC is in binary mode, and if so, 
 * it attempts to update the current time.
 * 
 * @return Returns 0 upon success, and 1 upon failure.
 */
int rtc_initialize_system();

/**
 * @brief Subscribes RTC interrupts.
 * 
 * @param bit_no A pointer to the bit number to be set in the mask returned upon an interrupt.
 *
 * This function subscribes RTC interrupts by setting the policy for the 
 * RTC IRQ line to be re-enabled after being handled.
 * 
 * @return Returns 0 upon success, and non-zero otherwise.
 */
int rtc_subscribe_interrupts(uint8_t *bit_no);

/**
 * @brief Unsubscribes RTC interrupts.
 *
 * This function unsubscribes RTC interrupts.
 * 
 * @return Returns 0 upon success, and non-zero otherwise.
 */
int rtc_unsubscribe_interrupts();

/**
 * @brief Reads a value from a specific RTC register.
 * 
 * @param command The register to read from.
 * @param output A pointer to store the read value.
 *
 * This function reads a value from a specified RTC register and stores it 
 * in the provided pointer.
 * 
 * @return Returns 0 upon success, and 1 upon failure.
 */
int rtc_read_register(uint8_t command, uint8_t *output);

/**
 * @brief Converts a BCD number to binary.
 * 
 * @param bcd_number The BCD number to convert.
 *
 * This function converts a BCD number to its equivalent binary number.
 * 
 * @return Returns the converted binary number.
 */
uint8_t rtc_convert_bcd_to_binary(uint8_t bcd_number);

/**
 * @brief Checks if the RTC is currently updating.
 *
 * This function checks the RTC update status register and returns 
 * true if an update is in progress.
 * 
 * @return Returns true if the RTC is updating, false otherwise.
 */
bool rtc_currently_updating();

/**
 * @brief Updates the current time stored in the RTC.
 *
 * This function updates the current time by reading the seconds, minutes, 
 * hours, day, month, and year registers from the RTC.
 * 
 * @return Returns 0 upon success, and 1 upon failure.
 */
int rtc_update_current_time();

/**
 * @brief Updates a specific time unit in the RTC.
 * 
 * @param register_address The register that stores the time unit to update.
 * @param time_component A pointer to the variable that will hold the updated time unit.
 *
 * This function reads the specified RTC register and updates the corresponding 
 * time component. If the RTC is in binary mode, the register value is used 
 * directly. Otherwise, it's converted from BCD to binary.
 * 
 * @return Returns 0 upon success, and 1 upon failure.
 */
int rtc_update_time_unit(uint8_t register_address, uint8_t* time_component);

/**
 * @brief Interrupt handler for the RTC.
 * 
 * @param timestamp A pointer to the variable that will hold the current time upon interrupt.
 *
 * This function handles the RTC interrupts by updating the current time 
 * and storing it in the provided timestamp.
 * 
 * @return Returns 0 upon success, and 1 upon failure.
 */
int rtc_ih(rtc_timestamp_t* timestamp);

#endif
