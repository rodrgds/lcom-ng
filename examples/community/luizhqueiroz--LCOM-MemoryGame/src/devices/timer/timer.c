#include <lcom/lcf.h>

#include <stdint.h>

#include "timer.h"
#include "i8254.h"

#define SEL_4_LSB 0x0F;

int timer_int_counter = 0;
static int hook_id;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  
  uint8_t st;
  int port;
  
  if (timer_get_conf(timer, &st)) {
    return 1;
  }

  st &= SEL_4_LSB;
  st |= TIMER_LSB_MSB;

  switch (timer) {
    case 0:
      st |= TIMER_SEL0;
      port = TIMER_0;
      break;
    case 1:
      st |= TIMER_SEL1;
      port = TIMER_1;
      break;
    case 2:
      st |= TIMER_SEL2;
      port = TIMER_2;
      break;

    default:
      printf("Invalid Timer");
      return 1;
  } 

  if (sys_outb(TIMER_CTRL, st)) {
    return 1;
  }
  
  uint16_t value = TIMER_FREQ/freq;
  uint8_t lsb = 0x00;
  uint8_t msb = 0x00;

  if (util_get_LSB(value, &lsb)) {
    return 1;
  }

  if (util_get_MSB(value, &msb)) {
    return 1;
  }

  if (sys_outb(port, lsb)) {
    return 1;
  }
  if (sys_outb(port, msb)) {
    return 1;
  }

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) {
    return 1;
  }

  hook_id = TIMER0_IRQ;
  *bit_no = hook_id;

  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id)) {
    return 1;
  }

  return 0;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  if (sys_irqrmpolicy(&hook_id)) {
    return 1;
  }

  return 0;
}

void (timer_ih)() {
  timer_int_counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {

  uint8_t RB_CMD = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

  if (sys_outb(TIMER_CTRL, RB_CMD)) {
    return 1;
  }

  switch (timer) {
    case 0:
      if (util_sys_inb(TIMER_0, st)) {
        return 1;
      }
      break;
    case 1: 
      if (util_sys_inb(TIMER_1, st)) {
        return 1;
      }
      break;
    case 2:
      if (util_sys_inb(TIMER_2, st)) {
        return 1;
      }
      break;

    default: 
      printf("Invalid Timer");
      return 1;
  }

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {

  union timer_status_field_val conf;
  switch (field) {
    case tsf_all: 
      conf.byte = st;
      if (timer_print_config(timer, field, conf)) {
        return 1;
      }
      break;
    case tsf_initial:
      switch (((st & BIT(5)) | (st & BIT(4))) >> 4) {
        case 1:
          conf.in_mode = LSB_only; 
          if (timer_print_config(timer, field, conf)){
            return 1;
          }
          break;

        case 2:
          conf.in_mode = MSB_only;
          if (timer_print_config(timer, field, conf)) {
            return 1;
          }
          break;

        case 3: 
          conf.in_mode = MSB_after_LSB;
          if (timer_print_config(timer, field, conf)) {
            return 1;
          }
          break;

        default:
          conf.in_mode = INVAL_val;
          if (timer_print_config(timer, field, conf)) {
            return 1;
          }
        }
      break;
    case tsf_mode:
      conf.count_mode = ((st & (BIT(1) | BIT(2) | BIT(3))) >> 1);
      if (timer_print_config(timer, field, conf)) {
        return 1;
      }
      break;
    case tsf_base:
      conf.bcd = (st & BIT(0));
      if (timer_print_config(timer, field, conf)) {
        return 1;
      }
      break;

      default:
        printf("Invalid field");
        return 1;
  }

  return 0;
}
