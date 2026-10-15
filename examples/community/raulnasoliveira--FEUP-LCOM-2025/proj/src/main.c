#include <lcom/lcf.h>

#include"graphics.h"
#include <stdint.h>
#include <stdio.h>
#include "keyboard.h"
#include "timer.c"
#include "game.h"
#include "mouse.h"
#include "font8x8_basic.h"

extern vbe_mode_info_t information;
extern uint8_t scancode;
uint8_t keyboard_bit_no;
uint8_t timer_bit_no;
uint8_t mouse_bit_no;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  //lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(proj_main_loop)(int argc, char* argv[]) {

  start_game();
  if (timer_set_frequency(0, 250) != 0) return 1; // Set timer 0 to 250Hz
  if (keyboard_subscribe_interrupts(&keyboard_bit_no) != 0) return 1;
  if (timer_subscribe_int(&timer_bit_no) != 0) return 1;

  while (1) {
    scancode = 0;
    int selection = show_main_menu();
    scancode = 0;
    if (selection == 0) {
        video_test_reaction();
        scancode = 0;
        tickdelay(micros_to_ticks(50000)); // 50ms
    } else if (selection == 1) {
        play_memory_game();
        scancode = 0;
        tickdelay(micros_to_ticks(50000)); // 50ms
    } else if (selection == 2) {
        play_verbal_memory_test();
        scancode = 0;
        tickdelay(micros_to_ticks(50000)); // 50ms
    }
  }

  /* nunca alcançado, mas para termos a cláusula de cleanup: */

  keyboard_unsubscribe_interrupts();
  timer_unsubscribe_int();
  return 0;
}

