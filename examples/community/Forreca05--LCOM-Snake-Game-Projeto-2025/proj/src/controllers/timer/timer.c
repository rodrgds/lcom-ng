#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int timer_count = 0;
int hook_id_timer = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  // Bound checking
  if (freq < 19 || freq > TIMER_FREQ) return 1;

  // Read the control word
  uint8_t st;
  if (timer_get_conf(timer, &st) != 0) return 1;
  
  // Reset the bits corresponding to the selected timer and the initialization mode
  st &= 0x0F;

  // Set the initialization mode to LSB followed by MSB
  st |= TIMER_LSB_MSB;

  // Calculate the divisor and divide it into MSB and LSB
  uint16_t divisor = TIMER_FREQ / freq;
  uint8_t MSB, LSB;
  util_get_MSB(divisor, &MSB);
  util_get_LSB(divisor, &LSB);

  // Write the appropriate timer bit into the control word
  switch (timer) {
    case 0: st |= TIMER_SEL0; break;
    case 1: st |= TIMER_SEL1; break;
    case 2: st |= TIMER_SEL2;
  }

  // Write the divisor into the given timer
  if (sys_outb(TIMER_CTRL, st) != OK) return 1;
  if (sys_outb(TIMER_0 + timer, LSB) != OK) return 1;
  if (sys_outb(TIMER_0 + timer, MSB) != OK) return 1;

  return 0;
}

int (timer_subscribe_int)(uint8_t *irq_set) {
  if (irq_set == NULL) return 1;
  *irq_set = BIT(hook_id_timer);
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id_timer) != 0) return 1;
  return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id_timer) != 0) return 1;
  return 0;
}

void (timer_int_handler)() {
  timer_count++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (st == NULL || timer > 2 || timer < 0) return 1;
  uint32_t command = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  if(sys_outb(TIMER_CTRL, command)!=0) return 1;
  if(util_sys_inb(TIMER_0 + timer, st)!=0) return 1;
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  union timer_status_field_val tsfv;
  switch (field) {
    case tsf_all: tsfv.byte = st; break;
    case tsf_base: tsfv.bcd = st & TIMER_BCD; break;
    case tsf_initial: 
      switch (st & 0x30) {
        case 0x10: tsfv.in_mode = LSB_only;break;
        case 0x20: tsfv.in_mode = MSB_only;break;
        case 0x30: tsfv.in_mode = MSB_after_LSB; break;
        default:
          tsfv.in_mode= INVAL_val;
      }
      break;
    case tsf_mode: 
      st=(st & 0xE) >> 1;
      if(st==6) st=0x2;
      else if(st==7) st=0x3;
      tsfv.count_mode = st;
  }
  timer_print_config(timer, field, tsfv);
  return 0;
}
