#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>
#include "macros.h"

int counter = 0;
int timer_hook_id = TIMER0_IRQ;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint8_t st;

  if (freq >= TIMER_FREQ || freq < TIMER_MIN_FREQ){
    printf("Invalid frequency %d\n", freq);
  }

  // Read timer's configuration
  if (timer_get_conf(timer, &st) != OK)
     return 1;

  // Assemble control word selecting the timer, initialization mode and keeping the 4 lsb's we read from the config
  uint8_t controlWord = TIMER_SEL_SHIFT(timer) | TIMER_LSB_MSB | (st & TIMER_4LSB_MASK);

  // Write the word to the control register
  if (sys_outb(TIMER_CTRL, controlWord)){
    return 1;
  }
  // Compute frequency divider

  uint16_t div = TIMER_FREQ / freq;
  uint8_t lsb, msb;

  // Extract the least and most signficant bytes
  if (util_get_LSB(div, &lsb) || util_get_MSB(div, &msb))
    return 1;

  // Write them in memory according to the defined order
  if (sys_outb(TIMER_0 + timer, lsb) != OK)
    return 1;

  if (sys_outb(TIMER_0 + timer, msb) != OK)
    return 1;

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = timer_hook_id;

  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) != OK){
    printf("Error while subscribing interruptions\n");
    return 1;
  }

  return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&timer_hook_id) != OK){
    printf("Error while unsubscribing interruptions\n");
    return 1;
  }

  return 0;
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (timer < 0 || timer > 2){
    printf("Invalid timer %d\n", timer);
    return 1;
  }

  uint8_t readbackCommand = 0;
  readbackCommand = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

  if (sys_outb(TIMER_CTRL, readbackCommand) != OK){
      printf("Error while writing the readback command\n");
      return 1;
  }

  if (util_sys_inb(TIMER_0 + timer, st) != 0){
    printf("Error while reading from the timer\n");
    return 1;
  }

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val conf;

  if (field == tsf_all){
    conf.byte = st;
  }

  else if (field == tsf_initial){
    conf.in_mode = (enum timer_init) (st & (TIMER_LSB_MSB)) >> 4;
  }

  else if (field == tsf_mode){
    conf.count_mode = (st & (TIMER_OPER_MODE)) >> 1;
  }

  else if (field == tsf_base){
    conf.bcd = (bool) (st & TIMER_BCD);
  }

  else {
    printf("Invalid field %d\n", field);
    return 1;
  }

  timer_print_config(timer, field, conf);

  return 0;
}
