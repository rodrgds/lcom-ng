#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "src/drivers/i8254.h"

int hook_id = 0;
int timer_counter;

int(timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (freq > TIMER_FREQ) {
    printf("Frequency out of bounds \n");
    return 1;
  }
  uint8_t config = 0;
  if (timer_get_conf(timer, &config) != 0) {
    printf("timer_get_conf error \n");
    return 1;
  }
  uint8_t cw = 0;

  // 7 6 5 4 3 2 1 0
  // 0 0 1 1 C C C C

  cw = ((BIT(3) | BIT(2) | BIT(1) | BIT(0)) & config) | (BIT(4) | BIT(5)) | (timer << 6);
  sys_outb(TIMER_CTRL, cw);

  uint16_t div = TIMER_FREQ / freq;
  uint8_t lsb, msb;
  util_get_LSB(div, &lsb);
  util_get_MSB(div, &msb);
  sys_outb(0x40 + timer, lsb);
  sys_outb(0x40 + timer, msb);
  return 0;
}

int(timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = BIT(hook_id);
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != 0) {
    printf("Failed to set policy for timer interrupt.\n");
    return 1;
  }
  return 0;
}

int(timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id) != 0) {
    printf("Failed to remove policy for timer interrupt. \n");
    return 1;
  }
  return 0;
}

void(timer_int_handler)() {
  timer_counter++;
}

int(timer_get_conf)(uint8_t timer, uint8_t *st) {
  // Bits 7,6 are for Read-Back Command, Bit 5 is for reading the Counter Value (1-to not read,0-to read), Bit 4 is for reading the Programed Mode, Bit 3 is 1 if the Counter 2 is selected, Bit 2 is 1 if the Counter 1 is selected, Bit 1 is 1 if the Counter 0 is selected, Bit 0 is Reserved
  //  7 6 5 4 3 2 1 0
  // [1,1,1,0,0,0,1,0]
  // 3,2,1 are for selecting timer
  // 1 0 0 Timer 2
  // 0 1 0 Timer 1
  // 0 0 1 Timer 0
  uint8_t control_word = 0;
  control_word = BIT(7) | BIT(6) | BIT(timer + 1) | BIT(5);
  sys_outb(TIMER_CTRL, control_word);
  // Timer 0 port = 0x40
  uint8_t timer_port = 0x40 + timer;
  util_sys_inb(timer_port, st);
  return 0;
}

int(timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  // TIMER STATUS BYTE
  // Bit 7 is the Output
  // Bit 6 is the Null Count
  // Bits 5,4 are the Type of access
  // Bits 3,2,1 are the Programed Mode
  // Bit 0 is the represents if the counter is binary ou BCB counter

  union timer_status_field_val conf;
  switch (field) // Selects the Task to be executed
  {
    case tsf_all:
      conf.byte = st;
      break;

    case tsf_initial:
      conf.in_mode = (st & TIMER_LSB_MSB) >> 4; // gets the 5th/4th bit of the state, that represents the initial mode (TIMER_LSB_MSB = (BIT(4) | Bit(5)))
      break;

    case tsf_mode:
      conf.count_mode = (st & (BIT(3) | BIT(2) | BIT(1))) >> 1; // gets bit 1,2,3 of state, that represent the Programed Mode
      break;

    case tsf_base:
      conf.bcd = st & TIMER_BCD; // gets the last bit of the state, that represents if the counter is binary ou BCB counter
      break;

    default: // non existent task returns 1
      return 1;
      break;
  }

  timer_print_config(timer, field, conf); // given by LCF
  return 0;
}

