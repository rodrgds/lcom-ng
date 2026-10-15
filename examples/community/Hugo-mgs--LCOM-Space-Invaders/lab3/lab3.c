#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include <stdbool.h>

#include "i8042.h"
#include "kbd.h"
#include "KBC.c"
#include "timer.c"

extern uint32_t sys_calls_KBC;
extern uint32_t time_counter;
extern uint8_t scancode;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  int ipc_status;
  message msg;

  uint8_t bit_no;
  if(kbd_subscribe_int(&bit_no) != 0) return 1;

  while(scancode != BREAK_ESC) { /* You may want to use a different condition */
   /* Get a request message. */
    if ( (driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d");
        continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: 	
              if (msg.m_notify.interrupts & bit_no) { 
                  kbc_ih();
                  kbd_print_scancode(!(scancode & BIT(7)), scancode == 0xE0 ? 2 : 1, &scancode);
              }
              break;
            default:
              break; /* no other notifications expected: do nothing */
        }
    } else { 
    }
  }
  
  printf("Unsubscribing\n");
  if (kbd_unsubscribe_int() != 0) return 1;
  if (kbd_print_no_sysinb(sys_calls_KBC) != 0) return 1;

  return 0;
}

int(kbd_test_poll)() {
  while (scancode != BREAK_ESC){
    uint8_t attempts = 1;
    while (attempts){
      kbc_ih();
      attempts--;
    }
  }

  KBC_reset();

  return 1;
}

int(kbd_test_timed_scan)(uint8_t n) {
  int ipc_status;
  message msg;
  uint8_t bit_no_kbd, bit_no_timer;

  int timeElapsed = 0;

  if(kbd_subscribe_int(&bit_no_kbd) != 0) return 1;
  if (timer_subscribe_int(&bit_no_timer) != 0) return 1;

  while(scancode != BREAK_ESC && timeElapsed < n) { /* You may want to use a different condition */
    /* Get a request message. */
     if ( (driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
         printf("Error");
         continue;
     }
     if (is_ipc_notify(ipc_status)) { /* received notification */
         switch (_ENDPOINT_P(msg.m_source)) {
             case HARDWARE: /* hardware interrupt notification */				
                 if (msg.m_notify.interrupts & bit_no_kbd) { /* subscribed interrupt */
                      kbc_ih();
                      kbd_print_scancode(!(scancode & BIT(7)), scancode == 0xE0 ? 2 : 1, &scancode);
                      timeElapsed = 0;
                      time_counter = 0;
                 }
                  if (msg.m_notify.interrupts & bit_no_timer) { /* subscribed interrupt */
                        timer_int_handler();
                        if (timeElapsed % sys_hz() == 0) timeElapsed++; 
                        break;
                  }
             default:
                 break; /* no other notifications expected: do nothing */	
         }
     } 
   }

  if (kbd_unsubscribe_int() != 0) return 1;
  if (timer_unsubscribe_int() != 0) return 1;
  if (kbd_print_no_sysinb(sys_calls_KBC) != 0) return 1;
 
  return 1;
}
