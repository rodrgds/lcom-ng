#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "defines.h"
#include "auxfuncs.h"

/** @defgroup Timer
 * @{
 *
 * Functions related to the use of the timer in the project
 */

int timerIRQ = TIMER0_IRQ;
int timerCounter = 0;

/**
 * @brief Sets the frequency of a timer.
 *
 * @param timer The timer to configure (0, 1, or 2).
 * @param freq The desired frequency.
 * @return 0 upon success, non-zero otherwise.
 */
int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

  if (!((timer==0)||(timer==1)||(timer==2)) || freq>TIMER_FREQ || freq<19) return 1;

  uint16_t counter = TIMER_FREQ/freq;

  uint8_t lsb;
  uint8_t msb;
  aux_get_LSB(counter,&lsb);
  aux_get_MSB(counter,&msb);

  uint8_t conf;
  timer_get_conf(timer,&conf);
  conf &= 0x0F;
  conf |= BIT(5) | BIT(4);

  sys_outb(TIMER_CTRL, conf);
  sys_outb(TIMER_0 + timer, lsb);
  sys_outb(TIMER_0 + timer, msb);

  return 0;
}

/**
 * @brief Subscribes and enables timer interrupts.
 *
 * @param bit_no Address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt.
 * @return 0 upon success, non-zero otherwise.
 */
int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = timerIRQ;
  sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timerIRQ);
  return 0;
}

/**
 * @brief Unsubscribes timer interrupts.
 *
 * @return 0 upon success, non-zero otherwise.
 */
int (timer_unsubscribe_int)() {
  sys_irqrmpolicy(&timerIRQ);
  return 0;
}

/**
 * @brief Timer interrupt handler.
 */
void (timer_int_handler)() {
  timerCounter++;
}

/**
 * @brief Reads the configuration of a timer.
 *
 * @param timer The timer to get the configuration from (0, 1, or 2).
 * @param st Address of memory to be initialized with the timer configuration.
 * @return 0 upon success, non-zero otherwise.
 */
int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  uint8_t bt = TIMER_RB_CMD | BIT(5) | BIT(timer + 1);
  
  sys_outb(TIMER_CTRL, bt);
  aux_sys_inb(TIMER_0 + timer, st);
  return 0;
}

/**
 * @brief Displays the configuration of a timer.
 *
 * @param timer The timer to display the configuration from (0, 1, or 2).
 * @param st The timer configuration.
 * @param field The field to display.
 * @return 0 upon success, non-zero otherwise.
 */
int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  union timer_status_field_val output;

  switch (field) {
    case tsf_base: {
      output.bcd = st & 1;
      break;
    }
    case tsf_mode: {
      if (((st >> 1) & 7) == 6) output.count_mode = 2;
      else if (((st >> 1) & 7) == 7) output.count_mode = 3;
      else output.count_mode = (st >> 1) & 7;
      break;
    }
    case tsf_initial: {
      if ((st & TIMER_LSB) && (st & TIMER_MSB)) output.in_mode = MSB_after_LSB;
      else if (st & TIMER_LSB) output.in_mode = LSB_only;
      else if (st & TIMER_MSB) output.in_mode = MSB_only;
      else output.in_mode = INVAL_val;
      break;
    }
    case tsf_all: {
      output.byte = st;
      break;
    }
  }
  timer_print_config(timer, field, output);

  return 0;
}

