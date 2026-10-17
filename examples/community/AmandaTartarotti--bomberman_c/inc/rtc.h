#ifndef _LCOM_RTC_H_
#define _LCOM_RTC_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "util.h"
#include "controller.h"


#define WRITE_RTC_REG 0x70
#define READ_RTC_REG 0x71

#define SEC_COMMAND 0
#define MIN_COMMAND 2
#define HOUR_COMMAND 4
#define DAY_COMMAND 7
#define MON_COMMAND 8
#define YEAR_COMMAND 9
#define BUSY_COMMAND 10
#define COUNT_ST_COMMAND 11

#define IRQ_LINE_RTC 8

typedef struct { 
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t days;
    uint8_t months;
    uint8_t years;  
    
} rtc_time;

/**
     * @brief Convert the unit of time being used by the rtc to a binary number.
     * 
     * First, sends a command to the rtc to know which unit of time is being (binary or BCD). If is BCD, convert to binary.
     * 
     * @return Return 0 upon success and non-zero otherwise
**/
int (conversion)();
/**
     * @brief Subscribes and enables RTC interrupts. 
     * 
     * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
     * 
     * @return Return 0 upon success and non-zero otherwise
**/
int (rtc_subscribe)(uint8_t *bit_no);
/**
     * @brief Unsubscribes RTC interrupts. 
     * 
     * @return Return 0 upon success and non-zero otherwise
**/
int (rtc_unsubscribe)();
/**
     * @brief Reads the rtc answer to a given command.
     * 
     * First, sends a command to the rtc writing port, then reads what is in the output register to find out the answer
     * 
     * @param command an uint8_t unit that specifies the command
     * @param output an uint8_t unit that stores the rtc output
     * 
     * @return Return 0 upon success and non-zero otherwise
**/
int (read_rtc)(uint8_t command, uint8_t *output);
/**
     * @brief Reads each current time unit from the rtc and stores it in a struct
     * 
     * @param clocking a rtc_time unit that stores the time from years to seconds
     * 
     * @return Return 0 upon success and non-zero otherwise
**/
int (update_rtc_time)(rtc_time *clocking);
/**
     * @brief Updates the rtc time. 
     * 
     * It attempts to call update_rtc_time at least 10 times passing as the parameter the game_clock, once it succeeds then converts the value to a binary number using the conversion().
     * The function is called on each rtc interrupt and each timer interrupt, to ensure that the time is updated every second
**/
void (rtc_ih)();


#endif /* _LCOM_RTC_H */
