#ifndef __RTC_H
#define __RTC_H

/** @defgroup RTC RTC
 * @{
 */

typedef struct date{
  unsigned year;
  unsigned month;
  unsigned day;
  unsigned hour;
  unsigned minute;
  unsigned second;
} Date;

#define RTC_IRQ            8
#define RTC_ADDR_REG	     0x70
#define RTC_DATA_REG	     0x71
#define REGISTER_B         11
#define REGISTER_C         12

/*
@brief subscribes the interrupts of the rtc

@param bit_no returns the bit of the rtc irq

@return 0 if everything is ok
*/

int rtc_subscribe_int(int *bit_no);

/*
@brief unsubscribes the rtc interrupts

@return 0 if everything is ok
*/

int rtc_unsubscribe_int();

/*
@brief enables the rtc interrupts

@return 0 if everything is ok
*/

int rtc_enable();

/*
@brief disables the rtc interrupts

@return 0 if everything is ok
*/

int rtc_disable();

/*
@brief converts bcd to int

@param x variable to be converted

@return the variable converted to int
*/

int convert_bcd_to_int(unsigned x);

/*
@brief initializes the Date struct

@return the Date struct
*/

Date NewDate();

/*
@brief updates the Date struct

@param date struct to be updated
*/

void rtc_update(Date *date);

/*
@brief converts a Date struct to a string in the format dd/mm/yyyy

@param d  the date struct from which the data is taken

@param s  the string to hold the format
*/
void dateToString(Date *d, char *s);

/*
@brief converts a string in the format dd/mm/yyyy to a Date struct  

@param d  the date struct to be changed

@param s  the string from which the data is taken
*/
void stringToDate(Date *d, char *s);

#endif
