#include "timer.h"


int timer_hook_id = 0; 
int timer_counter = 0; //used to count the number of interrupts --- see lab2.c



int (timer_subscribe_int)(uint8_t *bit_no) {
  if(bit_no == NULL) return 1;    
  *bit_no = BIT(timer_hook_id);
  if (sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE, &timer_hook_id) != 0) return 1;  
  return 0;
}

int (timer_unsubscribe_int)() {
  if(sys_irqrmpolicy(&timer_hook_id) != 0) return 1;
  return 0;
}

void (timer_int_handler)() {
  timer_counter++;
}






int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (st == NULL || timer < 0 || timer > 2) return 1;
  uint8_t rb_cmd = BIT(7) | BIT(6) | BIT(5) | BIT(timer + 1); // = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer)
  if (sys_outb(TIMER_CTRL, rb_cmd) != 0) return 1;
  if (util_sys_inb(TIMER_0 + timer, st)) return 1;
  return 0;
}





int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  
  if (freq < 19 || freq > TIMER_FREQ || timer < 0 || timer > 2) return 1;


  /*parte 1*/
  uint8_t rb_cmd = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer + 1);
  if (sys_outb(TIMER_CTRL, rb_cmd) != 0) return 1;

  uint8_t old_conf, new_conf = 0;
  timer_get_conf(timer, &old_conf);

  //-> new configuration:

  //timer selection
  uint8_t selectedTimer;

  switch(timer) {
      case 0: 
        new_conf |= TIMER_SEL0; 
        selectedTimer = TIMER_0;
        break;
      
      case 1: 
        new_conf |= TIMER_SEL1; 
        selectedTimer = TIMER_1;
        break;
      
      case 2: 
        new_conf |= TIMER_SEL2;
        selectedTimer = TIMER_2;
        break;
      
      default: 
        return 1;
  }

  //register selection
  new_conf |= TIMER_LSB_MSB;

  //!!! we do not change 4 LSB (mode and BCD/binary)
  new_conf |= old_conf & 0x0F;


  
  /*parte 2*/

  //calculo do novo valor para o contador c/ nova freq
  uint16_t counter = TIMER_FREQ / freq; 
  uint8_t LSB, MSB;

  util_get_LSB(counter, &LSB);
  util_get_MSB(counter, &MSB);


  if (sys_outb(TIMER_CTRL, new_conf) != 0) return 1;
  if (sys_outb(selectedTimer, LSB) != 0) return 1;
  if (sys_outb(selectedTimer, MSB) != 0) return 1;

  return 0;
}
