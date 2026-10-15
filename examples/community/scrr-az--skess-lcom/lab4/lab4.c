// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "mouse.h"
#include "timer.c"

extern struct packet mouse_packet;
extern int byte_index;
extern int counter_timer;

int state; //variable to keep the state for the gesture test
/*
0: start (no gesture for ending has been made, passes to the next when it notices that only the left button is pressed)
1: up (right button has been pressed, when it detects upwards right motion passes to vertex)
2: vertex (when it detects that only the right button is pressed passes to down)
3: down (when it detects downwards right motion passes to end)
4: end (stop the cycle)
at any moment, when a non-expected gesture is detected (outside of the tolerance), return to 0
*/

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
  //primeiro enable depois subscribe
  int ipc_stat;
  uint8_t mask;
  message msg;

  if(mouse_write_command(ENABLE_DATA_REPORTING)) return 1; //enables data reporting
  if(mouse_subscribe_int(&mask)) return 1; //subscribe to mouse interruptions

  while (cnt>0){
    if(driver_receive(ANY, &msg, &ipc_stat)) continue; //error on driver_receive, try again

    if(is_ipc_notify(ipc_stat)){ //if it was a notification
      switch(_ENDPOINT_P(msg.m_source)){ //check where the message comes from

        case HARDWARE: //it came from a hardware device
          if(msg.m_notify.interrupts & mask){ //check if the interrupt was from the mouse
            mouse_ih(); //call the interruption handler

            if(byte_index == 3){ //we have read the three bytes of this packet
              byte_index = 0; //reset for next packet
              mouse_print_packet(&mouse_packet); //call the function to print the packet
              cnt --; //we have read one more packet!
            }
          }
          break;

        default:
          break; //if the notification is other than a hardware interrupt, do nothing
      }
    }
  }

  if(mouse_write_command(DISABLE_DATA_REPORTING)) return 1; //disable data reporting
  if(mouse_unsubscribe_int()) return 1;

  return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
  int ipc_stat, time_passed = 0;
  uint8_t mask_mouse, mask_timer;
  message msg;

  uint16_t frequency_timer = sys_hz(); //timer 0 frequency (aka how many times it interrupts per second)

  if(mouse_write_command(ENABLE_DATA_REPORTING)) return 1; //enables data reporting
  if(mouse_subscribe_int(&mask_mouse)) return 1; //subscribe to mouse interruptions
  if(timer_subscribe_int(&mask_timer)) return 1;


  while (time_passed<idle_time){
    if(driver_receive(ANY, &msg, &ipc_stat)) continue; //error on driver_receive, try again

    if(is_ipc_notify(ipc_stat)){ //if it was a notification
      switch(_ENDPOINT_P(msg.m_source)){ //check where the message comes from

        case HARDWARE: //it came from a hardware device
          if(msg.m_notify.interrupts & mask_mouse){ //check if the interrupt was from the mouse
            mouse_ih(); //call the interruption handler

            if(byte_index == 3){ //we have read the three bytes of this packet
              byte_index = 0; //reset for next packet
              mouse_print_packet(&mouse_packet); //call the function to print the packet
            }
            counter_timer =0;
            time_passed =0;
          }
          if(msg.m_notify.interrupts & mask_timer){ //check if the interrupt was from the timer
            timer_int_handler();
            if (counter_timer%frequency_timer == 0) time_passed ++;
          }
          break;

        default:
          break; //if the notification is other than a hardware interrupt, do nothing
      }
    }
  }

  if(mouse_write_command(DISABLE_DATA_REPORTING)) return 1; //disable data reporting
  if(mouse_unsubscribe_int()) return 1;
  if(timer_unsubscribe_int()) return 1;

  return 0;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  int ipc_stat;
  uint8_t mask;
  message msg;

  if(mouse_write_command(ENABLE_DATA_REPORTING)) return 1; //enables data reporting
  if(mouse_subscribe_int(&mask)) return 1; //subscribe to mouse interruptions

  
  
  return 0;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
