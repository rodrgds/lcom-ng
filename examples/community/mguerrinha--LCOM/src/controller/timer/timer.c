#include "timer.h"

int timer_counter = 0;
int timer_hook_id = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (freq < 19 || freq > TIMER_FREQ) return 1;
  uint8_t conf;
  uint8_t timerSelected;
  if (timer_get_conf(timer, &conf)) return 1;
  conf = (conf & 0x0f) | TIMER_LSB | TIMER_MSB;
  uint16_t timerFrequency = TIMER_FREQ / freq;
  uint8_t lsb, msb;
  if (util_get_LSB(timerFrequency, &lsb)) return 1;
  if (util_get_MSB(timerFrequency, &msb)) return 1;
  switch (timer)
  {
  case 0:
    conf |= TIMER_SEL0;
    timerSelected = TIMER_0;
    break;
  case 1:
    conf |= TIMER_SEL1;
    timerSelected = TIMER_1;
    break;
  case 2:
    conf |= TIMER_SEL2;
    timerSelected = TIMER_2;
    break;
  default:
    return 1;
    break;
  }
  if (sys_outb(TIMER_CTRL, conf)) return 1;
  if (sys_outb(timerSelected, lsb)) return 1;
  if (sys_outb(timerSelected, msb)) return 1;
  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  *bit_no = BIT(timer_hook_id);
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id)) return 1;
  return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&timer_hook_id)) return 1;
  return 0;
}

void (timer_int_handler)() {
  timer_counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (st == NULL) return 1;
  uint8_t timerSelected;
  uint8_t command = BIT(7) | BIT(6) | BIT(5);
  switch (timer)
  {
  case 0:
    command |= TIMER_RB_SEL(0);
    timerSelected = TIMER_0;
    break;
  case 1:
    command |= TIMER_RB_SEL(1);
    timerSelected = TIMER_1;
    break;
  case 2:
    command |= TIMER_RB_SEL(2);
    timerSelected = TIMER_2;
    break;
  default:
    return 1;
    break;
  }
  if (sys_outb(0x43, command)) return 1;
  if (util_sys_inb(timerSelected, st)) return 1;
  return 0;
}
