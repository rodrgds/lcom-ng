#include <lcom/lcf.h>
#include "timer.h"
#include "i8254.h"


int timerIntCounter = 0; //Counts the number of timer0 interrupts
int timer_hook_id = 0;


int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (timer <0 || timer >2 || freq < 19 || freq > TIMER_FREQ) return 1;


  uint16_t counterInitial = TIMER_FREQ / freq;
  uint8_t lsb, msb;
  if (util_get_LSB(counterInitial, &lsb) !=0) return 1;
  if (util_get_MSB(counterInitial, &msb) !=0) return 1;

  uint8_t word;
  uint8_t st;
  if (timer_get_conf(timer, &st) != 0) return 1;

  st = st & 0x0F;
  word = st | TIMER_LSB_MSB;
  uint8_t timerPort;

  switch (timer) {
    case 0:
      word = word | TIMER_SEL0;
      timerPort = TIMER_0;
      break;
    case 1:
      word = word | TIMER_SEL1;
        timerPort = TIMER_1;
      break;
    case 2:
      word = word | TIMER_SEL2;
        timerPort = TIMER_2;
      break;
    default:
      return 1;
  }

  if (sys_outb(TIMER_CTRL, word) != 0) return 1;
  if (sys_outb(timerPort, lsb) != 0) return 1;
  if (sys_outb(timerPort, msb) != 0) return 1;

  return 0;
}




int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  *bit_no = BIT(timer_hook_id);
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) != 0) return 1;
  
  return 0;
}



int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&timer_hook_id) != 0) return 1;
  return 0;
}


void (timer_int_handler)() {
 timerIntCounter++;
}



int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (!(timer == 0 || timer == 1 || timer == 2)) return 1;
  if (st == NULL) return 1;

  uint8_t rbc;
  rbc = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

  if (sys_outb(TIMER_CTRL, rbc) != 0) return 1;

  if (util_sys_inb(TIMER_0+timer, st) != 0) return 1;

  return 0;
}



int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  if (!(timer==0 || timer==1 ||timer==2)) return 1;

  union timer_status_field_val config;

  switch (field) {

    case tsf_all:
      config.byte = st;
      break;

    case tsf_initial:
      st = (st >> 4) & 0x3;

      if (st==1) config.in_mode = LSB_only;
      else if (st == 2) config.in_mode = MSB_only;
      else if (st == 3) config.in_mode = MSB_after_LSB;
      else config.in_mode = INVAL_val;
      break;

    case tsf_mode:
      st = (st >> 1) & 0x7;

      if (st == 0) config.count_mode = 0;
      else if (st == 1) config.count_mode = 1;
      else if (st == 2) config.count_mode = 2;
      else if (st == 3) config.count_mode = 3;
      else if (st == 4) config.count_mode = 4;
      else if (st == 5) config.count_mode = 5;
      else if (st == 6) config.count_mode = 2;
      else if (st == 7) config.count_mode = 3;
      break;


    case tsf_base:
      if (st & 0x1) config.bcd = true;
      else config.bcd= false;
      break;

    default:
      return 1;
  }

  if (timer_print_config(timer, field, config) != 0) return 1;

  return 0;
}
