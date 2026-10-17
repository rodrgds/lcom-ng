#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

static int hook_id;
int timer_counter = 0;

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = val & 0x00FF; 
  return 1;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = (val & 0xFF00) >> 8;
  return 1;
}

int(timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint16_t init = TIMER_FREQ / freq;

  uint8_t cmd = TIMER_RB_CMD | TIMER_RB_COUNT_ | BIT(timer + 1);
  if (sys_outb(TIMER_CTRL, cmd) != OK) {
    printf("Error in sys_outb()\n");
    return 1;
  }

  uint8_t previous;
  timer_get_conf(timer, &previous);

  // select the timer, with LSB->MSB and keep bits 0-4 the same
  cmd = (timer << 6) | TIMER_LSB_MSB | (previous & 15);
  uint8_t lsb, msb;
  util_get_LSB(init, &lsb);
  util_get_MSB(init, &msb);

  if (sys_outb(TIMER_0 + timer, lsb) != OK) {
    printf("Error in sys_outb()\n");
    return 1;
  }

  if (sys_outb(TIMER_0 + timer, msb) != OK) {
    printf("Error in sys_outb()\n");
    return 1;
  }

  return 1;
}

int(timer_subscribe_int)(uint8_t *bit_no) {
  hook_id = *bit_no;
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id);
}

int(timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id);
  
}

void(timer_int_handler)() {
  timer_counter++;
}

int(timer_get_conf)(uint8_t timer, uint8_t *st) {

  uint8_t cmd = TIMER_RB_CMD | TIMER_RB_COUNT_ | BIT(timer + 1);
  if (sys_outb(TIMER_CTRL, cmd) != OK) {
    printf("Error in sys_outb()\n");
    return 1;
  }

  uint32_t lst;
  if (sys_inb(TIMER_0 + timer, &lst) != OK) {
    printf("Error in sys_inb()\n");
    return 1;
  }
  else {
    //printf("status = 0x%x\n", lst);
    *st = lst;
  }

  return 0;
}

int(timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  union timer_status_field_val value;
  enum timer_init arr[3] = {MSB_after_LSB, MSB_only, LSB_only};
  switch (field) {
    case tsf_all:
      value.byte = st;
      break;
    case tsf_initial:
      value.in_mode = arr[(st & (BIT(5) | BIT(4)) >> 4)];
      break;
    case tsf_mode:
      value.count_mode = st & (BIT(1) | BIT(2) | BIT(3));
      if (value.count_mode >= 6) {
        value.count_mode -= 4;
      }
      break;
    case tsf_base:
      value.bcd = st & BIT(0);
      break;
    default:
      return 1;
  }
  timer_print_config(timer, field, value);

  return 0;
}
