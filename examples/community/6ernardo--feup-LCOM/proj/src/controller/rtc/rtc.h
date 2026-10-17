#ifndef _LCOM_RTC_H_
#define _LCOM_RTC_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

/**
 * @defgroup rtc RTC
 * @{
 *
 * Functions and definitions for interacting with the Real-Time Clock (RTC).
 */

// Macros
#define RTC_MASK            5
#define IRQ_RTC             8
#define REGISTER_INPUT      0x70
#define REGISTER_OUTPUT     0x71

#define REGISTER_UPDATING   10
#define REGISTER_COUNTING   11

#define SECONDS    0
#define MINUTES    2
#define HOURS      4   
#define DAY        7
#define MONTH      8
#define YEAR       9

#define BINARY              BIT(2)
#define UPDATING            BIT(7)

// Structure to hold real-time information
typedef struct {
    uint8_t year;    
    uint8_t month;   
    uint8_t day;      
    uint8_t hours;   
    uint8_t minutes;  
    uint8_t seconds;  
} real_time_info;

// Function prototypes

/**
 * @brief Sets up the RTC.
 */
void rtc_setup();

/**
 * @brief Subscribes RTC interrupts.
 *
 * @param bit_no Address of memory to store the RTC interrupt bit number.
 * @return 0 if successful, non-zero otherwise.
 */
int rtc_subscribe_interrupts(uint8_t *bit_no);

/**
 * @brief Unsubscribes RTC interrupts.
 *
 * @return 0 if successful, non-zero otherwise.
 */
int rtc_unsubscribe_interrupts();

/**
 * @brief Reads a value from the RTC.
 *
 * @param command Command to be sent to the RTC.
 * @param output Pointer to the variable where the output will be stored.
 * @return 0 if successful, non-zero otherwise.
 */
int rtc_output(uint8_t command, uint8_t *output);

/**
 * @brief Checks if the RTC module is currently updating its registers.
 *
 * @return 0 if the RTC is not updating its registers, 1 if it is updating.
 */
int rtc_is_updating();

/**
 * @brief Checks if the RTC is in binary mode.
 *
 * @return 0 if successful, non-zero otherwise.
 */
int rtc_is_binary();

/**
 * @brief Checks if the RTC is in BCD mode.
 *
 * @return 0 if successful, non-zero otherwise.
 */
int rtc_is_bcd();

/**
 * @brief Updates the time information from the RTC module.
 *
 * @return 0 if the time information is successfully retrieved and updated, non-zero otherwise.
 */
int rtc_update_time();

/**
 * @brief Transforms a BCD number into binary.
 *
 * @param bcd_number BCD number to be transformed.
 * @return The binary number.
 */
uint8_t to_binary(uint8_t bcd_number);

/** @} end of rtc */

#endif /* _LCOM_RTC_H_ */
