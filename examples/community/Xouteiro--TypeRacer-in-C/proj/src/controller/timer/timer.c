#include <lcom/lcf.h>
#include <stdint.h>
#include "timer.h"

int hook_id = 0;
int counter = 0;

/**
 * @brief Set the timer's frequency
 * @param timer timer to configure
 * @param freq frequency to be set
 * @return 0 if successful, 1 otherwise
*/
int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (freq > TIMER_FREQ || freq < 19) return 1;

  uint8_t controlWord;
  if (timer_get_conf(timer, &controlWord)) return 1;

  controlWord = (controlWord & 0x0F) | TIMER_LSB_MSB; 

  uint32_t initialValue = TIMER_FREQ / freq;
  uint8_t MSB, LSB;
  util_get_MSB(initialValue, &MSB);
  util_get_LSB(initialValue, &LSB);

  uint8_t selectedTimer;      
  switch (timer) {  
    case 0: 
      controlWord |= TIMER_SEL0;
      selectedTimer = TIMER_0;
      break;
    case 1:
      controlWord |= TIMER_SEL1;
      selectedTimer = TIMER_1;
      break;
    case 2:
      controlWord |= TIMER_SEL2;
      selectedTimer = TIMER_2;
      break;
    default:
      return 1;
  }

  if (sys_outb(TIMER_CTRL, controlWord)) return 1;

  if (sys_outb(selectedTimer, LSB)) return 1;
  if (sys_outb(selectedTimer, MSB)) return 1;
  return 0;
}

/**
 * @brief Subscribes and enables timer interrupts
 * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @return 0 if successful, 1 otherwise
*/
int (timer_interrupts_subscription)(uint8_t *bit_no) {
  if(bit_no == NULL) return 1;
  (*bit_no) = BIT(hook_id);
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id);
}

/**
 * @brief Unsubscribes timer interrupts
 * @return 0 if successful, 1 otherwise
*/
int (timer_interrupts_unsubscription)() {
  return sys_irqrmpolicy(&hook_id);
}

/**
 * @brief Reads the timer configuration
 * @param timer timer whose configuration is to be read
 * @param st address of memory to be initialized with the timer config read
 * @return 0 if successful, 1 otherwise
*/
int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (st == NULL || timer > 2 || timer < 0) return 1;
  uint8_t RBC = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer));
  if (sys_outb(TIMER_CTRL, RBC)) return 1;
  if (util_sys_inb(TIMER_0 + timer, st)) return 1;
  return 0;
}
