#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int counter = 0;
// set when you subscribe to an irq line, need to unsubscribe, needs to be global
int hook_id = 0;

/*Set the frequency of the timer
(timer -> 8 bit number that contains what timer we're working with)
(freq -> 32 bit number of the frequency that we want to set)
*/
int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  //if frequency is less than max timer frequency divided by the max value for an unsigned short or more than the max timer frequency or timer is less than 0 or timer is more than 2 return 1 for failure
  if(freq < TIMER_FREQ/USHRT_MAX || freq > TIMER_FREQ  || timer < 0 || timer > 2) {
    return 1;
  }

  // 8 bit number to store status
  uint8_t st;

  // get configuration of the timer
  if(timer_get_conf(timer, &st) == 1) {
    return 1;
  }

  //select bits regarding timer selection for control word
  uint8_t timer_cw = TIMER_SEL0;

  if (timer == 1) {
    timer_cw = TIMER_SEL1;
  }

  else if(timer == 2) {
    timer_cw = TIMER_SEL2;
  }

  //getting the bits in the status  byte that correspond to the BCD and the operating mode
  
  st = st << 4;

  st = st >> 4;
  
  //the control word consists on the timer selection for the control word, the initialization mode, the operationg mode and the BCD
  uint8_t control_word  = timer_cw | TIMER_LSB_MSB | st;

  //the current frequency is the max timer frequency divided by the frequency given in the parameters
  uint16_t curr_freq = TIMER_FREQ/freq;

  // write the control word to the timer controller
  if(sys_outb(TIMER_CTRL, control_word) != 0) {
    return 1;
  }

  //create variable for least significant bit
  uint8_t lsb = 0x00;

  //get least significant byte for current frequency
  util_get_LSB(curr_freq, &lsb);

  //create variable for most significant bit
  uint8_t msb = 0x00;

  //get most significant byte for current frequency
  util_get_MSB(curr_freq, &msb);

  //select adress of the correct timer (of the register)
  uint8_t timer_register = TIMER_0;

  switch (timer)
  {
  case 1:
  timer_register = TIMER_1;
    break;
  case 2:
  timer_register = TIMER_2;
    break;
  default:
    break;
  }

  //write to timer register the least significant byte
  if(sys_outb(timer_register, (uint32_t )lsb) != 0) {
    return 1;
  }

  //write to timer register the most significant byte
  if(sys_outb(timer_register, (uint32_t )msb) != 0) {
    return 1;
  }

  // return 0 if success
  return 0;
}

/*subscribe irq line
(bit_no -> bit where irq is set)*/
int (timer_subscribe_int)(uint8_t *bit_no) {
   //if bit_no is NUll then return 1 for failure
  if(bit_no == NULL) {
    return 1;
  }

  // set policy for IRQ
  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) == 1) {
    return 1;
  }
  
  //return 0 if success
  return 0;
}

/*unsubscribe irq line*/
int (timer_unsubscribe_int)() {

  //remove policy for IRQ
  if(sys_irqrmpolicy(&hook_id) == 1) {
    return 1;
  }
  
  // return 0 if success
  return 0;
}

/*increment counteer*/
void (timer_int_handler)() {
  counter++;
}

/*Get configuration of the timer 
(timer -> 8 bit number that contains what timer we're working with)
(st -> 8 bit number that will contain the status of the timer)*/
int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  //if the status number is null or the timer number is less than 0 or bigger than 2, the function returns 1 on error
  if(st == NULL || timer < 0 || timer > 2) {
    return 1;
  }

  // to build the read back command for the status, we must activate the read-back command (bits 6 and 7), the read counter values (bit 5) and select the timer (bit 1, 2 or 3)
  uint8_t read_back_command = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer));

  // writes to a register (TIMER_CTRL) the read back command
  if(sys_outb(TIMER_CTRL, read_back_command) != 0) {
    return 1;
  }

  // reads the status from the register of whatever timer is selected
  if(util_sys_inb(TIMER_0 + timer, st) == 1){
    return 1;
  }

  //returns 0 if success
  return 0;

}

/*Display of the timer configuration 
(timer -> 8 bit number that contains what timer we're working with)
(st -> 8 bit number that contains the status of the timer)
(field -> it's a selector for what information of the status we want displayed)*/
int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  //structure to store a field value
  union timer_status_field_val val;
  
  //switch field to select what information of the status we want displayed and putting stuff inside the union
  switch(field) {
    case tsf_all:
    val.byte = st;
    break;
    case tsf_base:
    val.bcd = (st & 0x01);
    break;
    case tsf_initial:
    val.in_mode = (st & 0x30) >> 4;
    break;
    case tsf_mode:
    val.count_mode = (st & 0x0E) >> 1;

    break;
    default:
    break;
  }

  //print the configuration of the timer
  if(timer_print_config(timer, field, val) == 1) {
    return 1;
  }

  //returns 0 if success
  return 0;

}
