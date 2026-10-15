#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int hook_id_timer=0;
uint32_t counter_timer=0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (timer < 0 || timer > 2) {
    printf("Invalid timer number.\n");
    return 1;
  }

  if (freq < 19 || freq > TIMER_FREQ) {
    printf("Frequency out of range.\n");
    return 1;
  }

  uint16_t divisor = TIMER_FREQ / freq;

  // Read current timer configuration to preserve bits
  uint8_t current_status;
  if (timer_get_conf(timer, &current_status) != 0) {
    printf("Failed to read timer status.\n");
    return 1;
  }

  // Preserve the last 4 bits (0x0F) from the current status
  uint8_t preserved_bits = current_status & 0x0F;

  // Set the control word with preserved bits
  uint8_t control_word;
  switch (timer) {
    case 0:
      control_word = (TIMER_SEL0 | TIMER_LSB_MSB | TIMER_SQR_WAVE | TIMER_BIN) | preserved_bits;
      break;
    case 1:
      control_word = (TIMER_SEL1 | TIMER_LSB_MSB | TIMER_SQR_WAVE | TIMER_BIN) | preserved_bits;
      break;
    case 2:
      control_word = (TIMER_SEL2 | TIMER_LSB_MSB | TIMER_SQR_WAVE | TIMER_BIN) | preserved_bits;
      break;
    default:
      printf("Invalid timer number.\n");
      return 1;
  }

  // Write the updated control word to the control register
  if (sys_outb(TIMER_CTRL, control_word) != OK) {
    printf("Failed to write control word.\n");
    return 1;
  }

  uint8_t lsb, msb;
  util_get_LSB(divisor, &lsb);
  util_get_MSB(divisor, &msb);

  if (sys_outb(TIMER_0 + timer, lsb) != OK) {
    printf("Failed to write LSB.\n");
    return 1;
  }

  if (sys_outb(TIMER_0 + timer, msb) != OK) {
    printf("Failed to write MSB.\n");
    return 1;
  }

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;

  *bit_no = BIT(hook_id_timer);

  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id_timer) != OK) {
        return 1;
    }
  return 0;
}


int (timer_unsubscribe_int)() {
    if (sys_irqrmpolicy(&hook_id_timer) != OK) {
        return 1;
    }
    return 0;
}


void (timer_int_handler)() {
  counter_timer++;
}


int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (timer > 2 || st == NULL) {
        return 1;
    }

    uint8_t read_back_command = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

    if (sys_outb(TIMER_CTRL, read_back_command) != OK) {
        return 1;
    }

    if (util_sys_inb(TIMER_0 + timer, st) != OK) {
        return 1;
    }

    return 0;
}
int (timer_display_conf)(uint8_t timer, uint8_t st,
  enum timer_status_field field) {
if (timer > 2){
return 1;
}
union timer_status_field_val value;

switch (field){
case tsf_all:
value.byte = st;
break;

case tsf_initial:
value.in_mode = (st & (BIT(4) | BIT(5))) >> 4;
break;

case tsf_mode:
value.count_mode = (st >> 1) & (BIT(2) | BIT(1) | BIT(0));

if(value.count_mode == 6){
value.count_mode = 2;
}

if(value.count_mode == 7){
value.count_mode = 3;
}
break;

case tsf_base:
if ((st & BIT(1)) == 1){
value.bcd = true;
}
else{
value.bcd = false;
}
break;

default:
return 1;  
}
timer_print_config(timer, field, value); 
return 0;
}





