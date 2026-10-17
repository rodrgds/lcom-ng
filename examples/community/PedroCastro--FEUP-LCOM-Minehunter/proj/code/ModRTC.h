#ifndef __MODRTC_H
#define __MODRTC_H


#define BIT(n)                  (0x01<<(n))

#define RTC_ADDR_REG 		0x70
#define RTC_DATA_REG 		0x71
#define RTC_SEC 		0
#define RTC_MIN 		2
#define RTC_HOUR 		4
#define RTC_DAY_WEEK 		6
#define RTC_DAY_MONTH 		7
#define RTC_MONTH 		8
#define RTC_YEAR 		9
#define RTC_REG_A		10
#define RTC_REG_B		11
#define RTC_REG_C		12
#define RTC_REG_D		13
#define RTC_UIP                 BIT(7)
#define RTC_DATA_MODE   	BIT(2)


unsigned long day_month;
unsigned long day_week;
unsigned long month;
unsigned long year;
unsigned long hour;
unsigned long min;
unsigned long sec;

///@brief reads index from the rtc
///@param index register to read
///@param dado register where information that is read from the register is put
///@return success
int scan_rtc (unsigned long index, unsigned long *dado);

///@brief writes data on the index provided
///@param index
///@param dado
///@return success
int write_rtc(unsigned long index, unsigned long dado);

///@brief deactivates the updates of the rtc
void disable();

///@brief activates the updates of the rtc
void enable();

///@brief wait for a valid rtc after updates
void wait_valid_rtc(void);

///@brief takes a bcd value and turns it into a binary value
///@param bcdvalue value to turn into binary
///@return  the value
unsigned char bcd_to_binary(unsigned char bcdvalue);

///@brief takes a binary value and turns it into a bcd value
///@param binaryvalue
///@return the value
unsigned char binary_to_bcd(unsigned char binaryvalue);

///@brief reads the date set on the rtc
///@return success
int rtc_date();


#endif
