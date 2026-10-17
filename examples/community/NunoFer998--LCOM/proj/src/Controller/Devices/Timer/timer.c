#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>
#include "timer.h"


int time_h_id=0;
int game_counter = 60;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (freq < MIN_FREQ || freq > CLOCK_FREQ) return 1;
  uint8_t con;
  if (timer_get_conf(timer, &con) != 0) return 0;
  con = (con & 0x0F) | (BIT(4) | BIT(5));
  if (timer == 1) {
    con = con | BIT(6);
  }
  else if (timer == 2) {
    con = con | BIT(7);
  }
  uint32_t n = CLOCK_FREQ / freq;
  uint8_t MSB, LSB;
  if (aux_get_LSB(n, &LSB) != 0) return 1;
  if (aux_get_MSB(n, &MSB) != 0) return 1;
  if (sys_outb(CONTROL_REG, con) != 0) return 1;
  if (sys_outb(TIMER_0 + timer, LSB) != 0) return 1;
  if (sys_outb(TIMER_0 + timer, MSB) != 0) return 1;
  return 0;
}

void (timer_int_handler)() {
  game_counter++;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  *bit_no = BIT(time_h_id);
  if (sys_irqsetpolicy(0, IRQ_REENABLE, &time_h_id) != 0) return 1;
  return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&time_h_id) != 0) return 1; 
  return 0;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (st == NULL) return 1;
  uint8_t Command = BIT(7)| BIT(6)| BIT(5)|BIT(timer + 1);
  if (sys_outb(CONTROL_REG,Command) != 0) return 1;
  if (aux_sys_inb(TIMER_0 + timer, st) != 0) return 1;
  return 0;
}
