#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"
#include "KBC.h"  
#include "kbd.h"
//#include "../lab2/timer.c" 


extern uint8_t scancode;
extern uint32_t kbc_counter;
extern int timer_counter;


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

  int ipc_status, r;
  message msg;
  uint8_t kbd_irq_set;

  //we subscribe interrupt 
  if (kbd_subscribe_int(&kbd_irq_set) != 0) return 1;

  //the function is defined to run until the ESC key is pressed
  //so when we receive the breakcode for the ESC key, we must stop the loop
  while (scancode != ESC_BREAK) { 
    
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */				
                if (msg.m_notify.interrupts & kbd_irq_set) { /* subscribed interrupt */
                    kbc_ih(); //we read the scancode again to check if anything changed      
                    process_scancode(scancode);
                }
                break;
            default:
                break; /* no other notifications expected: do nothing */	
        }
    } else { /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
    }
  }
  
  if (kbd_unsubscribe_int() != 0) return 1;
  if (kbd_print_no_sysinb(kbc_counter) != 0) return 1; 
  return 0;
}



//unlike before, we are continuosly reading from the OUT_BUF, even if nothing has happened
int(kbd_test_poll)() {
  while (scancode != ESC_BREAK) {
    if (read_KBC_output(OUT_BUF, &scancode) == 0) {
      process_scancode(scancode);
    }
  }

  //we restore the previous keyboard state -> see slide 22
  if (kbd_restore() != 0) return 1;
  if (kbd_print_no_sysinb(kbc_counter) != 0) return 1; 
  return 0;
}



int(kbd_test_timed_scan)(uint8_t n) {
  int ipc_status, r;
  message msg;
  
  uint8_t timer_irq_set;
  uint8_t kbd_irq_set;

  int seconds = 0;

  //we subscribe interrupts 
  if (timer_subscribe_int(&timer_irq_set) != 0) return 1;
  if (kbd_subscribe_int(&kbd_irq_set) != 0) return 1;

  //the function is defined to stop when the ESC key is pressed
  //or if it does not receive a scancode for a number of seconds equal to n
  while (scancode != ESC_BREAK && seconds < n ) { 
    
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: 			

                if (msg.m_notify.interrupts & kbd_irq_set) { 
                    kbc_ih();      
                    process_scancode(scancode);
                    timer_counter = 0;
                    seconds = 0;
                }

                if (msg.m_notify.interrupts & timer_irq_set) { 
                    timer_int_handler();
                    if (timer_counter%60 == 0) { //one more second passed
                      seconds++;
                      //printf("seconds: %d\n", seconds);
                      //timer_print_elapsed_time();
                    } 
                } 

                break;

            default:
                break; /* no other notifications expected: do nothing */	
        }
    }

  }
  
  if (timer_unsubscribe_int() != 0) return 1;
  if (kbd_unsubscribe_int() != 0) return 1;
  if (kbd_print_no_sysinb(kbc_counter) != 0) return 1; 
  return 0;
}
