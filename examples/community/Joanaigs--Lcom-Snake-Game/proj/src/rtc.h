#include <lcom/lcf.h>

#include "rtc_macros.h"

/**
 * @brief Subscribes and enables rtc interrupts
 *
 * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int rtc_subscribe_int(uint8_t *bit_no);

/**
 * @brief Unsubscribes rtc interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int rtc_unsubscribe_int();

/**
 * @brief wait till it recevies a valid rtc
 * 
 * @return int Return 0 upon success and non-zero otherwise
 */
int wait_valid_rtc();

/**
 * @brief Set the periodicity of rtc to 0.5 ms
 * 
 * @return int Return 0 upon success and non-zero otherwise
 */
int set_periodic();

/**
 * @brief Enables/disinables the RTC updates
 * 
 * @param on true to enable, false to disable
 * @return Return 0 upon success and non-zero otherwise
 */
int set_update_int(bool on);

/**
 * @brief delas with rtc interrupts
 * 
 */
void rtc_ih();
bool dealWithInterrupt;
