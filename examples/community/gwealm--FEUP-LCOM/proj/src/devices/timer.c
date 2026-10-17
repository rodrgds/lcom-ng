#include <stdint.h>

#include "devices/timer.h"

#include "events/events.h"

static int hook_id;
static int irq_set;

static int frequency;

int (timer_init)() {
  frequency = 30;

  int err_no;
  if ((err_no = timer_set_frequency(0, frequency)) != OKAY) {
    return err_no;
  }
  

  if (timer_subscribe_interrupts() != 0) {
    printf("timer_subs: could not subscribe to interrupts\n");
    return TIMER_EINT;
  }

  event_on(INTERRUPT, timer_handle_interrupt);

  return OKAY;
}

int (timer_cleanup)() {
  if (timer_unsubscribe_interrupts() != 0) {
    printf("timer_cleanup: could not unsubscribe interrupts\n");
    return TIMER_EINT;
  }

  int err_no;
  if ((err_no = timer_set_frequency(0, 60)) != 0) {
    return err_no;
  }

  event_unregister(INTERRUPT, timer_handle_interrupt);
  return OKAY;
}

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (freq < 19 || freq > TIMER_FREQ)
    return 1;

  uint8_t st = 0;

  int err_no;
  if ((err_no = timer_get_conf(timer, &st)) != 0) {
    return err_no;
  }

  uint8_t cmd = (st & TIMER_DONT_CHANGE) | TIMER_LSB_MSB | TIMER_SEL(timer);

  uint16_t div = TIMER_FREQ / freq;
  uint8_t msb, lsb;
  util_get_LSB(div, &lsb);
  util_get_MSB(div, &msb);

  if (sys_outb(TIMER_CTRL, cmd) != 0) {
    return TIMER_EWRITE;
  }

  if (sys_outb(TIMER_0 + timer, lsb) != 0) {
    return TIMER_EWRITE;
  }
  
  if (sys_outb(TIMER_0 + timer, msb) != 0) {
    return TIMER_EWRITE;
  }
  
  return OKAY;
}

int (timer_subscribe_interrupts)() {
  hook_id = TIMER0_IRQ; irq_set = BIT(hook_id);
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id);
}

int (timer_unsubscribe_interrupts)() {
  return sys_irqrmpolicy(&hook_id);
}

void (timer_handle_interrupt)(struct event *event) {
  if (*((int*) event->data) & irq_set) {
    event_dispatch(TIMER_TICK, NULL, 0);
  }
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {

  uint8_t read_back_cmd = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  if (sys_outb(TIMER_CTRL, read_back_cmd) != 0) {
    return TIMER_EWRITE;
  }

  if (util_sys_inb(TIMER_0 + timer, st) != 0) {
    return TIMER_EREAD;
  }

  return OKAY;
}

unsigned long (timer_get_frequency)() {
  return frequency;
}
