#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>
#include "utils.c"
#include "i8254.h"

int id;
unsigned int timer_interrupt_counter = 0;
unsigned int seconds = 0;
unsigned int minutes = 0;

int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = BIT(id);
  //subscribes notifications from timer0 based on id passed as 3rd argument
  //from now on the IRQ line 0 will be masked
  if(sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE,&id) != OK){
    printf("sys_irqsetpolicy::Error setting policy\n");
    return 1;
  }

  return 0;
}

int (timer_unsubscribe_int)() {
    if (sys_irqdisable(&id) == 0){
        if (sys_irqrmpolicy(&id) == 0) {
          return 0;
        }
        printf("timer_unsubscribe_int didn't disable the policy\n");
        return 1;
    }
    else {
      printf("timer_unsubscribe_int didn't disable interruptions\n");
      return 1;
    }
    

    
  return 1;
}

void (timer_ih)() {
  timer_interrupt_counter++;
}


void update_game_elapsed_time() {
    if (timer_interrupt_counter % 60 == 0) {
        if (seconds == 59) {
            seconds = 0;
            minutes++;
        }
        else {
          seconds++;
        }
    } 
}

void reset_game_elapsed_time(){
  seconds=0;
  minutes=0;
}



