#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int counter = 0; ///< The interruption counter
int timer_hook_id; ///< The id to unsubscribe the timer interruptions


int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint8_t status;
  if(timer_get_conf(timer, &status) != 0) return 1; //get config as not to change the first 4 bits
  status &= 0x0F; //apply mask to status

  if(freq < 19) return 1; //prevent overflow
  uint16_t realFreq = TIMER_FREQ/freq;
  uint8_t LSB; util_get_LSB(realFreq, &LSB);
  uint8_t MSB; util_get_MSB(realFreq, &MSB);

  switch (timer){
    case 0:
    case TIMER_0:
      sys_outb(TIMER_CTRL, TIMER_SEL0 | TIMER_LSB_MSB | status); //0b0011xxxx
      sys_outb(TIMER_0, LSB);
      sys_outb(TIMER_0, MSB);
      break;
    case 1:
    case TIMER_1:
      sys_outb(TIMER_CTRL, TIMER_SEL1 | TIMER_LSB_MSB | status); //0b0111xxxx
      sys_outb(TIMER_1, LSB);
      sys_outb(TIMER_1, MSB);
      break;
    case 2:
    case TIMER_2:
      sys_outb(TIMER_CTRL, TIMER_SEL2 | TIMER_LSB_MSB | status); //0b1011xxxx
      sys_outb(TIMER_2, LSB);
      sys_outb(TIMER_2, MSB);
      break;
    default: //invalid timer
      return 1;
  }
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
  ++counter;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  switch (timer){
    case 0:
    case TIMER_0:
      timer = TIMER_0; // convert to right port value for sys_inb use
      sys_outb(TIMER_CTRL, TIMER_RB_CMD | TIMER_RB_SEL(0)); //0b11000010
      break;
    case 1:
    case TIMER_1:
      timer = TIMER_1;
      sys_outb(TIMER_CTRL, TIMER_RB_CMD | TIMER_RB_SEL(1)); //0b11000100
      break;
    case 2:
    case TIMER_2:
      timer = TIMER_2;
      sys_outb(TIMER_CTRL, TIMER_RB_CMD | TIMER_RB_SEL(2)); //0b11001000
      break;
    default: //invalid timer
      return 1;
  }
  return util_sys_inb(timer, st);
}


int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val conf;
  switch (field) {
      case tsf_all:
          // Display entire status byte
          conf.byte = st;
          break;
      case tsf_initial: {
          // Display initialization mode | Located on bits 4-5
          uint8_t initValue = (st >> 4) & 0x03;
          if(initValue == 0) conf.in_mode = INVAL_val;
          else if(initValue == 1) conf.in_mode = LSB_only;
          else if(initValue == 2) conf.in_mode = MSB_only;
          else if(initValue == 3) conf.in_mode = MSB_after_LSB;
          break;
      }
      case tsf_mode: {
          // Display the counting mode | Located on bits 1-3
          uint8_t count_mode = (st >> 1) & 0x07;
          // since the third bit does not matter, 0b110 also means mode 2 and 0b111 means mode 3
          if(count_mode == 6) conf.count_mode = 2;
          else if(count_mode == 7) conf.count_mode = 3;
          else conf.count_mode = (st >> 1) & 0x07;
          break;
      }
      case tsf_base:
          // Display counting base | Located on bit 0 | Either "Binary" or "BCD"
          conf.bcd = st & 0x01;
          break;
      default:
          return 1; // Invalid, return error
  }
  return timer_print_config(timer, field, conf);
}
