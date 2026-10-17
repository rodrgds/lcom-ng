#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

#include <lcom/utils.h>
#include "rtc.h"

#ifdef DEBUG
#  define DEBUG_MSG(str) printf(str)
#else
#  define DEBUG_MSG(str)
#endif


/*global variables*/

int rtc_hook_id = RTC_MASK; // RTC mask
real_time_info rt_info;   // Info sobre rt
extern int counter;


void (rtc_ih)() {

    /* TO UPDATE TIME per minute*/
    if (counter % 3600 == 0){
        rtc_update_real_time();
    }
}


int (rtc_subscribe_int)(uint8_t *bit_no) {
  rtc_hook_id = *bit_no;
  return sys_irqsetpolicy(RTC_IRQ,IRQ_REENABLE|IRQ_EXCLUSIVE,&rtc_hook_id);
}

int (rtc_unsubscribe_int)() {
  return sys_irqrmpolicy(&rtc_hook_id);
}


uint8_t (rtc_convert_to_binary)(uint8_t n) {

  uint8_t result = 0;
  uint8_t multiplier = 1;
  int i;

  for (i = 0; i < 8; i++) {
    uint8_t lsb = n & 0x0F;
    result += (lsb * multiplier);
    n >>= 4;
    multiplier *= 10;
  }

  return result;
}

int (rtc_update_real_time)() {
    
    // Se o RTC estiver ocupado a atualizar os registos não devemos ler dados
    uint8_t status_rtc;
    uint8_t res_minute;
    uint8_t res_hour;
    if (sys_outb(RTC_INPUT, RTC_REG_A) != 0) return 1;
	  if (util_sys_inb(RTC_OUTPUT, &status_rtc) != 0) return 1;
    if ((status_rtc & BIT(7)) != 0) return 1; /*update in progress -> BIT(7) (must not read or write) */

    if (sys_outb(RTC_INPUT, RTC_READ_MIN) != 0) return 1;
	  int mins = util_sys_inb(RTC_OUTPUT, &res_minute);

    if (sys_outb(RTC_INPUT, RTC_READ_HRS) != 0) return 1;
	  int hours = util_sys_inb(RTC_OUTPUT, &res_hour);


    if (mins || hours){
        DEBUG_MSG("Error reading output\n");
        return 1;
    }
  

    rt_info.mins = rtc_convert_to_binary(res_minute);
    rt_info.hrs = rtc_convert_to_binary(res_hour);

    printf("Mins: %d\n", rt_info.mins);
    printf("Hrs: %d\n", rt_info.hrs);

    return 0;
}

int (rtc_check_permission)(){
    if(rt_info.hrs >= 23 || rt_info.hrs <= 7){
        return 1;
    }
    return 0;
}
