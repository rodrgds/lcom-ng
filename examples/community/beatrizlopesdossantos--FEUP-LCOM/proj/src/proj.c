// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Any header files included below this line should have been created by you
#include "timer.h"
#include "keyboard.h"
#include "mouse.h"
#include "video.h"
#include "rtc.h"
#include "initiate.h"
#include "mainmenu.h"
#include "game.h"
#include "i8042.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

extern uint8_t kbd_bit_no;
extern uint8_t mouse_bit_no;
extern uint8_t timer_bit_no;
extern uint8_t rtc_bit_no;

extern uint8_t scancode;
uint8_t bytes[2];
uint8_t size = 0;
uint32_t mouse_counter = 0;
uint8_t byte;
uint8_t packet[3];
bool esq = false;

int(proj_main_loop)(int argc, char *argv[]) {

  enum location state;
  state = ENTRANCE;

  load_xpms();
  inicialize_minix();
  create_sprites();
  
  int ipc_status;
  uint32_t irq_kbd_set = BIT(kbd_bit_no);
  uint32_t irq_mouse_set = BIT(mouse_bit_no);
  uint32_t irq_timer_set = BIT(timer_bit_no);
  uint32_t irq_rtc_set = BIT(rtc_bit_no);
  message msg;
  int r;

  clear_sec_buff();
  show_entrada();
  show_cursor();
  refresh();

  while (esq != true) { 
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d",r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE:
        if (msg.m_notify.interrupts & irq_mouse_set) {
          mouse_ih();
          if (mouse_counter == 0 && (byte & CHECK)) {
            packet[0] = byte;
            mouse_counter++;
          }
          else if (mouse_counter == 1) {
            packet[mouse_counter] = byte;
            mouse_counter++;
          }
          else if (mouse_counter == 2) {
            packet[mouse_counter] = byte;
            packet_make();
            mouse_counter = 0;
            handler(&state,MOUSE);
          }
        }
        if (msg.m_notify.interrupts & irq_kbd_set) {
          kbc_ih();
          if (scancode == FIRST_OF_TWO_BYTES) {
            bytes[0] = scancode;
            size = 1;  
          }
          else {
            
            bytes[size] = scancode;
            size++;
            size = 0; 

          }

          handler(&state,KEYBOARD);
        }
        if (msg.m_notify.interrupts & irq_timer_set) {
          timer_int_handler();
          handler(&state,TIMER);
        }
        if (msg.m_notify.interrupts & irq_rtc_set) {
          if (rtc_ih()){
            handler(&state,RTC);
          }

        }
        break;
      default:
        break;

      }
    }

    else { 
    }
  }

  destroy_all_sprites();
  exit_minix();
  
  return 0;
}

