#ifndef _LCOM_RTC_H_
#define _LCOM_RTC_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

#define IRQ_RTC             8

typedef struct {
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
} RTC_structure;

/// @brief subscribes the real time counter interrupts
/// @return a 0 in case of success and a 1 in case of error with the system call
int rtc_subscribe_interrupts();

/// @brief unsubscribes the real time counter interrupts
/// @return a 0 in case of success and a 1 in case of error with the system call
int rtc_unsubscribe_interrupts();

/// @brief fetches data from the real time counter whenever there´s an interrupt
/// @param infoByte byte that indicates what informtaion we want
/// @param output received answer
/// @return a 0 in case of success and a 1 in case of error with the system call
int get_rtc_output(uint8_t infoByte, uint8_t *answer);

/// @brief fills the rtc_structure struct with the fetched information
/// @return a 0 in case of success and a 1 in case of error with the system call
int fill_rtc_structure();

/// @brief checks if the information is gonna be sent in bcd and, in case it is, convert to binary
/// @param bit what informtaion we want
/// @return return the result
uint8_t check_binary(int bit);

#endif
