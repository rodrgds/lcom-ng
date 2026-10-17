#pragma once

#include <minix/sysutil.h>
#include <lcom/lcf.h>

#define IRQ_RTC           8
#define REGISTER_INPUT    0x70
#define REGISTER_OUTPUT   0x71
#define REGISTER_A 0x0A
#define REGISTER_B        0x0B
#define REGISTER_C        0x0C
#define RTC_PF            BIT(6)
#define BIT(n)            (1 << (n))
#define REGISTER_UPDATING 10
#define REGISTER_COUNTING 11


typedef struct {
    uint8_t seconds;
} rtc_info;

/**
 * @brief Initializes the RTC by updating time and enabling periodic interrupts
 */
void rtc_setup();

/**
 * @brief Subscribes to RTC interrupts
 * 
 * @param bit_no Pointer to the bit number used for interrupt handling
 * 
 * @return 0 on success, 1 on failure
 */
int (rtc_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes from RTC interrupts.
 *
 * @return 0 on success, 1 on failure
 */
int (rtc_unsubscribe_int)();

/**
 * @brief RTC interrupt handler
 */
void (rtc_ih)();

/**
 * @brief Enables periodic interrupts from the RTC
 */
void rtc_enable_periodic_interrupts();

/**
 * @brief Sends a command to the RTC and retrieves the output
 * 
 * @param command RTC register to access
 * @param output Pointer to variable where the output will be stored
 * @return 0 on success, 1 on failure
 */
int rtc_output(uint8_t command, uint8_t *output);

/**
 * @brief Checks if the RTC is currently updating its time
 * 
 * @return 0 if not updating, 1 if updating or on error 
 */
int rtc_is_updating();

/**
 * @brief Updates the current time by reading from the RTC
 * 
 * @return 0 on success, 1 on failure or if the RTC is updating
 */
int rtc_update_time();

