/** 
 * @file game.c
 * @brief Source code file for the entry point of the program, as well as the main setup and clean functions.
 */


#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#include "drivers/drivers.h"
#include "macros/scancodes.h"
#include "controller/main_controller.h"

/**
 * @brief Initializes the game by setting up what is necessary 
 * 
 * Sets the timer 0's frequency to 60hz to serve as the frame rate, subscribes the timer, keyboard and mouse interruptions (the last one after enabling data reporting) and then initizalies the graphics mode and vram mapping. <br>
 * After setting up the drivers it calls setup_game() to initialize the program logic.
 * 
 * @return 0 in case of success, 1 otherwise
 */
int (game_init)() {
  uint8_t timer_int_no = 0, keyboard_int_no = 1, mouse_int_no = 2;
  const int frame_rate = 60;

  if(timer_set_frequency(TIMER_0, frame_rate)) return 1; // set the frame rate on timer 0
  if(timer_subscribe_int(&timer_int_no)) return 1; // subscribe the timer 0 interruptions
  if(keyboard_subscribe_int_exclusive(&keyboard_int_no)) return 1; // subscribe keyboard ints
  if(mouse_stream_enable_data_reporting()) return 1; // enable reporting on the mouse
  if(mouse_subscribe_int_exclusive(&mouse_int_no)) return 1; // subscribe the interruptions reported
  if(vg_graphics_init(0x14C)) return 1; // set the graphics mode as 0x14C and map the vmem

  if(setup_game()) return 1;

  return 0;
}

/**
 * @brief Cleans up the game by reverting to the starting state
 * 
 * Sets the timer 0's frequency back to 60hz, unsubscribes the timer, keyboard and mouse interruptions, disables the mouse data reporting and then calls vg_exit(). <br>
 * After reverting the drivers' state, it calls clean_game().
 * 
 * @return 0 in case of success, 1 otherwise
 */
int (game_exit)() {
  return timer_unsubscribe_int() |
  keyboard_unsubscribe_int() |
  mouse_unsubscribe_int() |
  mouse_stream_disable_data_reporting() |
  vg_exit() | timer_set_frequency(TIMER_0, 60) | clean_game();
}

/**
 * @brief The main loop of the game
 * 
 * Sets up the game by calling the game_init() function at the beginning. <br>
 * Catches the interrupts and, for the keyboard and mouse, calls event_handle_keyboard() or event_handle_mouse(). For the timer interruptions, each one causes a frame to be processed. <br>
 * Finally, it calls game_exit() before returning.
 * 
 * @return 0 in case of success, 1 otherwise
 */
int (proj_main_loop)() {
  if(game_init()) {
    printf("Error: could not initialize the game\n");
    if(game_exit()) printf("Warning: something went wrong while cleaning up\n");
    return 1;
  }

  int ipc_status, r;
  message msg;

  // main loop
  while(get_game_state() != QUIT) {
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:		
          if (msg.m_notify.interrupts & BIT(0)) { // timer, execute one frame
            process_frame();
          }
          if (msg.m_notify.interrupts & BIT(1)) { // keyboard
            event_handle_keyboard();
          }
          if (msg.m_notify.interrupts & BIT(2)) { // mouse
            event_handle_mouse();
          }
          break;
      }
    }
  }

  if(game_exit()) {
    printf("Warning: something went wrong while cleaning up\n");
    return 1;
  }
  return 0;
}

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  //lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  //lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}
