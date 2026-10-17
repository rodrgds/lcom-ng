#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>


#include "timer.h"

//Timer to be incremented by the timer interrupts
int timer_int_count = 0;

//Hook id to be used to set the interrupt policy
int timer0_hook_id = 0;




int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
 
  //We test if the frequency is less than 19 or higher than the timer max frequency and if so it is not possible
  if(freq < 19 || freq > TIMER_FREQ)
    return 1;
  
  //First we retrieve the status by calling timer_get_conf
  uint8_t status;
  if (timer_get_conf(timer, &status)) 
    return 1;

  //Then we save the LSB bytes as those we want to maintain when assembling the control word
  status = (status & 0x0f);

  //We calculate the frequency needed to send to the timer register
  uint16_t count = (uint16_t)(TIMER_FREQ / freq);

  //Initialization of the control word
  uint8_t controlw = 0;

  //Assembly of the control word based on the timer requested (If the timer is not valid return 1)
  if (timer == 0)
    controlw = ((TIMER_SEL0) | (TIMER_LSB_MSB) | (status));
  else if (timer == 1)
    controlw = ((TIMER_SEL1) | (TIMER_LSB_MSB) | (status));
  else if (timer == 2)
    controlw = ((TIMER_SEL2) | (TIMER_LSB_MSB) | (status));
  else
    return 1;

  //Now we send the control word to the control register
  if (sys_outb(TIMER_CTRL, controlw))
    return 1;
  
  //We split the 16-bit number neeed for the timer register because we specified LSB then MSB
  uint8_t lsb = 0, msb = 0;
  util_get_LSB(count, &lsb);
  util_get_MSB(count, &msb);


  //We send the count for timer register based on timer requested
  if (timer == 0) 
  {
    if (sys_outb(TIMER_0, lsb))
      return 1;
    if (sys_outb(TIMER_0, msb))
      return 1;
  }
  else if (timer == 1)
  {
    if (sys_outb(TIMER_1, lsb))
      return 1;
    if (sys_outb(TIMER_1, msb))
      return 1;
  }
  else if (timer == 2)
  {
    if (sys_outb(TIMER_2, lsb))
      return 1;
    if (sys_outb(TIMER_2, msb))
      return 1;
  }


  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  
  //Checking the validity of the bit_no argument
  if (bit_no == NULL)
    return 1;


  //Auxiliaty variable used to pass and get the return of sys_irqsetpolicy
  int aux = (int)*bit_no;
 
  //used to subscribe the interruption
  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE,&aux))
    return 1;


  //Used for the return
  *bit_no = (uint8_t)aux;


  return 0;
}

int (timer_unsubscribe_int)() {
 
  //Unsubscribing the interruptions
  if(sys_irqrmpolicy(&timer0_hook_id))
    return 1;

  return 0;
}

void (timer_int_handler)() {

  timer_int_count++;

}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  
  /*Assembly of the readback command
    -Because we want the count we have to leave a bit to 0 so we don't use the macro
    -Last bit is reserved to 0
  */

  //Check if the passed timer argument is valid
  if (timer < 0 || timer > 2)
    return 1;

  //Checking the validity of the pointer passed as argument
  if (st == NULL)
    return 1;

  uint8_t readBack = (TIMER_RB_CMD) |       
                     (TIMER_RB_COUNT_) |
                     (TIMER_RB_SEL(timer));

  //printf("%x",readBack);  //Used to test the assembly of the readback command

  //Sending the readback command to 0x43
  if(sys_outb(TIMER_CTRL, readBack))
    return 1;

  //Retrieving the timer status requested with the readback command
  if(util_sys_inb(TIMER_TO_PORT(timer),st))
    return 1;

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {

  //Display all of the status returned(tsf_all)
  if (timer != 0 && timer != 1 && timer != 2)
    return 1;

  if(field == tsf_all){
    
    //We want the entire status so we assign it to the byte in the union
    union timer_status_field_val val;
    val.byte = st;

    //Calling the function
    if (timer_print_config(timer,field,val))
      return 1;
  }
  //Display only the initialization mode(tsf_initial)
  else if(field == tsf_initial){

    //We only want the intialization mode so we only want bits number 4 and 5
    uint8_t init_mode = ((STATUS_INIT_MODE & st) >> 4); 

    union timer_status_field_val val;
    //LSB only mode
    if(init_mode == 1)
      val.in_mode = LSB_only;
    //MSB only mode
    else if(init_mode == 2)
      val.in_mode = MSB_only;
    //LSB first then MSB
    else if(init_mode == 3)
      val.in_mode = MSB_after_LSB;
    //Other option will be invalid
    else
      val.in_mode = INVAL_val;


    if(timer_print_config(timer,field,val))
      return 1;

  }
  //Display only the counting mode(tsf_mode)
  else if(field == tsf_mode){
    //WE want only bit 1 and 2
    uint8_t count_mode = ((STATUS_COUNT_MODE & st) >> 1);

    union timer_status_field_val val;
    val.count_mode = count_mode;

    if(timer_print_config(timer,field,val))
      return 1;

  }
  //Display only the counting base(tsf_base)
  else if(field == tsf_base){
    //We want only bit 0
    uint8_t count_base = (STATUS_COUNTING_BASE & st);

    union timer_status_field_val val;
    //Its true if BCD is the counting base desired (bit 0 is 1)
    if(count_base == 0)
      val.bcd = false;
    else if(count_base == 1)
      val.bcd = true;

    if(timer_print_config(timer,field,val))
      return 1;
  }else{
    return 1;
  }

  return 0;
}
