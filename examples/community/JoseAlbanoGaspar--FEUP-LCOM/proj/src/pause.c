#include "pause.h"

extern uint16_t scancode;

int pause_loop(uint32_t irq_set_keyboard,uint32_t irq_set_mouse){

  int ipc_status;
  message msg;
  //1 is true
  int r;
  scancode = 0x0000;
  //Main loop variable
  //bool running = true;
  while (scancode != P_KEY) {
    // Get a request message
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          // hardware interrupt notification
          if (msg.m_notify.interrupts & irq_set_keyboard) { // subscribed keyboard interrupt
            kbc_ih();
            if(scancode != P_KEY) scancode = 0;
            
          }
          if (msg.m_notify.interrupts & irq_set_mouse) { // subscribed keyboard interrupt
            mouse_ih();
          }
          // hardware interrupt notification
         
          break;
        default:
          break; // no other notifications expected: do nothing
        }
    }
    else { //received a standard message, not a notification
      // no standard messages expected: do nothing
    }
  }
  scancode = 0x0000;
  return 0;
}
