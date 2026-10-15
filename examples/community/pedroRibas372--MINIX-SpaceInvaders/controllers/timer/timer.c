#include <lcom/timer.h>

#include <stdint.h>
#include "i8254.h"

static int hook_id;
uint32_t counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (timer > 2) return 1;
  if (freq == 0 || freq > TIMER_FREQ) return 1;

  uint16_t res = TIMER_FREQ / freq;

  //Extract 0xFF00, 0x00FF of res
  uint8_t lsb, msb;
  util_get_LSB(res, &lsb);
  util_get_MSB(res, &msb);

  uint8_t st;
  if (timer_get_conf(timer, &st) != 0) return 1;

  uint8_t cmd = (st & 0x0F) | (timer << 6) | 0x30;
  if (sys_outb(TIMER_CTRL, cmd) != 0) return 1;
  if (sys_outb(TIMER_0 + timer, lsb) != 0 || sys_outb(TIMER_0 + timer, msb) != 0) return 1;
  
  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  hook_id = TIMER0_IRQ;
  
  if (bit_no == NULL)return 1;
  *bit_no = hook_id; 
  
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != 0) return 1;
  
  return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id) != 0) return 1;
  return 0;
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (timer > 2 || st == NULL) return 1;
  
  uint8_t cmd = TIMER_RB_CMD | TIMER_RB_COUNT_| TIMER_RB_SEL(timer);
  if (sys_outb(TIMER_CTRL,cmd) != 0) return 1;
  
  uint32_t temp;
  if (sys_inb(TIMER_0+timer, &temp) != 0) return 1;
  
  *st = temp;
  
  return 0;
}
