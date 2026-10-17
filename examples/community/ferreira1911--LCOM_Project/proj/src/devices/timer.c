#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int counter = 0;
int timer_hook_id = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if(timer > 2) return 1;

  uint16_t div = TIMER_FREQ / freq;
  uint8_t lsb8, msb8;

  if(util_get_LSB(div, &lsb8) || util_get_MSB(div, &msb8)) return 1;

  uint8_t st;
  if(timer_get_conf(timer, &st)) return 1;

  uint8_t control_word = (timer << 6) | TIMER_LSB_MSB | (st & 0x0F);

  if(sys_outb(TIMER_CTRL, control_word) != 0) return 1;

  if(sys_outb(TIMER_0 + timer, lsb8) != 0) return 1;

  if(sys_outb(TIMER_0 + timer, msb8) != 0) return 1;

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;

  timer_hook_id = *bit_no;

  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) != 0) return 1;

  return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&timer_hook_id) != 0) return 1;

  return 0;
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if(timer > 2) return 1;

  uint8_t readBackCommand = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  sys_outb(TIMER_CTRL, readBackCommand);

  if(util_sys_inb(TIMER_0 + timer, st) != 0) return 1;
  
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  if(timer > 2) return 1;
  union timer_status_field_val conf;

  switch (field)
  {
  case tsf_all:
    conf.byte = st;
    break;
  
  case tsf_initial:
    {
      uint8_t initial = (st >> 4) & 0x03;
      switch (initial)
      {
      case 0x01:
        conf.in_mode = LSB_only;
        break;
      
      case 0x02:
        conf.in_mode = MSB_only;
        break;
      
      case 0x03:
        conf.in_mode = MSB_after_LSB;
        break;
      
      default:
        conf.in_mode = INVAL_val;
        break;
      }
    }
    break;
  
  case tsf_mode:
    {
      uint8_t mode = (st >> 1) & 0x07;
      conf.count_mode = mode;
    }
    break;
  
  case tsf_base:
    {
      uint8_t base = st & 1;
      if(base == 0){
        conf.bcd = true;
      }else{
        conf.bcd = false;
      }
    }
    break;
  
  default:
    return 1;
  }

  return timer_print_config(timer, field, conf);
}
