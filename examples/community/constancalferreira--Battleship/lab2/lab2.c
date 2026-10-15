#include <lcom/lcf.h>
#include <lcom/lab2.h>

#include <stdbool.h>
#include <stdint.h>

extern int timer_counter;


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

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {
  uint8_t config;                                          // vamos colocar aqui a configuração do timer
  if (timer_get_conf(timer, &config)) return 1;            // chamamos função que lê a configuração do timer e coloca-a em config
  if (timer_display_conf(timer, config, field)) return 1;  // chamamos a função que dá display da configuração
  return 0;           
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  if (timer_set_frequency(timer, freq) != 0) return 1;
  return 0;
}



int(timer_test_int)(uint8_t time) {

  int ipc_status, r;
  message msg;
  uint8_t irq_set;

  //important notice -> the default frequency is 60Hz
  //meaning that the timer will generate an interrupt 60 times per second

  //we subscribe interrupt 
  if (timer_subscribe_int(&irq_set) != 0) return 1;

  //and we create a loop that will run for the time we want to count
  while( time > 0 ) { /* time interval  */
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {  /*ANY -> accept messages from any source; &msg -> where we store the message; &ipc_status -> stores status flags about message (+what kind of message)*/
        printf("driver_receive failed with: %d", r);
        continue;
    }

    //we check the message received from driver_receive, to see if it is a notification
    if (is_ipc_notify(ipc_status)) { /* received notification */
        //we check the source of the notification (for example, hardware...)
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */				
                //if the interrupts that happenned (msg.m_notify...) matches the one we subscribed for the timer (in irq_set) ...
                if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
                    timer_int_handler();
                    if (timer_counter%60 == 0) {   //each second we print a message ! -> 60 timer ticks have passed
                      timer_print_elapsed_time();
                      time--;
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
  
  if(timer_unsubscribe_int() != 0) return 1;
  return 0;
}
