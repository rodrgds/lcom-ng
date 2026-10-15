#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int timer_hook_id = 0;
uint32_t counter = 0;
int timer_frequency = 1193182; // Default frequency of the timer in Hz

int(timer_set_frequency)(uint8_t timer, uint32_t freq) {

  unsigned char st;
  timer_get_conf(timer, &st);

  // change initialization mode to LSB first then MSB and keep the rest
  st = (st & (0x0F)) | TIMER_LSB_MSB; // 0x0F is 00001111

  uint32_t newFreq = TIMER_FREQ / freq;
  uint8_t lsb, msb;

  util_get_LSB(newFreq, &lsb);
  util_get_MSB(newFreq, &msb);

  uint8_t selected_timer;
  switch (timer) {
  case 0:
    st |= TIMER_SEL0;
    selected_timer = TIMER_0;
    break;
  case 1:
    st |= TIMER_SEL1;
    selected_timer = TIMER_1;
    break;
  case 2:
    st |= TIMER_SEL2;
    selected_timer = TIMER_2;
    break;
  default:
    return 1;
    break;
  }

  timer_frequency = (int)freq;

  // send control word to timer control register
  sys_outb(TIMER_CTRL, st);

  // send the new frequency to the timer
  sys_outb(selected_timer, lsb);
  sys_outb(selected_timer, msb);

  return 0;
}

int(timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = BIT(timer_hook_id); // "timer_subscribe_int() must return, via its input argument, the
                                // value that it has passed to the kernel in the third argument of
                                // sys_irqsetpolicy()."
  // The policy you should specify in sys_irqsetpolicy() is IRQ_REENABLE, so that the generic
  // interrupt handler will acknowledge the interrupt
  sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id);
  return 0;
}

int(timer_unsubscribe_int)() {
  sys_irqrmpolicy(&timer_hook_id);
  return 0;
}

void(timer_int_handler)() {
  counter++;
}

int(timer_get_conf)(uint8_t timer, uint8_t *st) {

  // construct readback command
  // see more here (https://pages.up.pt/~up722898/aulas/lcom2425/lab2/lab2_03.html#sect3.3)
  uint8_t readback = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

  // send the readback command to the timer control register
  sys_outb(TIMER_CTRL, readback);

  // read the timer configuration
  // TIMER_0 is the base address for timer 0
  // TIMER_0 + timer is the address of the timer we want to read
  // fills st with the configuration of the timer
  util_sys_inb(TIMER_0 + timer, st);

  return 0;
}

int(timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {

  union timer_status_field_val conf;

  if (field == tsf_all) {
    conf.byte = st;
  } else if (field == tsf_initial) {

    // extract the initial count mode
    // see more here (https://pages.up.pt/~up722898/aulas/lcom2425/lab2/lab2_03.html#sect3.2)
    st = (st >> 4) & 0x03;

    switch (st) {
    case 1:
      // 01 is LSB
      conf.in_mode = LSB_only;
      break;
    case 2:
      // 10 is MSB
      conf.in_mode = MSB_only;
      break;
    case 3:
      // 11 is LSB followed by MSB
      conf.in_mode = MSB_after_LSB;
      break;
    default:
      conf.in_mode = INVAL_val;
      break;
    }
  } else if (field == tsf_mode) {

    // extract the operating mode
    // see more here (https://pages.up.pt/~up722898/aulas/lcom2425/lab2/lab2_03.html#sect3.2)
    st = (st >> 1) & 0x07; // 0x07 is 00000111 00000

    switch (st) {
    case 6: // special "compatibility" values
      conf.count_mode = 2;
      break;
    case 7: // special "compatibility" values
      conf.count_mode = 3;
      break;
    default:
      conf.count_mode = st;
      break;
    }
  } else if (field == tsf_base) {
    conf.bcd = st & 0x01; // gets LSB from control word (counting base)
  }

  timer_print_config(timer, field, conf);

  return 0;
}
