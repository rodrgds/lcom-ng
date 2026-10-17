#include "game.h"

extern bool quit;


int(gameLoop)() {
  initGame();
  uint8_t bit_no=0;
  int ipc_status;
  message msg;
  int r; 
  if (timer_subscribe_int(&bit_no)){
    return 1;
  }
  if (kdb_subscribe_int(&bit_no)){
    return 1;
  }
  if (rtc_subscribe_int(&bit_no)){
    return 1;
  }
  if(toggle_data_report(ENABLE_MOUSE)){
    return 1;
  }
  if (mouse_subscribe_int(&bit_no)){
    return 1;
  }
  uint32_t timer_irq_set = BIT(0);
  uint32_t kb_irq_set= BIT(1);
  uint32_t rtc_irq_set= BIT(8);
  uint32_t mouse_irq_set= BIT(2);

  while(!quit) { 
    if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { 
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & kb_irq_set) {
            kbc_ih();
            DeviceHandler(KEYBOARD);
          }
          if (msg.m_notify.interrupts & timer_irq_set ){
            timer_int_handler();
            DeviceHandler(TIMER);
          }
          if ((msg.m_notify.interrupts & mouse_irq_set)) {
            mouse_ih();
            DeviceHandler(MOUSE);
          }

          if (msg.m_notify.interrupts & rtc_irq_set ){
            rtc_ih();
            DeviceHandler(RTC);
          }
          break;
        default:
          break;  
      }
    } else { 
    } 
  }
  if(kdb_unsubscribe_int()){
    return 1;
  }
  if (timer_unsubscribe_int()){
    return 1;
  }
  if (rtc_unsubscribe_int()){
    return 1;
  }
  if (mouse_unsubscribe_int()){
    return 1;
  }
  if (toggle_data_report(DISABLE_MOUSE)){
    return 1;
  }

  return 0;

}
