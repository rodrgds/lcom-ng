#include <lcom/lcf.h>
#include <keyboard.h>
#include <stdint.h>
#include "i8042.h"

int keyboard_hook_id = 1;
uint8_t scancode;

int (keyboard_subscribe)(uint8_t *bit_no){
    if (bit_no == NULL) return 1;
    *bit_no = BIT(keyboard_hook_id);
    if (sys_irqsetpolicy(IRQ_LINE_KEYBOARD,IRQ_REENABLE|IRQ_EXCLUSIVE,&keyboard_hook_id)) return 1;    
    return 0;
}

int (keyboard_unsubscribe) (){
    if(sys_irqrmpolicy(&keyboard_hook_id)) return 1;
    return 0;
}

void (kbc_ih_keyboard)() { // read scancode
    int tries = 10;
    uint8_t status;
    while(tries){
        if(util_sys_inb(STATUS_PORT, &status)) return;

        if(status & OUT_BUF_FULL) {
            if(status & (TIMEOUT_ERROR|PARITY_ERROR)) return;
            else{
                if (util_sys_inb(DATA_PORT,&scancode)) return;
            }
        }
    tries--;
    }
}

int (escape_key) (){
    int ipc_status, r;
  message msg;
  uint8_t irq_set;
  if(keyboard_subscribe(&irq_set)) return 1;
  while (scancode != ESC_BREAK_CODE)
  {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
    printf("driver_receive failed with: %d", r);
    continue;
  }
    if (is_ipc_notify(ipc_status)) { /* received notification */
    switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE: /* hardware interrupt notification */
        if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
          kbc_ih_keyboard();
        }
        break;
      default:
        break; /* no other notifications expected: do nothing */
    }
  } else { /* received a standard message, not a notification */
    /* no standard messages expected: do nothing */
  }
  }
  if(keyboard_unsubscribe()) return 1;
  return 0;
}


