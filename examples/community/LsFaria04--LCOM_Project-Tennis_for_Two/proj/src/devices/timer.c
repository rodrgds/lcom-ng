#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

static int hook_id = 0;
int counter = 0;

int (get_hook_id)(){
    return hook_id;
}

int (get_counter)(){
  return counter;
}

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

  if(TIMER_FREQ/freq > BIT(16) - 1){
    printf("Frequency is not allowed\n");
    return EXIT_FAILURE;
  }

  //Get Timer configuration
  uint8_t conf = 0;
  if(timer_get_conf(timer, &conf) != 0){
    return EXIT_FAILURE;
  }

  conf &= 0x0F; //resets 4 first bits

  //Insert the timer we want to configure
  switch(timer){
    case 0:
      conf |= TIMER_SEL0;
      break;
    case 1:
      conf |= TIMER_SEL1;
      break;
    case 2:
      conf |= TIMER_SEL2;
      break;
  }

  //Insert Initialization Mode
  conf |= TIMER_LSB_MSB;


  //write command on the control register
  if(sys_outb(TIMER_CTRL, conf) != 0){
    return EXIT_FAILURE;
  }

  //calculates the div value and separates the lsb and msb
  uint16_t div = TIMER_FREQ/freq;
  uint8_t lsb = 0;
  uint8_t msb = 0;
  if(util_get_LSB(div,&lsb) != 0){
    return EXIT_FAILURE;
  }
  if(util_get_MSB(div,&msb) != 0){
    return EXIT_FAILURE;
  }

  //inserts the lsb and msb in the timer
  if(sys_outb(TIMER_0 + timer, lsb) != 0){
    return EXIT_FAILURE;
  }
  if(sys_outb(TIMER_0 + timer, msb) != 0){
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
  
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  //calls the sys_irqsetpolicy (initializes the interrupts)
  *bit_no = BIT(0);
  hook_id = *bit_no;
  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != 0)
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

int (timer_unsubscribe_int)() {
  /*Call the sys_irqrmpolicy(int *hook_id) where hook_id is the same as the bit_no*/
  if(sys_irqrmpolicy(&hook_id) != 0){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

void (timer_int_handler)() {
  //increments the global counter
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  uint32_t rb_command = 0;

  //Insert read_back Command
  rb_command |= TIMER_RB_CMD; // 11000000b

  //Insert count and status (their active low!!!)
  rb_command |= TIMER_RB_COUNT_; //counter to 1


  //Insert Timer Value
  rb_command |= TIMER_RB_SEL(timer);

  //Write command on the control register
  if(sys_outb(TIMER_CTRL , rb_command) != 0){
    return EXIT_FAILURE;
  }
  //reads the value on the timer
  if(util_sys_inb(TIMER_0 + timer,st) != 0){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;

}
