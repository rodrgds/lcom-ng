#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>
#include <stdio.h>

#include "i8254.h"

uint32_t auxCounter = 0;  // initialized global variable used in timer_int_handler
int hook_id = TIMER0_IRQ;  // initialized global variable used in time_subscribe_int

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if(timer<0 || timer>2 || freq>TIMER_FREQ || freq <=18) return 1; // values were wrong for freqs below 19
  uint32_t seltimer;
  uint8_t CWord, CWordAux, lsb, msb;
  if(timer_get_conf(timer, &CWordAux)) return 1;
  CWord = (CWordAux & 0x0F) | TIMER_LSB_MSB; //builds the Control Word apart from the timer
  switch (timer){ // adds the timer and saves the timer port
      case 0: { // when the timer is 0
          CWord |= TIMER_SEL0;
          seltimer = TIMER_0;
          break;
      }
      case 1: { // when the timer is 1
          CWord |= TIMER_SEL1;
          seltimer = TIMER_1;
          break;
      }
      case 2: { // when the timer is 2
          CWord |= TIMER_SEL2;
          seltimer = TIMER_2;
          break;
      }
      default: // error handler
          return 1;
  }
  if(sys_outb(TIMER_CTRL, CWord)) return 1; // changes control register to the newly built control word
  uint16_t div = (uint16_t)(TIMER_FREQ / freq); // calculates the freq
  if(util_get_LSB(div, &lsb)) return 1;
  if(util_get_MSB(div, &msb)) return 1;
  if(sys_outb(seltimer,(uint32_t)lsb)) return 1;
  if(sys_outb(seltimer,(uint32_t)msb)) return 1;

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    if (bit_no == NULL) { //checks if the pointer is valid
        printf("Pointer bit_no is invalid.\n");
        return 1;
    }

    *bit_no = (uint8_t) hook_id;
    if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id)) {
        printf("Error subscribing a notification in timer interrupts.\n");
        return 1;
    }
    return 0;
}

int (timer_unsubscribe_int)() {
    if(sys_irqrmpolicy(&hook_id)) {
        printf("Failed unsubscribing the previous timer interrupt subscription.\n");
        return 1;
    }
    return 0;
}

void (timer_int_handler)() {
    auxCounter++;
}

/*int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  uint32_t rb_cmd = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer); // builds the readback command (apart from the count)
  uint8_t staux;
  if (sys_outb(TIMER_CTRL, rb_cmd)) return 1; // writes to the control register the readback command
  switch(timer) {
      case 0: {
          if(util_sys_inb(TIMER_0, &staux)) return 1;
          *st = staux;
          return 0;
      }
      case 1: {
          if(util_sys_inb(TIMER_1, &staux)) return 1;
          *st = staux;
          return 0;
      }
      case 2: {
          if(util_sys_inb(TIMER_2, &staux)) return 1;
          *st = staux;
          return 0;
      }
      default:
          return 1;
  }
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
      if(timer<0||timer>2) return 1;
      union timer_status_field_val aux;
      switch(field) {
          case tsf_all: {
              aux.byte = st;
              break;
          }
          case tsf_initial: {
              st = (st & TIMER_LSB_MSB)>>4; // selects bits 4 and 5 and shifts them to the LSBs
              aux.in_mode = st;
              break;
          }
          case tsf_mode: {
              st = (st & 0x0E)>>1; // selects bits 1, 2 and 3 and shifts them to the LSBs
              if(st==6) aux.count_mode = 2;
              else if(st==7) aux.count_mode = 3;
              else aux.count_mode = st;
              break;
          }
          case tsf_base: {
              if(st & 1) aux.bcd = true; // selects bit 1
              else aux.bcd = false;
              break;
          }
          default:
              return 1;
      }
      if(timer_print_config(timer, field, aux)) return 1;
      return 0;
}
*/
