#include "timer.h"

int hook_id_timer = 0;

int timer_subscribe_interrupt(){
  hook_id_timer = TIMER_KEY;
  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id_timer)){
    printf("Error while subscribing to timer's interrupts\n");
    return 1;
  }
  return 0;
}

int timer_unsubscribe_interrupt(){
  if(sys_irqrmpolicy(&hook_id_timer)){
    printf("Error while unsubscribing to timer's interrupts\n");
    return 1;
  }
  return 0;
}

bool is_timer_0_interrupt(int ipc_status, message msg){
  return (is_ipc_notify(ipc_status) && 
         (_ENDPOINT_P(msg.m_source) == HARDWARE) && 
         (msg.m_notify.interrupts & IRQ_SET_TIMER));
}
