#ifndef _LCOM_RTC_H_
#define _LCOM_RTC_H_

/**
 * @file rtc.h
 * 
 * @brief This file contains the RTC functions.
*/


/**
 * @brief Check if the number of the RTC is in binary-coded decimal (BCD).
 * 
 * @param bcd : 1 case in RTC format, 0 otherwise.
 * @return int : 0 if sucess and 1 otherwise.
 */
int RTCBinary(uint8_t *bcd);

/**
 * @brief Transform the RTC from BCD to binary.
 * 
 * @param time : Number in BCD format to be converted.
 */
void toBinary(uint8_t * time);

/**
 * @brief Read a register from the RTC.
 * 
 * @param time : Number to be returned by the register.
 * @param port : Register to read the information
 * @return int : 0 if sucess and 1 otherwise.
 */
int readRTC(uint8_t * time, uint8_t port);

/**
 * @brief Test the results of the rtc to find the adequate background for main menu.
 * 
 * @return int : 0 if light, 1 if dark, 2 if sunset and 3 if error.
 */
int timeRTC();

#endif
