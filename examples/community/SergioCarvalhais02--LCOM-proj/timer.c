#include <lcom/lcf.h>

#include <stdint.h>

#include "i8254.h"


int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  
  if (freq < 19 || freq > TIMER_FREQ) return 1;

  if (timer < 0 || timer > 2) return 1;

  uint16_t timer_start_val = (uint16_t)(TIMER_FREQ / freq);

  uint8_t msb_timer_start_val, lsb_timer_start_val; 
  
  if (util_get_LSB(timer_start_val, &lsb_timer_start_val) != 0) return 1;

  if (util_get_MSB(timer_start_val, &msb_timer_start_val) != 0) return 1;

  uint8_t st;

  if ((timer_get_conf(timer, &st)) != 0) return 1;

  uint8_t control_word;

  st &= 0x0f;

  switch (timer){
    case 0: {
      control_word = TIMER_LSB_MSB | st | TIMER_SEL0;
      break;
    }

    case 1: {
      control_word = TIMER_LSB_MSB | st | TIMER_SEL1;
      break;
    }
    
    case 2: {
      control_word = TIMER_LSB_MSB | st | TIMER_SEL2;
      break;
    }
    
    default: return 1;
  }

  if ((sys_outb(TIMER_CTRL, control_word)) != 0) return 1;

  if ((sys_outb(TIMER_N(timer), lsb_timer_start_val)) != 0) return 1;

  if ((sys_outb(TIMER_N(timer), msb_timer_start_val)) != 0) return 1;

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  
  timer_hook_id = *bit_no;

  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id);
}

int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&timer_hook_id);
}

void (timer_int_handler)() {
  counter_var++; 
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {

  if(timer < 0 || timer > 2) return 1;

  uint32_t control_word = TIMER_RB_SEL(timer) | TIMER_RB_CMD | TIMER_RB_COUNT_;

  if ((sys_outb(TIMER_CTRL, control_word)) != 0) return 1;

  if ((util_sys_inb(TIMER_N(timer), st)) != 0) return 1;

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {  

  union timer_status_field_val val;

  switch(field){
    case tsf_all: {
      val.byte = st;
      break;
    }

    case tsf_initial : { 
      switch ((st >> 4) & 0x03){
        case 0: {
          val.in_mode = INVAL_val;
          break;
        }
        
        case 1: {
          val.in_mode = LSB_only;
          break;
        }
        
        case 2: {
           val.in_mode = MSB_only;
          break;
        }
        
        case 3: {
          val.in_mode = MSB_after_LSB;
          break;
        }

        default: return 1;
      }
      break;
    }

    case tsf_mode: {
      switch ((st >> 1) & 0x07){
        case 0: {
          val.count_mode = 0x00;
          break;
        }

        case 1: {
          val.count_mode = 0x01;
          break;
        }
        
        case 2:
        case 6: {
          val.count_mode = 0x02;
          break;
        }
        
        case 3:
        case 7: {
          val.count_mode = 0x03;
          break;
        }
        
        case 4: {
          val.count_mode = 0x04;
          break;
        }
        
        case 5: {
          val.count_mode = 0x05;
          break;
        }
        
        default: return 1;
      }
      break;
    }

    case tsf_base: {
      val.bcd = (st % 2 == 0) ? false : true;
      break;
    }

    default: return 1;
  }

  if ((timer_print_config(timer, field, val)) != 0) return 1;

  return 0;
}
