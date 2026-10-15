/**
 * @file timer.c
 * @brief Timer driver implementation for configuring and handling the i8254 timer.
 *
 * This file provides functions for setting the timer frequency, subscribing/unsubscribing timer interrupts,
 * handling timer interrupts, reading timer configuration, and displaying timer configuration fields.
 */

#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>
#include "i8254.h"

#undef timer_set_frequency

int counter = 0;       
int timer_hook_id = 0;   

/**
 * @brief Sets the frequency of the specified timer.
 *
 * Configures the timer to generate interrupts at the specified frequency.
 *
 * @param timer Timer number (0, 1, or 2).
 * @param freq Desired frequency in Hz.
 * @return 0 on success, 1 on failure.
 */
int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (timer > 2 || freq < 19) {  // 1193181 / 2^16 ≈ 18.2 Hz minimum
    printf("Invalid timer or frequency\n");
    return 1;
  }

  uint32_t base_freq = TIMER_FREQ; 
  uint16_t divisor = (uint16_t)(base_freq / freq);

  uint8_t st;
  if (timer_get_conf(timer, &st) != 0) {
    printf("Failed to get timer config\n");
    return 1;
  }

  uint8_t control = (st & 0x0F);
  control |= TIMER_LSB_MSB;

  switch (timer) {
    case 0: control |= TIMER_SEL0; break;
    case 1: control |= TIMER_SEL1; break;
    case 2: control |= TIMER_SEL2; break;
  }

  if (sys_outb(TIMER_CTRL, control)) {
    printf("Failed to write control word\n");
    return 1;
  }

  uint8_t lsb, msb;
  if (util_get_LSB(divisor, &lsb) || util_get_MSB(divisor, &msb)) {
    printf("Failed to get LSB or MSB\n");
    return 1;
  }

  uint8_t timer_port = TIMER_0 + timer;

  if (sys_outb(timer_port, lsb) || sys_outb(timer_port, msb)) {
    printf("Failed to write divisor to timer\n");
    return 1;
  }

  return 0;
}

/**
 * @brief Subscribes timer interrupts.
 *
 * Sets the IRQ policy for timer interrupts.
 *
 * @param bit_no Pointer to variable where the hook id bit number will be stored.
 * @return 0 on success, non-zero otherwise.
 */
int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = timer_hook_id;
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id);
}

/**
 * @brief Unsubscribes timer interrupts.
 *
 * Removes the IRQ policy for timer interrupts.
 *
 * @return 0 on success, non-zero otherwise.
 */
int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&timer_hook_id);
}

/**
 * @brief Timer interrupt handler.
 *
 * Increments the global counter on each timer interrupt.
 */
void (timer_int_handler)() {
  counter++;
}

/**
 * @brief Reads the configuration of the specified timer.
 *
 * Uses the read-back command to retrieve the status byte of the timer.
 *
 * @param timer Timer number (0, 1, or 2).
 * @param st Pointer to variable where the status byte will be stored.
 * @return 0 on success, 1 on failure.
 */
int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (timer > 2) return 1; 

    uint8_t read_back_cmd = 0xE0 | (1 << (timer + 1));  

    if (sys_outb(TIMER_CTRL, read_back_cmd) != OK) {
        return 1;
    }

    uint16_t timer_port = TIMER_0 + timer;

    if (util_sys_inb(timer_port, st) != 0) {
        return 1;
    }

    return 0;
}

/**
 * @brief Displays the specified field of the timer configuration.
 *
 * Extracts and prints the requested field from the timer status byte.
 *
 * @param timer Timer number (0, 1, or 2).
 * @param st Status byte of the timer.
 * @param field Field to extract (all, initial, mode, or base).
 * @return 0 on success, 1 on failure.
 */
int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  if (timer > 2) return 1;  

    union timer_status_field_val val; 

    switch (field) {
        case tsf_all:
            val.byte = st;  
            break;

        case tsf_initial:
            val.in_mode = (st & 0x30) >> 4;  
            break;

        case tsf_mode:
            val.count_mode = (st >> 1) & 0x07;  

            if (val.count_mode == 6) val.count_mode = 2;
            if (val.count_mode == 7) val.count_mode = 3;
            break;

        case tsf_base:
            val.bcd = st & 0x01;  
            break;

        default:
            return 1; 
    }

    return timer_print_config(timer, field, val); 
}
