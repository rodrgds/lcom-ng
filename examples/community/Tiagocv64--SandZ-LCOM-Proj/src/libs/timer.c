#include <lcom/lcf.h>
#include <lcom/timer.h>
#include "timer.h"
#include <stdint.h>

#include "i8254.h"

unsigned long long counter = 0;
static int hook_timer = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

  if (timer < 0 || timer > 2)
    return 1;

  if (freq & 0xFF00)
    return 1;

  if (freq == 0)
    return 1;

  uint8_t t;
  timer_get_conf(timer, &t);

  t = t & 0x0F;
  t = t | TIMER_LSB_MSB | (timer << 6);

  if (sys_outb(TIMER_CTRL, t))
    return 1;

  uint8_t lsb, msb;
  uint16_t total = TIMER_FREQ/freq;
  uint32_t pass;

  util_get_LSB(total, &lsb);
  util_get_MSB(total, &msb);

  pass = (uint32_t) lsb;

  if (sys_outb(TIMER_0 + timer, pass))
    return 1;

  pass = (uint32_t) msb;

  if (sys_outb(TIMER_0 + timer, pass))
    return 1;

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {

  int idhook_back = hook_timer;

  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_timer) == OK){
    *bit_no = BIT(idhook_back);
    return 0;
  } else {
    return 1;
  }
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_timer) == OK)
    return 0;
  else
    return 1;
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {

  if (timer < 0 || timer > 2)
    return 1;

	uint8_t n = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

	if (sys_outb(TIMER_CTRL, n))
    return 1;

	uint32_t status;

	if (sys_inb(TIMER_0 + timer, &status))
    return 1;

	*st = (uint8_t) status;

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {

  if (timer < 0 || timer > 2)
    return 1;

	union timer_status_field_val val;


  switch(field){
    case all:

      val.byte = st;
      break;

    case initial:

      if ((st & TIMER_LSB_MSB) == TIMER_LSB_MSB)
        val.in_mode = MSB_after_LSB;
      else if (st & TIMER_LSB)
        val.in_mode = LSB_only;
      else if (st & TIMER_MSB)
        val.in_mode = MSB_only;
      else
        val.in_mode = INVAL_val;

      break;

    case mode:

      switch((st >> 1) & 7){

        case 0:
          val.count_mode = 0;
        break;
        case 1:
          val.count_mode = 1;
        break;
        case 2: case 6:
          val.count_mode = 2;
        break;
        case 3: case 7:
          val.count_mode = 3;
        break;
        case 4:
          val.count_mode = 4;
        break;
        case 5:
          val.count_mode = 5;
        break;

      }

      break;

    case base:

      val.bcd = (bool) (TIMER_BCD & st);
      break;

  }
	timer_print_config(timer, field , val);
	return 0;
}

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = (uint8_t) val;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = (uint8_t) (val >> 8);
  return 0;
}
