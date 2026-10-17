#include <stdint.h>
#include <stdio.h>
#include <lcom/lcf.h>
/** @defgroup rtc rtc.h
 * @{
 *
 * This file contains the functions related to the rtc functioning.
 * 
 */

int( rtc_data_io)(uint8_t address, uint8_t *content, bool is_out);
int (read_date)();
void(terminate_minix)(bool kill);
int (bcd_conversion)(uint8_t *bcd_num);
int (rtc_subscribe_int)(int *irq_set);
int(rtc_unsubscribe_int)();
int(rtc_ih)();
int(rtc_enable_interrupts)();


typedef struct {
    unsigned seconds;
    unsigned minutes;
    unsigned hours;
    unsigned day_month;
    unsigned day_week;
    unsigned month;
    unsigned year;
} date_t; 
/**@}*/
