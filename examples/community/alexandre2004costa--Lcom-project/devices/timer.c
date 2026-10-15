#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>
#include "i8254.h"

/** @defgroup timer timer
 * @{
 */

int hookId = 0;
int count = 0;



/**
 * @brief Changes the frequency of a timer.
 * @param timer Timer that will have the frequency changed.
 * @param freq New wanted frequency.
 * @return 0 in case of success.
 */
int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (timer < 0 || timer > 2 || freq < 19 || freq > TIMER_FREQ) return 1;
  uint8_t oldConfig;
  if (timer_get_conf(timer, &oldConfig) != 0) return 1;
  uint8_t newConfig = (oldConfig & 0x0f) | TIMER_LSB_MSB;
  if (timer == 1) newConfig |= TIMER_SEL1;
  else if (timer == 2) newConfig |= TIMER_SEL2;
  if (sys_outb(TIMER_CTRL, newConfig) != 0) return 1;
  uint16_t f = TIMER_FREQ / freq;
  uint8_t lsb, msb;
  lsb = f & 0xff;
  msb = f >> 8;
  if (sys_outb(TIMER_0 + timer, lsb) != 0) return 1;
  return sys_outb(TIMER_0 + timer, msb);
}

/**
 * @brief Activates a timer interrupt.
 * @param bit_no
 * @return 0 in case of success.
 */
int (timer_subscribe_int)(uint8_t *bit_no) {
  if( bit_no == NULL) return 1; 
  *bit_no = BIT(hookId);       
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hookId) != 0) return 1;
  return 0;
}

/**
 * @brief Deactivates a timer interrupt.
 */
int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hookId) != 0) return 1;
  return 0;
}

/**
 * @brief Increments the timer counter.
 */
void (timer_int_handler)() {
  count++;
}

/**
 * @brief Gets the current timer configuration.
 * @param timer Desired timer.
 * @param st Reference that will store the configuration.
 * @return 0 in case of success.
 */
int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (timer < 0 || timer > 2 || st == NULL) return 1;
  uint8_t rb = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  if (sys_outb(TIMER_CTRL,rb) != 0) return 1;
  if (util_sys_inb(TIMER_0 + timer, st) != 0) return 1;
  return 0;
}

/**
 * @brief Displays the current timer configuration.
 * @param timer Desired timer.
 * @param st Reference that will store the configuration.
 * @param field Auxiliary variable to show the configuration.
 * @return 0 in case of success.
 */
int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  if (timer < 0 || timer > 2) return 1;
  union timer_status_field_val myunion;
  switch (field)
  {
  case tsf_all:
    myunion.byte = st;
    break;
  case tsf_initial:
    st = (st >> 4) & 0x03;
    if (st == 1) myunion.in_mode = LSB_only;
    else if (st == 2) myunion.in_mode = MSB_only;
    else  myunion.in_mode = MSB_after_LSB;
    break;
  case tsf_mode:
    st = (st >> 1) & 0x07;
    if (st == 6) myunion.count_mode = 2;
    else if (st == 7) myunion.count_mode = 3;
    else myunion.count_mode = st;
    break;
  case tsf_base:
    st = st & TIMER_BCD;
    myunion.bcd = st;
    break;
  default:
    break;
  }
  if (timer_print_config(timer,field,myunion) != 0) return 1;
  return 0;
}

/**@}*/
