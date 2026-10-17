#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

uint32_t timer_count;
int timerID = 0;

// @brief Changes the frequency of the timer selected
// @param timer timer to apply frequency change
// @param frequency is the new frequency that we want to set

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  
  int response;

  uint8_t msb, lsb;

  uint32_t v_cont = TIMER_FREQ/freq; // calculating the new timer_counting value based on the division of the value of frequency of the timer divided by the frequency we desire

  if(timer > 2 || freq <= 0 || freq > TIMER_FREQ || v_cont > 0xffff) // to avoid errors and overflows
    return 1;

  util_get_MSB(v_cont,&msb); // gets 8 most significant bits from #vcont
  util_get_LSB(v_cont,&lsb); // gets 8 least significant bits from #vcont

  uint8_t conf, byte;
  response = timer_get_conf(timer, &conf);  // gets the current timer configurations and puts them in response
  byte = conf & 0x0f;  //preserves the lest significant 4 bits and zero's the 1st 4


  // writes to 0x43 (control) to let me write the lsb and then msb on the selected timer_counter
  switch(timer)
  {
    case 0:
      response = response || sys_outb(TIMER_CTRL, byte | TIMER_SEL0 | TIMER_LSB_MSB);
    break;
    case 1:
      response = response || sys_outb(TIMER_CTRL, byte | TIMER_SEL1 | TIMER_LSB_MSB);
    break;
    case 2:
      response = response || sys_outb(TIMER_CTRL, byte | TIMER_SEL2 | TIMER_LSB_MSB);
    break;
  }

  //writes the configurations on the timer
  response = response || sys_outb(TIMER_0+timer, lsb);
  response = response || sys_outb(TIMER_0+timer, msb);

  return response;
}

// @brief puts the timer selected available to send interrupts and receive notifications
// @param bit_no memory part where the said notifications will go to upon an interrupt
int (timer_subscribe_int)(uint8_t *bit_no) {
  if(bit_no == NULL) // no memory was allocated for bit_no
    return 1;

  *bit_no = BIT(timerID); // intializes bit_no with the number of bits to be set by the mask upon an interrupt
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timerID);
}

//@brief unsubscribes timer interrupts from the irqline
int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&timerID);
}

//Increments the timer_counter timer_count
void (timer_int_handler)() {
  timer_count++;
}

//@brief reads status for the selected timer and stores it in st
// @param timer selected timer to obtain status
// @param *st memory space where the status configurations will be read to
int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  //Returns error code if the timer selected is not in the range[0,2], since uint8_t is unsigned there's no need to check if its lower then zero
  //Also returns error code if the sys_outb() function to set the timer to read the control word is not successfull
  if(timer > 2 || sys_outb(TIMER_CTRL, TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer) ))
    return 1;
  //Get the status for the desired timer and returns if it was successfull or not
  return util_sys_inb(TIMER_0+timer,st);
}

//@brief displays the configurations of a timer using an enumerator and a union
// @param timer timer we want to read status from
// @param st where the status is stored
// @param field shows the alphabet possible for what a certain bit or numer of bits represents of the configuration
int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  
  if(timer > 2)
    return 1;

  union timer_status_field_val conf;

  // selects which field of the enum variable we want to read and puts it into the union conf
  switch(field)
  {
      case tsf_all:
        conf.byte = st;
      break;
      
      case tsf_initial:
        conf.in_mode = (st & (BIT(4) | BIT(5))) >> 4;
      break;
      
      case tsf_mode:
        conf.count_mode = (st & (BIT(1) | BIT(2) | BIT(3))) >> 1;
        if (conf.count_mode > 5)
        {
          conf.count_mode = conf.count_mode & (BIT(0) | BIT(1));
        }
      break;

      case tsf_base:
        conf.bcd = st & BIT(0);
      break;

      default:
        //invalid field
        return 1;
  }

  return timer_print_config(timer,field, conf);;
}
