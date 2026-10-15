/**
 * @file timer.c
 * @brief Timer driver implementation for i8254 timer.
 */

#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>

#include "i8254.h"

#define TIMER_MODE_MASK 0x0E

static int hook_id = TIMER0_IRQ;       /**< IRQ line for timer 0 */
uint32_t timer_ticks = 0;               /**< Counts timer interrupts */

/**
 * @brief Sets the frequency of a specified timer.
 * 
 * @param timer Timer number (0, 1, or 2).
 * @param freq Desired frequency in Hz.
 * @return 0 on success, 1 on failure (invalid params or syscalls fail).
 */
int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (timer > 2 || freq < 19 || freq > TIMER_FREQ) {
      return 1;  
  }

  uint8_t status;
  if (timer_get_conf(timer, &status) != 0) {
      return 1;
  }

  uint16_t initial_value = TIMER_FREQ / freq;
  uint8_t lsb, msb;

  if (util_get_LSB(initial_value, &lsb) != 0 || util_get_MSB(initial_value, &msb) != 0) {
      return 1;
  }

  uint8_t control_word = TIMER_LSB_MSB | (status & 0x0F);

  switch (timer) {
    case 0: control_word |= TIMER_SEL0; break;
    case 1: control_word |= TIMER_SEL1; break;
    case 2: control_word |= TIMER_SEL2; break;
    default: return 1;
  }

  if (sys_outb(TIMER_CTRL, control_word) != OK) {
      return 1;
  }

  if (sys_outb(TIMER_0 + timer, lsb) != OK || sys_outb(TIMER_0 + timer, msb) != OK) {
      return 1;
  }

  return 0;
}

/**
 * @brief Subscribes to timer interrupts.
 * 
 * @param bit_no Pointer to store the interrupt bitmask.
 * @return 0 on success, 1 on failure.
 */
int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;

  *bit_no = hook_id;

  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != OK) return 1;
  
  return 0;
}

/**
 * @brief Unsubscribes from timer interrupts.
 * 
 * @return 0 on success, 1 on failure.
 */
int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id) != OK) return 1;

  return 0;
}

/**
 * @brief Timer interrupt handler: increments tick count.
 */
void (timer_int_handler)() {
  timer_ticks++;
}

/**
 * @brief Reads the current configuration of the specified timer.
 * 
 * @param timer Timer number (0, 1, or 2).
 * @param st Pointer to store the timer status.
 * @return 0 on success, 1 on failure.
 */
int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (timer > 2 || st == NULL) {
    return 1;
  }

  uint8_t read_back_cmd = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

  if (sys_outb(TIMER_CTRL, read_back_cmd) != OK) {
      return 1;
  }

  if (util_sys_inb(TIMER_0 + timer, st) != 0) {
    return 1;
  }

  return 0;
}

/**
 * @brief Displays the configuration of the specified timer.
 * 
 * @param timer Timer number (0, 1, or 2).
 * @param st Status byte to interpret.
 * @param field Field of the timer configuration to display.
 * @return 0 on success, 1 on failure.
 */
int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  
  if (timer > 2) {
        return 1;
    }

  union timer_status_field_val value;

  switch (field) {
    case tsf_all:
        value.byte = st; 
        break;

    case tsf_initial:
        value.in_mode = (st & TIMER_LSB_MSB) >> 4;
        break;

    case tsf_mode:
        value.count_mode = (st & TIMER_MODE_MASK) >> 1;
        if (value.count_mode == 6) value.count_mode = 2; 
        if (value.count_mode == 7) value.count_mode = 3;
        break;

    case tsf_base:
        value.bcd = (st & TIMER_BCD) ? 1 : 0;
        break;

    default:
        return 1;
  }

  if (timer_print_config(timer, field, value) != 0) {
      return 1; 
  }

  return 0;
}

