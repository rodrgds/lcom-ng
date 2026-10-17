#ifndef _LCOM_RTC_H
#define _LCOM_RTC_H

#define RTC_IRQ             8

#define RTC_ADDR_REG        0x70
#define RTC_DATA_REG        0x71 

#define RTC_SECONDS 	    	0   
#define RTC_MINUTES 	    	2   
#define RTC_HOURS 		    	4   
#define RTC_DAYOFWEEK 	    6   
#define RTC_DAYOFMONTH      7   
#define RTC_MONTH			      8   
#define RTC_YEAR			      9

#define PERIODIC_INTERRUPT_ENABLE   BIT(6)

#define RTC_REG_A	    	    10
#define RTC_REG_B		        11
#define RTC_REG_C	         	12
#define RTC_REG_D	         	13

#define RTC_UIP             BIT(7)

/**
* @brief Reads RTC's data
* 
* Reads RTC's data by writing to the appropriate register
*
* @param reg - hex register to write to RTC's register
* @param data - pointer to data where date is written to
* @return Returns 1
*/
int rtc_read_reg_data(uint8_t reg, uint8_t *data);
/**
* @brief Reads RTC's year
*
* Reads RTC's year
*
* @return Returns the year read
*/
int rtc_read_year();
/**
* @brief Reads RTC's month
*
* Reads RTC's month
*
* @return Returns the month read
*/
int rtc_read_month();
/**
* @brief Reads RTC's day
*
* Reads RTC's day
*
* @return Returns the day read
*/
int rtc_read_day();
/**
* @brief Reads RTC's hour
*
* Reads RTC's hour
*
* @return Returns the hour read
*/
int rtc_read_hour();
/**
* @brief Reads RTC's minutes
*
* Reads RTC's minutes
*
* @return Returns the minutes read
*/
int rtc_read_minute();
/**
* @brief Reads RTC's seconds
*
* Reads RTC's seconds
*
* @return Returns the seconds read
*/
int rtc_read_second();

#endif /*_LCOM_RTC_H*/
