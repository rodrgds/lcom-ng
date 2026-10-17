#include <lcom/lcf.h>

/**
 * @brief Subscribes the rtc's interrupts
 * 
 */
int (rtc_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes the rtc's interrupts
 * 
 */
int (rtc_unsubscribe_int)();

/**
 * @brief RTC interrupt handler that makes sure there are no erros and updates the varibales of hours and minutes
 * 
 */
void (rtc_ih)();

/**
 * @brief Waits for an rtc update so that we can make sure we won't have any errors while waiting for its interruptions
 * 
 */
void waitUpdate();

/**
 * @brief Converts an hexadecimal value to int so that the written value is the same, for example if the value is 64 in hexadecimal the funtion will return 64 in decimal
 * 
 * @param time time in hexadecimal
 * @return int time in decimal
 */
int hexInt(uint8_t time);

