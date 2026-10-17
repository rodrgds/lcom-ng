#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int global_counter = 0;
static int hookID = 0;

int(timer_set_frequency)(uint8_t timer, uint32_t freq) {

  uint32_t div = TIMER_FREQ / freq;
  uint8_t lsb, msb;
  uint8_t st;

  //Check if indicated timer is valid
  if (timer != 0 && timer != 1 && timer != 2)
    return 1;

  //Check value of div --> must be lower than 2^16(bit)=65536
  if (div >= 65536) {
    printf("Invalid frequency!\n");
    return 1;
  }

  //LSB and MSB
  util_get_LSB(div, &lsb);
  util_get_MSB(div, &msb);

  //Read timer configuration
  if (timer_get_conf(timer, &st) != OK) {
    printf("Failed reading timer configuration!");
    return 1;
  }

  //keep the 4 lsb
  st = 0x0F & st;

  switch (timer) {
    case 0: {
      st |= TIMER_LSB_MSB | TIMER_SEL0;
      if (sys_outb(TIMER_CTRL, st) != OK || sys_outb(TIMER_0, lsb) != OK || sys_outb(TIMER_0, msb) != OK) {
        printf("sys_outb failed at timer 0\n");
        return 1;
      }
      break;
    }
    case 1: {
      st |= TIMER_LSB_MSB | TIMER_SEL1;
      if (sys_outb(TIMER_CTRL, st) != OK || sys_outb(TIMER_1, lsb) != OK || sys_outb(TIMER_1, msb) != OK) {
        printf("sys_outb failed at timer 1\n");
        return 1;
      }
      break;
    }
    case 2: {
      st |= TIMER_LSB_MSB | TIMER_SEL2;
      if (sys_outb(TIMER_CTRL, st) != OK || sys_outb(TIMER_2, lsb) != OK || sys_outb(TIMER_2, msb) != OK) {
        printf("sys_outb failed at timer 2\n");
        return 1;
      }
      break;
    }
    default: {
      return 1;
      break;
    }
  }
  return 0;
}

int(timer_subscribe_int)(uint8_t *bit_no) {

  hookID = *bit_no;

  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hookID) != OK)
    return 1;

  return 0;
}

int(timer_unsubscribe_int)() {

  if(sys_irqrmpolicy(&hookID)!=OK)
    return 1;

  return 0;
}

void(timer_int_handler)() {
  global_counter++;
}

int(timer_get_conf)(uint8_t timer, uint8_t *st) {

  if (timer != 0 && timer != 1 && timer != 2)
    return 1;

  uint8_t read_rb; //variable used to store read configuration

  read_rb = TIMER_RB_CMD | TIMER_RB_SEL(timer) | TIMER_RB_COUNT_;

  //writes to the control register
  if (sys_outb(TIMER_CTRL, read_rb) != OK)
    return 1;

  if (util_sys_inb(TIMER_0 + timer, st) != OK)
    return 1;
  return 0;
}

int(timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {

  union timer_status_field_val val;
  //unsigned char mode =  ( BIT(3) | BIT(2) | BIT(1) );

  switch (field) {

    case tsf_all:
      val.byte = st;
      break;

    case tsf_initial:
      //LSB mode
      if ((st & TIMER_LSB_MSB) == TIMER_LSB)
        val.in_mode = LSB_only;
      //MSB mode
      else if ((st & TIMER_LSB_MSB) == TIMER_MSB)
        val.in_mode = MSB_only;
      //MSB_after_LSB mode
      else if ((st & TIMER_LSB_MSB) == TIMER_LSB_MSB)
        val.in_mode = MSB_after_LSB;
      else
        val.in_mode = INVAL_val;
      break;

    case tsf_mode:
      //mode 0 (=000)
      if ((st & BIT(2)) == 0)
        val.count_mode = 0;
      //mode 1 (=001)
      if ((st & BIT(1)) == BIT(1))
        val.count_mode = 1;
      //mode 2 (=010)
      if ((st & BIT(2)) == BIT(2))
        val.count_mode = 2;
      //mode 3 (=011)
      if ((st & (BIT(1) | BIT(2))) == (BIT(1) | BIT(2)))
        val.count_mode = 3;
      //mode 4 (=100)
      if ((st & BIT(3)) == BIT(3))
        val.count_mode = 4;
      //mode 5 (=101)
      if ((st & (BIT(1) | BIT(3))) == (BIT(1) | BIT(3)))
        val.count_mode = 5;
      break;

    case tsf_base:
      val.bcd = st & TIMER_BCD;
      break;

    default:
      break;
  }
  timer_print_config(timer, field, val);
  return 0;
}
