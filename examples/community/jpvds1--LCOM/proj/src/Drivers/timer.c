#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>

#include "timer.h"

int hook_id = 0;



int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */

  if(freq > TIMER_FREQ || freq <= 18)
    return 1;

  uint8_t st;

  if(timer_get_conf(timer, &st))
    return 1;
  

  uint16_t div = TIMER_FREQ/freq;
  uint8_t lsb, msb;

  util_get_MSB(div, &msb);
  util_get_LSB(div, &lsb);
  
  st = st & 0x0f;

  switch (timer)
  {
  case 0:
    st = (st | TIMER_LSB_MSB | TIMER_SEL0);
    break;
  case 1:
    st = (st | TIMER_LSB_MSB | TIMER_SEL1);
    break;
  case 2:
    st = (st | TIMER_LSB_MSB | TIMER_SEL2);
    break; 
  default:
    return 1;
  }
  

  if(sys_outb(TIMER_CTRL,st))
    return 1;

  if(sys_outb(TIMER_0+timer, lsb))
    return 1;

  return sys_outb(TIMER_0+timer, msb);

}

int (timer_subscribe_int)(uint8_t *bit_no) {
   
  *bit_no = BIT(hook_id);
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id);
 
}

int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id);
}

void (timer_int_handler)() {
  global_counter++;
}


// done, no bugs
int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if(timer < 0 || timer >2)
    return 1;
  
  uint8_t readBackCommand = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer));

  if (sys_outb(TIMER_CTRL, readBackCommand))
    return 1;
    /* Obter a configuração do timer que queremos */
  return util_sys_inb(TIMER_0+timer, st);
}

//done, no bugs
int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
                          
  union timer_status_field_val friendly_config;


  //in_mode e  
  // "1" e "2"
  switch (field){
  case 0:
    friendly_config.byte = st;
    break;
  case 1:
    friendly_config.in_mode = (st & TIMER_LSB_MSB) >> 4;
    break;
  case 2:
  {
    uint8_t count_mode = (st >> 1) & 7;
    if(count_mode == 6)
      friendly_config.count_mode = 2;
    else if(count_mode == 7)
      friendly_config.count_mode = 3;
    else friendly_config.count_mode = count_mode;
  }
    break;

  case 3:
    friendly_config.bcd = st & 1;
    break;
  default:
    return 1;
  }

  return timer_print_config(timer, field, friendly_config);
}
