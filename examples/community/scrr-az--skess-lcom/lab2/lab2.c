#include <lcom/lcf.h>
#include <lcom/lab2.h>

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

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {
  if (timer > 2) return 1; //Checks if timer is valid (uint8_t is always positive, so it only needs to be <= 2)

  uint8_t conf;
  
  if (timer_get_conf(timer, &conf) != 0) return 1; //puts the timer configuration in conf

  if (timer_display_conf(timer, conf, field) != 0) return 1; //prints the configuration in conf

  return 0;
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  if (timer > 12 || freq < 19 || freq > TIMER_FREQ) return 1; //check validity of timer and of frequency

  if(timer_set_frequency(timer, freq)) return 1; //updates the timer frequency

  return 0;
}

int(timer_test_int)(uint8_t time) {
  int ipc_stat;
  uint8_t mask;
  message msg;

  if(timer_subscribe_int(&mask)!=0) return 1; //subscribes to the interruptions of timer0

  while(time>0){
    if(driver_receive(ANY, &msg, &ipc_stat)){ //did not recieve what was expected
      continue;
    }

    if(is_ipc_notify(ipc_stat)){ //the message recieved was a notification
      switch(_ENDPOINT_P(msg.m_source)){ //check where the message comes from
        case HARDWARE: //it came from a hardware device (like a timer)
          if(msg.m_notify.interrupts & mask){ //check if the interrupt was from the timer 0
            timer_int_handler(); //call the interruption handler (count++)

            if(counter%60 == 0){ //the counter is at a multiple of 60 (one more minute has passed)
              time --;
              timer_print_elapsed_time();
            }
          }

        default:
          break; //if the notification is other than a timer 0 interrupt, do nothing
      }

    }
  }

  if (timer_unsubscribe_int()) return 1; //done with the timer, unsubscribe from the notifications
  return 0;
}
