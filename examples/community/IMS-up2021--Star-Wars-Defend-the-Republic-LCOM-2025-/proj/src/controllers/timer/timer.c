#include <lcom/lcf.h>
#include <lcom/timer.h>
#include "timer.h"
#include <stdint.h>


int timer_global_counter = 0;
int timer_hook_id = 0;


int (timer_subscribe_int)(uint8_t *bit_no_out) {
    if (bit_no_out == NULL) return 1;
    *bit_no_out = (uint8_t)timer_hook_id;

    if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) != 0) {
      printf("timer_subscribe_int: Kernel alocou/usou hook_id = %d para TIMER0_IRQ\n", timer_hook_id);
      return 1;
    }

    return 0;

}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&timer_hook_id) != 0) return 1; 
  return 0;
}

void (timer_int_handler)() {
  timer_global_counter++;
}



int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

  if (freq > TIMER_FREQ || freq < 19) return 1; // freq can't go bellow 19 because after a certain point the counter does an overflow

  uint8_t controlWord;
  if(timer_get_conf(timer, &controlWord) != 0) return 1;
  
  controlWord = (controlWord & 0x0F) | TIMER_LSB_MSB;

  uint32_t counter = TIMER_FREQ / freq;
  uint8_t lsb, msb;
  util_get_LSB(counter, &lsb);
  util_get_MSB(counter, &msb);

  uint8_t selectedTimer;
  switch (timer) {
  case 0:
    controlWord |= TIMER_SEL0;
    selectedTimer = TIMER_0;
    break;
  case 1:
    controlWord |= TIMER_SEL1;
    selectedTimer = TIMER_1;
    break;
  case 2:
    controlWord |= TIMER_SEL2;
    selectedTimer = TIMER_2;
    break;
  default:
    return 1;
  }

  if(sys_outb(TIMER_CTRL, controlWord) != 0) return 1;

  if(sys_outb(selectedTimer, lsb) != 0) return 1;
  if(sys_outb(selectedTimer, msb) != 0) return 1;
    
  return 0;
}



int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if(st == NULL || timer < 0  || timer > 2) return 1; // interruption in case: the address memory is not defined, and the timer not int the correct interval
  uint8_t RBC = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer));
  if(sys_outb(TIMER_CTRL, RBC) != 0) return 1; 
  if(util_sys_inb(TIMER_0 + timer, st)) return 1;
  return 0;
}


int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val val;

  switch(field) {

    case tsf_all:
      val.byte = st;
      break;
    case tsf_initial:
      st = (st >> 4) & 0x03; // Bitwise AND to isolate the last two bits (0x03 = 0011)
      if(st == 1) val.in_mode = LSB_only;
      else if (st == 2) val.in_mode = MSB_only;
      else if (st == 3) val.in_mode = MSB_after_LSB;
      else val.in_mode = INVAL_val;
      break;
    case tsf_mode:
      st = (st >> 1) & 0x07;
      if(st == 6) val.count_mode = 2; // Mode 2 - Rate Generator
      else if(st == 7) val.count_mode =3; // Mode 3 - Square Wave Generator
      else val.count_mode = st;
      break;
    case tsf_base:
      val.bcd = st & TIMER_BCD;
      break;
    default:
      return 1;
  }

  if(timer_print_config(timer, field, val) != 0) return 1;
  return 0;
}
