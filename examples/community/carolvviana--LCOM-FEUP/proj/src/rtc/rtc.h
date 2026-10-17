#pragma once

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

// ======================================== CONSTANTS ========================================

#define RTC_MASK 5 /*!< @brief RTC's mask */
#define RTC_IRQ 8 /*!< @brief RTC's IRQ line */

/*addresses*/
#define RTC_INPUT 0x70  /*!< @brief Address where the RTC data's byte offet is sent to */
#define RTC_OUTPUT 0x71 /*!< @brief Address where the selected data is read */

/*reading and alarm registers*/
#define RTC_READ_MIN 2 /*!< @brief Byte offset of the minutes */
#define RTC_READ_HRS 4 /*!< @brief Byte offset of the hours */

/*registers for control of the RTC*/
#define RTC_REG_A 10 /*!< @brief Byte offset of the register A */
#define RTC_REG_B 11 /*!< @brief Byte offset of the register B */

/** @brief struct for real time important information to be stored*/
typedef struct {
  uint8_t hrs; /*!< @brief real time hour*/
  uint8_t mins; /*!< @brief real time minutes*/
} real_time_info;

//======================================== FUNCTIONS ========================================

/**
 * @brief Subscribing to RTC interrupts.
 * @return 0 if success, 1 if error
 */
int(rtc_subscribe_int)(uint8_t *bit_no);
/**
 * @brief Unubscribing to RTC interrupts.
 * @return 0 if success, 1 if error
 */
int(rtc_unsubscribe_int)();

/**
 * @brief Converts the number in BCD to binary.
 *
 * @param n Number to convert
 * @return uint8_t
 */
uint8_t(rtc_convert_to_binary)(uint8_t n);

/**
 * @brief Updates the real_time_info struct with the current time.
 * @return 0 if success, 1 if error
 */
int(rtc_update_real_time)();

/**
 * @brief RTC's interrupt handler to update real time information on struct.
 */
void(rtc_ih)();

/**
 * @brief Checks if the current hour blocks the user from playing (night time).
 * @return 0 if user can play, 1 if not
 */
int(rtc_check_permission)();

