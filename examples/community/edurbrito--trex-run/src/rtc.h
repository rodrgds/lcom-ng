#ifndef RTC_H
#define RTC_H

/** 
 * @defgroup RTC rtc
 * @{
 *
 * Functions for using the Rtc
 */

#define RTC_IRQ						    8
#define RTC_ADDR_REG				    0x70
#define RTC_DATA_REG			        0x71

#define SEC								0x00
#define	MINT        			        0x02
#define	HOUR							0x04
#define	DAY				       	        0x07
#define	MONTH							0x08
#define	YEAR							0x09
#define REG_A							10
#define REG_B							11
#define REG_C							12
#define REG_D							13
#define REGAP				    	    BIT(7)
#define REGBM				            BIT(2)

/**
 * @brief Subscribes and enables RTC interrupts
 *
 * @param bit_no address of memory to be initialized with 
 * the bit number to be set in the mask returned upon an interrupt.
 * 
 * @return 0 if OK, 1 if NOT_OK
 */
int rtc_subscribe_int(uint8_t *bit_no);

/**
 * @brief Unsubscribes RTC interrupts
 *
 * @return 0 if OK, 1 if NOT_OK
 */
int rtc_unsubscribe_int();

/**
* @brief Reads the content in the specified port
*
* @param port Register from RTC
*
* @return Data on register port
*/
uint32_t rtc_data(uint8_t port);

/**
* @brief Checks if data is in bcd
*
* @return true if in bcd, false otherwise
*/
bool check_bcd();

/**
* @brief Converts num to binary if it's in bcd
*
* @param num number to be converted
*
*/
void bcd_to_bin(uint32_t* num);

/**
* @brief Checks if RTC is updating
*
* @return 0 if OK, 1 if NOT_OK 
*/
int isUpdating();

/**
* @brief Reads time from the RTC if it's not updating
*
* @param sec address of memory to be initialized with the current sec
* @param min address of memory to be initialized with the current min
* @param hour address of memory to be initialized with the current hour
*
*/
void get_time(uint32_t *sec,uint32_t *min, uint32_t *hour);

/**
* @brief Reads date from the RTC if it's not updating
*
* @param day address of memory to be initialized with the current day
* @param month address of memory to be initialized with the current month
* @param year address of memory to be initialized with the current year
*
*/
void get_date(uint32_t *day,uint32_t *month, uint32_t *year);

#endif
