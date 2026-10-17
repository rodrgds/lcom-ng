#ifndef RTC_H
#define RTC_H

/**
 * @defgroup rtc
 * @{ 
 * @brief Functions of the PC's Real Time Clock (RTC)
 */


#define RTC_IRQ             8 /**< @brief RTC IRQ line */

#define RTC_ADDR_REG        0x70 /**< @brief RTC Adress Register */
#define RTC_DATA_REG        0x71 /**< @brief RTC Data Register */

#define RTC_DAY_MONTH	    7 /**< @brief Day of the month register */
#define RTC_MONTH		    8 /**< @brief Month resgister */
#define RTC_YEAR            9 /**< @brief Year register */

#define RTC_REG_A           10 /**< @brief RTC Register A */
#define RTC_REG_B           11 /**< @brief RTC Register B */
#define RTC_REG_B_PIE       BIT(6) /**< @brief RTC Register B PIE*/
#define RTC_REG_C           12 /**< @brief RTC Register C*/
#define RTC_REG_C_PF        BIT(6) /**< @brief RTC Register C PF*/
#define RTC_REG_C_AF        BIT(5) /**< @brief RTC Register C AF*/
#define RTC_REG_C_UF        BIT(4) /**< @brief RTC Register C UF*/

/**
* @brief Subscribes and enables RTC 8 interrupts.
* @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
* @return Return 0 upon success and non-zero otherwise
*/
int rtc_subscribe_int(uint8_t *bit_no);

/**
* @brief Unsubscribes RTC 8 interrupts.
* @return Return 0 upon success and non-zero otherwise
*/
int rtc_unsubscribe_int();

/**
 * @brief Writes the cmd to the RTC_ADDR_REG.
 * 
 * @param cmd commad to be written in RTC_ADDR_REG
 * @return Return 0 upon success and non-zero otherwise
 */
int write_cmd_rtc(uint8_t cmd);

/**
 * @brief Reads the value of RTC_DATA_REG into st.
 * 
 * @param st where the value is written.
 * @return Return 0 upon success and non-zero otherwise
 */
int read_reg_rtc(uint8_t *st);

/**
 * @brief Enables or disables the RTC periodic interrupts.
 * 
 * @param enable true if enable, false if disable
 * @return Return 0 upon success and non-zero otherwise
 */
int enable_or_disable_rtc(bool enable);

/**
 * @brief Reads the day.
 * 
 * @return Returns the day
 */
int rtc_read_day();

/**
 * @brief Reads the month.
 * 
 * @return Returns the month
 */
int rtc_read_month();

/**
 * @brief Reads the year.
 * 
 * @return Returns the year
 */
int rtc_read_year();

/**
 * @brief RTC interrupt handler
 * 
 */
bool (rtc_ih)();

/** @} end of rtc */

#endif
