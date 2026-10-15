#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>
#include "keyboard.h"
#include "KBC.h"

extern bool ESC_key;
extern int kb_inb_counter;
extern int counter_timer;

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

  return 0;
}

int(kbd_test_scan)() {
  int ipc_stat;
  uint8_t mask;
  message msg;

  if(keyboard_subscribe_int(&mask)) return 1; //error subscribing to keyboard interruptions

  while(!ESC_key){
    if(driver_receive(ANY, &msg, &ipc_stat)) continue; //error on driver_receive, try again

    if(is_ipc_notify(ipc_stat)){ //if it was a notification
      switch(_ENDPOINT_P(msg.m_source)){ //check where the message comes from

        case HARDWARE: //it came from a hardware device (like a keyboard)
          if(msg.m_notify.interrupts & mask){ //check if the interrupt was from the keyboard
            kbc_ih(); //call the interruption handler
          }
          break;

        default:
          break; //if the notification is other than a keyboard interrupt, do nothing
      }
    }
  }

  //finished reading keyboard, unsubscribe KBC interrupt
  if(keyboard_unsubscribe_int()) return 1; //error unsubscribing
  if(kbd_print_no_sysinb(kb_inb_counter)) return 1;

  return 0;
}

int(kbd_test_poll)() {

  uint8_t scancode = 0;
  
  while(scancode != ESC){
    if (read_KBC_output(&scancode)==0){
      int scan_size = 1;
      if(scancode==0xE0) scan_size=2;
      kbd_print_scancode(!(scancode & CHK_MAKE),scan_size, &scancode);
    }
  }

  if(keyboard_enable_interrupts()) return 1; //reenable interrupts

  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  int ipc_stat, time = 0;
  uint8_t mask_timer, mask_keyboard;
  message msg;


  if(timer_subscribe_int(&mask_timer)) return 1; //subscribe timer interrupts
  if(keyboard_subscribe_int(&mask_keyboard)) return 1; //subscribe keyboard interrupts


  while((!ESC_key) && (time<n)){
    if(driver_receive(ANY, &msg, &ipc_stat)) continue; //error on driver_receive, try again

    if(is_ipc_notify(ipc_stat)){ //if it was a notification
      switch(_ENDPOINT_P(msg.m_source)){ //check where the message comes from

        case HARDWARE: //it came from a hardware device (like a keyboard)
          if(msg.m_notify.interrupts & mask_timer){ //check if the interrupt was from the timer 0
            timer_int_handler(); //call the interruption handler (count++)

            if(counter_timer%60 == 0){ //the counter is at a multiple of 60 (one more minute has passed)
              time ++;
            }
          }
          if(msg.m_notify.interrupts & mask_keyboard){ //check if the interrupt was from the keyboard
            kbc_ih(); //call the interruption handler
            time = 0;
            counter_timer = 0; //reset timer and time because a key was pressed
          }
          break;

        default:
          break; //no other notifications expected
      }
    }
  }

  if(timer_unsubscribe_int()) return 1;
  if(keyboard_unsubscribe_int()) return 1;
  if(kbd_print_no_sysinb(kb_inb_counter)) return 1;

  return 0;
}
