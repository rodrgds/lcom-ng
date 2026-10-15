#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"
#include "timer_info.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (freq > TIMER_FREQ || freq < TIMER_MIN_FREQ) { // range for frequency
    printf("invalid frequency\n");
    return 1;
  }

  uint8_t counting_mode_base, init_mode, counter_sel; // control word fields

  uint8_t st = 0;
  if (timer_get_conf(timer, &st)) // read configuration first
    return 1;
  uint8_t mask = 0x0F; // 4 LSB (counting mode and counting base)
  counting_mode_base = st & mask;

  init_mode = TIMER_LSB_MSB; // LSB followed by MSB

  int port; // timer register
  switch (timer) {
    case 0:
      counter_sel = TIMER_SEL0; // [00]000000
      port = TIMER_0; // 0x40
      break;
    case 1:
      counter_sel = TIMER_SEL1; // [01]000000
      port = TIMER_1; // 0x41
      break;
    case 2:
      counter_sel = TIMER_SEL2; // [10]000000
      port = TIMER_2; // 0x42
      break;
    default:
      printf("invalid timer\n");
      return 1;
  }

  uint8_t ctrl_word = counter_sel | init_mode | counting_mode_base;
  int ctrl_register = TIMER_CTRL; // 0x43
  if (sys_outb(ctrl_register, ctrl_word)) { // write control word to the control register
    printf("no proper writing\n");
    return 1;
  }

  uint16_t counter_initial_val = (uint16_t) (TIMER_FREQ / freq); // freq=clock/div -> div=clock/freq

  /* load counter initial value */
  uint8_t LSB = 0, MSB = 0;
  if (util_get_LSB(counter_initial_val, &LSB))
    return 1;
  if (util_get_MSB(counter_initial_val, &MSB))
    return 1;
  /* write to the counter register */
  if (sys_outb(port, LSB)) {
    printf("no proper writing\n");
    return 1;
  }
  if (sys_outb(port, MSB)) {
    printf("no proper writing\n");
    return 1;
  }

  return 0;
}

static int hook_id;

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) {
    printf("null pointer\n");
    return 1;
  }

  hook_id = TIMER0_IRQ;
  *bit_no = hook_id;
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id)) { // interrupt notification subscription
    printf("bad subscription\n");
    return 1;
  }

  return 0;
}

int (timer_unsubscribe_int)() {
  /* 
  unsubscribe a previous subscription of the interrupt 
  notification associated with the specified hook_id 
  */
  if (sys_irqrmpolicy(&hook_id)) {
    printf("bad unsubscription\n");
    return 1;
  }

  return 0;
}

uint32_t int_counter = 0;

void (timer_int_handler)() {
  int_counter++; // counter incremented on every interrupt
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (st == NULL) {
    printf("null pointer\n");
    return 1;
  }
  
  int port; // timer register
  switch (timer) {
    case 0:
      port = TIMER_0; // 0x40
      break;
    case 1:
      port = TIMER_1; // 0x41
      break;
    case 2:
      port = TIMER_2; // 0x42
      break;
    default:
      printf("invalid timer\n");
      return 1;
  }

  int ctrl_register = TIMER_CTRL; // 0x43
  u32_t rb_command = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer); // appropriate read-back
  if (sys_outb(ctrl_register, rb_command)) { // to write command to the control register
    printf("no proper writing\n");
    return 1;
  }

  if (util_sys_inb(port, st)) // to read the configuration from this timer
    return 1;

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val conf;
  uint8_t init;
  switch (field) {
    case tsf_all: // status
      conf.byte = st; // status byte
      break;
    case tsf_initial: // initialization mode
      init = (st & TIMER_INIT_MODE) >> 4; // to isolate value
      switch (init) {
        case 0:
          conf.in_mode = INVAL_val;
          break;
        case 1:
          conf.in_mode = LSB_only; // 01
          break;
        case 2:
          conf.in_mode = MSB_only; // 10
          break;
        case 3:
          conf.in_mode = MSB_after_LSB; // 11
          break;
        default:
          return 1;
      }
      break;
    case tsf_mode: // counting mode
      conf.count_mode = (st & TIMER_OP_MODE) >> 1; // to isolate value
      if (conf.count_mode == TIMER_INCOMP_2 || conf.count_mode == TIMER_INCOMP_3) { // 110 or 111
        uint8_t mask = 0x03; // ...011
        conf.count_mode &= mask; // will result in 010 (mode 2) or 011 (mode 3)
      }
      break;
    case tsf_base: // counting base
      conf.bcd = st & TIMER_BCD; // to isolate value (1 = true)
      break;
    default:
      printf("invalid field\n");
      return 1;
  }

  if (timer_print_config(timer, field, conf)) // to check
    return 1;

  return 0;
}
