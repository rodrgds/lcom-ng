#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>
#include "i8254.h"

uint32_t counter = 0; // Global counter variable to be incrmented by the interrupt handler.
int hook_id = 0; // hook_id to identify the notification subscriber.

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (timer > 2 || freq < 19 || freq > TIMER_FREQ) return 1; // Verifying input validity!
  // Upper limit: actual frequency of the timer (TIMER_FREQ).
  // Lower limit: the counter must be written in a uint16_t value (8 + 8); so, the value of the counter has to be at most
  // 2^16 - 1, or 65535. Because 1193182 / 65535 = ~ 18.207, if we select 18, then the counter would be ~ 66288 > 65535.
  // So, the minimum frequency that the wave can have is 19. 

  uint8_t read_status_word;  
  if (timer_get_conf(timer, &read_status_word) != 0) return 1;

  uint8_t new_control_word = read_status_word & 0xF; // Keep only the last 4 bits of the acquired status word.
  new_control_word |= TIMER_LSB_MSB; // Last year's ppt mentioned to preferrably use LSB followed by MSB as the initialization mode.
  // TODO: confirm if that is still the case!

  uint8_t timer_port = TIMER_0 + timer;

  if (timer == 1) new_control_word |= TIMER_SEL1; // Setting bit 6 to select timer 1.
  else if (timer == 2) new_control_word |= TIMER_SEL2; // Setting bit 7 to select timer 2.

  uint16_t starting_counter = TIMER_FREQ / freq; // Getting the initial counter value to configure the timer with.
  uint8_t starting_counter_lsb, starting_counter_msb;
  util_get_LSB(starting_counter, &starting_counter_lsb); // Extracting the LSB of the starting count value.
  util_get_MSB(starting_counter, &starting_counter_msb); // Extracting the MSB of the starting count value.
  
  if (sys_outb(TIMER_CTRL, new_control_word) != 0) return 1; // We send the defined control world to set up the timer...

  if (sys_outb(timer_port, starting_counter_lsb) != 0) return 1; // ... then send the LSB first (as defined in the configuration)...
  if (sys_outb(timer_port, starting_counter_msb) != 0) return 1; // ... followed by the MSB part.
  
  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;

  *bit_no = BIT(hook_id); // Sets the bit matching the value selected for the hook_id. This will be used to verify if a received notification
                          // matches the bit of the hook_id we configured for the timer 0 notifications.
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != 0) return 1; // TIMER0_IRQ (value 0) corresponds to the irq_line of timer 0;
                                                                           // we specify that this is the device we want to subscribe to,
                                                                           // and provide the defined hook_id that we can later use to cancel
                                                                           // this subscription; IRQ_REENABLE means that we keep the interrupt
                                                                           // line even after the notification is delivered (not set by default).
  
  return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id) != 0) return 1; // Unsubscribe notifications associated with the previously defined hook_id.

  return 0;
}

void (timer_int_handler)() {
  counter++; // Only thing we have to handle is to increase the counter!
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (timer > 2 || st == NULL) return 1; // Verifying input validity!

  uint8_t read_back_command = 0;
  read_back_command |= TIMER_RB_CMD; // Sets bits 7 and 6, because we are issuing a read-back command.
  read_back_command |= TIMER_RB_COUNT_; // We want to tead the statues (aka programmed mode), 
                                        // so bit 4 must be 0 and bit 5 must be 1 (we don't want to read the counter value!).
  read_back_command |= TIMER_RB_SEL(timer); // The macro is defined to set the n + 1 bit, so n corresponds to
                                            // the timer number (e.g., if timer = 2, we set bit 2 + 1 = 3).

  uint8_t read_status_word;  
  if (sys_outb(TIMER_CTRL, read_back_command) != 0) return 1; // Sends read-back command to the control register.                                      
  if (util_sys_inb(TIMER_0 + timer, &read_status_word) != 0) return 1; // The output comes out of the timer of interest; we have to select the respective port to get the value from
                                                                       // because the values are 0x40 (T0), 0x41 (T1) and 0x42 (T2), we can use 0x40 + the timer number to get the
                                                                       // respective port number.
  *st = read_status_word;
  // I opted to only attribute the read value to the st variable after validating the util_sys_inb status.
  // TODO: confirm if this is ok!
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  if (timer > 2 || field > 3) return 1; // Verifying input validity!

  union timer_status_field_val config; // This is the value that will be sent as argument of human-friendly time printer function timer_print_config.

  switch(field) {
    case tsf_all: // Display the full status byte in hexadecimal.
      config.byte = st; 
      break;
    case tsf_initial: // Only display the initialization mode.
      if (!(st & BIT(5)) && (st & BIT(4))) config.in_mode = LSB_only; // 01; LSB
      else if ((st & BIT(5)) && !(st & BIT(4))) config.in_mode = MSB_only; // 10; MSB
      else if ((st & BIT(5)) && (st & BIT(4))) config.in_mode = MSB_after_LSB; // 11; LSB followed by MSB
      else config.in_mode = INVAL_val; // 00; invalid
      break;
    case tsf_mode: { // Only display the counting mode.
      uint8_t mode = (st >> 1) & 7; // mode = bits 3, 2 and 1 (shift 1 to remove bit 1, and & 7 to remove bits 7, 6, 5 and 4)
      if (mode < 6) config.count_mode = mode;
      else if (mode == 6) config.count_mode = 2; // 110 is also 2.
      else config.count_mode = 3; // 111 (remaining possible number) is also 3.
      break;
    }
    case tsf_base: // Only display the counting base.
      config.bcd = st & 1; // Get the 0 bit.
      break;
    default:
      return 1;
  }

  if (timer_print_config(timer, field, config) != 0) return 1;
  return 0;
}
