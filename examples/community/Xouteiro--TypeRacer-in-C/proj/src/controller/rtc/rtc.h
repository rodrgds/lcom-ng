#ifndef RTC_H
#define RTC_H

#include <lcom/lcf.h>

#define RTC_MASK 5
#define RTC_IRQ 8
#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

#define DAY_REG 0X7
#define MONTH_REG 0X8
#define YEAR_REG 0X9
#define SECOND_REG 0X0
#define MINUTE_REG 0X2
#define HOUR_REG 0X4


#define UIP BIT(7)
#define RATE (BIT(0) | BIT(1) | BIT(2) | BIT(3))

#define PIE BIT(6)
#define UIE BIT(4)

#define IRQF BIT(7)
#define UF BIT(4)
#define PF BIT(6)

#define DATE_STR_SIZE 9
#define HOUR_STR_SIZE 9


int (rtc_interrupts_subscription)(uint8_t* bit_no);
int (rtc_interrupts_unsubscription)(void);
int (rtc_initialize)(void);
void (rtc_ih)(void);
int (rtc_counter_start)(void);
void (rtc_interrupt_counter)(void);
int (rtc_get_elapsed_time)(void);
int (rtc_update_date)(void);
int (rtc_update_hour)(void);
int (rtc_read_register)(uint8_t reg, uint8_t* byte);
int (rtc_write_register)(uint8_t reg, uint8_t byte);


#endif
