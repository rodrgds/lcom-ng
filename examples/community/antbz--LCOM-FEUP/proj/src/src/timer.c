#include <lcom/lcf.h>
#include <lcom/utils.h>
#include <lcom/timer.h>
#include <stdint.h>
#include "../headers/i8254.h"

int timer_hookid = 0;
uint32_t timer_counter = 0;

int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = timer_hookid;
  sys_irqsetpolicy(TIMER0_IRQ , IRQ_REENABLE, &timer_hookid);
  return 0;
}

int (timer_unsubscribe_int)() {
  sys_irqrmpolicy(&timer_hookid);
  return 0;
}

void (timer_int_handler)() {
  timer_counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  sys_outb(TIMER_CTRL, TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer));
  switch(timer)
  {
    case 0:
      return util_sys_inb(TIMER_0, st);
    case 1:
      return util_sys_inb(TIMER_1, st);
    case 2:
      return util_sys_inb(TIMER_2, st);
    default:
      return 1;
  }
}
