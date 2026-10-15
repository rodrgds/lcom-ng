#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>
#include "i8254.h"

static int hook_id = 0;
unsigned int counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

  if (freq > TIMER_FREQ) {
    printf("Invalid frequency\n");
    return 1;
  }
  
  uint8_t current_config;
  if (timer_get_conf(timer, &current_config) != 0) {
    printf("Error reading timer_get_conf\n");
    return 1;
  }

  uint16_t initial_count = TIMER_FREQ / freq;

  uint8_t msb, lsb;
  if (util_get_MSB(initial_count, &msb) != 0 || util_get_LSB(initial_count, &lsb) != 0) {
    printf("Could not obtain initial count MSB and/or LSB\n");
    return 1;
  }

  uint8_t control_word = (timer << 6) | TIMER_LSB_MSB | (current_config & 0x0F);
  
  if(sys_outb(TIMER_CTRL, control_word)) {
    printf("Error in sys_outb while writing control word\n");
    return 1;   
  } 
  
  if(sys_outb(TIMER_0 + timer, lsb) != 0) {
    printf("Error in sys_outb while writing LSB\n");
    return 1;
  }

  if(sys_outb(TIMER_0 + timer, msb) != 0) {
    printf("Error in sys_outb while writing MSB\n");
    return 1;
  }
  
  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {

  hook_id = 0;

  if(bit_no == NULL) 
    return 1;

  *bit_no = hook_id;

  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != 0) 
  {
    printf("Error subscribing to interrupts\n");
    return 1;
  }

  return 0;
}

int (timer_unsubscribe_int)() {

  if (sys_irqrmpolicy(&hook_id) != 0) 
  {
    printf("Error unsubscribing from interrupts\n");
    return 1;
  }

  return 0;
}

void (timer_ih)() {
  counter++;
}


int (timer_get_conf)(uint8_t timer, uint8_t *status) {

    if (status == NULL) {
        printf("Status pointer is invalid.\n");
        return 1;
    }

    if (timer < 0 || timer > 2) {
        printf("Invalid timer.\n");
        return 1;
    }
    
    uint8_t RBCommand = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

    if (sys_outb(TIMER_CTRL, RBCommand)) {
        printf("Error: Failed to send read-back command.\n");
        return 1;
    }

    uint8_t timer_port = TIMER_0 + timer;

    if (util_sys_inb(timer_port, status)) {
        printf("Error: Failed to read configuration from timer's port.\n");
        return 1;
    }

    return 0;  
}
