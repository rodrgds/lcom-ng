#include "rtc.h"
#include "rtc_macros.h"

static int rtc_hook_id = 8;
static bool _alarm = false;
static bool _periodic = false;
static bool _update = false;

/* Interrupt Handler */

/**
 * @brief RTC interrupt handler
 * @details On interrupt, check the type of interrupt (Alarm, Update or Periodic) and act accordingly
*/
void rtc_ih() {
    uint8_t data;
    if (rtc_read_register(RTC_REG_C, &data) != 0){
      return;
    }

    if(data & RTC_UF){
       _update = true;
    }

    if(data & RTC_AF) {
        _alarm = true;
    }

    if(data & RTC_PF) {
        _periodic = true;
    }

    rtc_clear_flags();
}





/* Auxiliary functions */

/**
 * @brief Converts a BCD number to decimal. 
 * @details For example, 0010 (= 2) 0011 (= 3) in BCD is 23 in decimal
*/
int bcd_to_decimal(uint8_t * number){
	uint8_t temp = 0;
	temp = (*number)&0xf; // Right digit
	temp += 10*(((*number)>>4)&0xf); // Left digit (Higher base, so multiply by 10)
	(*number) = temp;
	return 0;
}

/**
 * @brief Converts a decimal number to BCD. 
 * @details For example, 23 in decimal is 0010 (= 2) 0011 (= 3) in BCD
*/
int binary_to_bcd(uint8_t * number) {
    uint8_t temp = 0;
    temp = (*number)%10 | ((*number)/10) << 4; // Extract First and second digits
    *number = temp;
    return 0;
}





/* Functions that enable interrupts */

/**
 * @brief Subscribes RTC interrupts
 */
int rtc_subscribe_int(uint32_t *bit_no) {
    if (bit_no == NULL){
        printf("Error in rtc_subscribe_int()\n");
        return 1;
    }

    *bit_no = rtc_hook_id;

    if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &rtc_hook_id) != 0){
        printf("Error subscribing RTC interrupts");
        return 1;
    }

    return 0;
}

/**
 * @brief Enable Alarm Interrupts, by writing corresponding bytes to register B of the RTC
*/
int enable_aie_int(){
    uint8_t b_config;
    if (rtc_read_register(RTC_REG_B, &b_config) != 0){
        printf("Error in rtc_read_register\n");
        return 1;
    }
    uint8_t flag = b_config | RTC_AIE;
    if (rtc_write_reg(RTC_REG_B, flag) != 0){
        printf("Error in rtc_write_reg()\n");
        return 1;
    }
    return 0;
}





/* Functions that disable interrupts*/

/**
 * @brief Unsubscribes RTC interrupts
 */
int rtc_unsubscribe_int() {
    if (sys_irqrmpolicy(&rtc_hook_id) != 0){
        printf("Error unsubscribing RTC interrupts\n");
        return 1;
    }
    return 0;
}

/**
 * @brief Disable Periodic Interrupts, by writing corresponding bytes to register B of the RTC
*/
int disable_pie_int(){
    uint8_t b_config;
    rtc_read_register(RTC_REG_B, &b_config);
    return rtc_write_reg(RTC_REG_B, b_config & ~RTC_PIE);
}

/**
 * @brief Disable Alarm Interrupts, by writing corresponding bytes to register B of the RTC
*/
int disable_aie_int(){
    uint8_t b_config;
    rtc_read_register(RTC_REG_B, &b_config);
    return rtc_write_reg(RTC_REG_B, b_config & ~RTC_AIE);
}

/**
 * @brief Disable Update Interrupts, by writing corresponding bytes to register B of the RTC
*/
int disable_uie_int(){
    uint8_t b_config;
    rtc_read_register(RTC_REG_B, &b_config);
    return rtc_write_reg(RTC_REG_B, b_config & ~RTC_UIE);
}





/* Functions that interract with RTC */

/**
 * @brief Read a byte from an RTC register
 * @details This function first specifies the address of the RTC register to be accessed and then
 *          the port from which it reads the data
 */
int rtc_read_register(uint8_t reg, uint8_t * byte){
	if (rtc_write_to_port(RTC_ADDR_REG, reg) != 0){
        printf("Error in rtc_write_to_port()\n");
        return 1;
    }
	if (util_sys_inb(RTC_DATA_REG, byte) != 0){
        printf("Error in util_sys_inb()\n");
        return 1;
    }
    return 0;
}

RTC_Config rtc_get_config() {
    RTC_Config config;

    rtc_read_register(SECONDS, &config.seconds);
    rtc_read_register(MINUTES, &config.minutes);
    rtc_read_register(HOURS, &config.hours);
    rtc_read_register(WEEK_DAY, &config.day_of_week);
    rtc_read_register(MONTH_DAY, &config.day_of_month);
    rtc_read_register(MONTH, &config.month);
    rtc_read_register(YEAR, &config.year);
    rtc_read_register(RTC_REG_A, &config.reg_a);
    rtc_read_register(RTC_REG_B, &config.reg_b);
    rtc_read_register(RTC_REG_C, &config.reg_c);

    return config;
}

/**
 * @brief Check RTC's UIP flag to avoid race conditions
*/
bool rtc_is_updating(){
    uint8_t byte = 0;
    if (rtc_read_register(RTC_REG_A, &byte) != 0){
        return 1;
    }
    return (byte & RTC_UIP) >> 7; // 1 if RTC is updating, else 0
}

/**
 * @brief Write a byte to an RTC port
 */
int rtc_write_to_port(uint8_t port, uint8_t reg){
	if (sys_outb(port, reg) != 0){
        printf("Error in sys_outb()\n");
        return 1;
    }
    return 0;
}

/**
 * @brief Write a byte to an RTC register
 * @details This function first specifies the address of the RTC register to be accessed and then
 *          the port to which it transfers the data
 */
int rtc_write_reg(uint8_t reg, uint8_t byte) {
    if (rtc_write_to_port(RTC_ADDR_REG, reg) != 0){
        return 1;
    }
    if (rtc_write_to_port(RTC_DATA_REG, byte) != 0){
        return 1;
    }
    return 0;
}

/**
 * @brief Clear rtc interrupts.
 * @details Clears register C of rtc.
 */
int rtc_clear_flags(){
	if(rtc_write_reg(RTC_REG_C, 0) != 0){
        return 1;
    }
	return 0;
}

int rtc_set_alarm(uint8_t sec, uint8_t min, uint8_t hour){
    if (rtc_write_reg(RTC_ALARM_SECONDS, sec) != 0){
        return 1;
    }
    if (rtc_write_reg(RTC_ALARM_MINUTES, min) != 0){
        return 1;
    }
    if (rtc_write_reg(RTC_ALARM_HOURS, hour) != 0){
        return 1;
    }
    return 0;
}

bool rtc_get_interrupt_was_alarm() {
    return _alarm;
}

bool rtc_get_interrupt_was_periodic() {
    return _periodic;
}

bool rtc_get_interrupt_was_update() {
    return _update;
}

void rtc_set_interrupt_alarm_processed() {
    _alarm = false;
}

void rtc_set_interrupt_periodic_processed() {
    _periodic = false;
}

void rtc_set_interrupt_update_processed() {
    _update = false;
}

