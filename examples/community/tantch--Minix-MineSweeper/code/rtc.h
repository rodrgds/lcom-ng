#ifndef RTC_H
#define RTC_H

#include <minix/syslib.h>
#include <minix/drivers.h>
#include "general.h"
#include "font.h"





#define ADDR_REGR 0x70
#define DATA_REGR 0x71
#define REG_A 0x0A
#define REG_B 0x0B
#define REG_C 0x0C
#define REG_D 0x0D
#define IRQFRTC 8
#define SEC 0x00
#define SEC_ALAM 0X01
#define MINU 0x02
#define MINU_ALAM 0X03
#define HOUR 0x04
#define HOUR_ALAM 0X05
#define WEEK_DAY 0x06
#define DAY 0x07
#define MONTH 0x08
#define YEAR 0x09



int subscribe_rtc();
int unsubscribe_rtc();
void set_rtc();
int read_state();
void print_date() ;
void rtc_handle() ;



#endif
