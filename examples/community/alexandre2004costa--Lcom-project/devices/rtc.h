#ifndef _LCOM_RTC_H_
#define _LCOM_RTC_H_


#include <minix/sysutil.h>
#include <lcom/lcf.h>


#define REG_INPUT      0x70
#define REG_OUTPUT     0x71

// comandos
#define SECOND         0
#define MINUTE         2
#define HOUR           4   
#define DAY            7
#define MONTH          8
#define YEAR           9
#define REG_UPDATING   10
#define REG_COUNTING   11

#define IS_BINARY              BIT(2)
#define IS_UPDATING            BIT(7)


typedef struct {
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} real_time;


int (update_time)();


#endif 


