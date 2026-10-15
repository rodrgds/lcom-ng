#include <lcom/lcf.h>
#include <lcom/lab2.h>
#include "timer.h"

#include <stdbool.h>
#include <stdint.h>


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

//Read the configuration of the specified timer and display it
int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {
  /* Check if the timer is valid */
  if(timer < 0 || timer > 2) return 1;

  uint8_t st;
  if(timer_get_conf(timer, &st)) return 1;
  if(timer_display_conf(timer, st, field)) return 1;

  return 0;
}

//Set the frequency of the specified timer
int(timer_test_time_base)(uint8_t timer, uint32_t freq) {

  /* Check if the timer is valid */
  if(timer < 0 || timer > 2) return 1;

  if(timer_set_frequency(timer, freq)) return 1;
  
  return 0;
}



//Test the interrupts of the specified timer
int(timer_test_int)(uint8_t timer) {

  /* Check if the timer is valid */
  if(timer < 0 || timer > 2) return 1;

  /* subscribe timer interrupts */
  uint8_t bit_no;
  if(timer_subscribe_int(&bit_no)) return 1;

  uint32_t irq_set = BIT(bit_no); // used to check if the received notification is an interrupt notification

  int r;
  int ipc_status;
  message msg;
  
  if(set_int_counter(0)) return 1; //initialize the counter to 0
  
  while(timer > 0) {
      //Get a notification and handle it
      if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
          printf("driver_receive failed with: %d", r);
          continue;
      }

      //if the message is a notification, check if it's an interrupt notification and handle it
      if (is_ipc_notify(ipc_status)) {  /* received notification */
          switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: /* hardware interrupt notification */				
                  if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
                      timer_int_handler(); //increase the counter

                      if(get_int_counter() % 60 == 0) {
                        timer_print_elapsed_time(); //print the elapsed time every second
                        timer--;
                      }
                  }
                  break;
              default:
                  break; /* no other notifications expected: do nothing */	
          }
      } else { /* received a standard message, not a notification */
          /* no standard messages expected: do nothing */
      }
  };

  if(timer_unsubscribe_int()) return 1;
  return 0; 
}
