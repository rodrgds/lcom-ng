// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/lab4.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "mouse.h"

extern int timer_counter;


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}



int (mouse_test_packet)(uint32_t cnt) {
  
  int ipc_status, r;
  message msg;

  uint32_t mouse_irq_set;
  if (mouse_subscribe_int(&mouse_irq_set) != 0) return 1;

  if (_mouse_enable_data_reporting() != 0) {
      mouse_unsubscribe_int();
      _mouse_disable_data_reporting(); 
      return 1;
  }

  
  while (cnt != 0) {
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
          if (msg.m_notify.interrupts & mouse_irq_set) { /* subscribed interrupt */
            mouse_ih(); 
            if (mouse_sync_bytes()) {
              struct packet pp;
              create_packet(&pp);
              mouse_print_packet(&pp);
              cnt--;
            }
          }
          break;

        default:
          break; /* no other notifications expected: do nothing */	
      }
    } else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }

  if (mouse_unsubscribe_int() != 0) return 1;
  if (_mouse_disable_data_reporting() != 0) return 1;
  return 0;
}




int (mouse_test_async)(uint8_t idle_time) {
  
  int ipc_status, r;
  message msg;

  uint32_t mouse_irq_set;
  if (mouse_subscribe_int(&mouse_irq_set) != 0) return 1;

  uint8_t timer_irq_set;
  if (timer_subscribe_int(&timer_irq_set) != 0) return 1;

  if (_mouse_enable_data_reporting() != 0) {
      timer_unsubscribe_int();
      mouse_unsubscribe_int();
      _mouse_disable_data_reporting(); 
      return 1;
  }

  uint8_t seconds = 0;

  while (seconds < idle_time) {
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
        
          if (msg.m_notify.interrupts & mouse_irq_set) { /* subscribed interrupt */
            mouse_ih(); 
            if (mouse_sync_bytes()) {
              struct packet pp;
              create_packet(&pp);
              mouse_print_packet(&pp);
              seconds = 0;
              timer_counter = 0;
            }
          }

          if (msg.m_notify.interrupts & timer_irq_set) { 
            timer_int_handler();
            if (timer_counter%60 == 0) {
              //timer_print_elapsed_time();
              seconds++;
            } 
          } 

          break;

        default:
          break; /* no other notifications expected: do nothing */	
      }
    } else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }

  if (timer_unsubscribe_int() != 0) return 1;
  if (mouse_unsubscribe_int() != 0) return 1;
  if (_mouse_disable_data_reporting() != 0) return 1;
  return 0;
}



int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) { //???
    /* To be completed */
    printf("%s: under construction\n", __func__);
    return 1;
}

