#include <lcom/lcf.h>
#include <lcom/proj.h>

#include <stdio.h>
#include <stdint.h>

#include "devices/timer/timer.h"
#include "devices/kbc/keyboard/keyboard.h"
#include "devices/kbc/mouse/mouse.h"
#include "devices/graphics/graphics.h"
#include "devices/kbc/i8042.h"

#include "controller/game_controller.h"
#include "controller/timer_controller/timer_controller.h"
#include "controller/keyboard_controller/keyboard_controller.h"
#include "controller/mouse_controller/mouse_controller.h"
#include "controller/game_controller.h"

#include "view/initialize_pixmaps.h"
#include "view/maze_view/maze_view.h"
#include "view/pacman_view/pacman_view.h"
#include "view/ghosts_view/ghosts_view.h"
#include "view/timer_view/timer_view.h"
#include "view/cursor_view/cursor_view.h"
#include "view/menu_view/menu_view.h"

extern bool menu_is_on;
extern bool game_is_on;
extern bool pause_is_on;
extern bool exited;
extern bool initializing;
extern unsigned int score;
extern unsigned int game_time;

extern uint8_t* timer_bit_no;
extern uint8_t* keyboard_bit_no;
extern uint8_t* mouse_bit_no;

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

int (menu_loop)() {
  if (subscribe_menu_devices() != 0)
    return 1;
  draw_menu();

  message msg;
  int ipc_status, r;
  int mouse_irq_set = BIT(*mouse_bit_no);
  while (menu_is_on) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:        
            if (msg.m_notify.interrupts & mouse_irq_set) {
              mouse_interrupt_handler();
            }
            break;
        default:
            break;
      }
    }
  }

  erase_menu();
  if (unsubscribe_menu_devices() != 0)
    return 1;
  return 0;
}

int (game_loop)() {
  if (subscribe_game_devices() != 0)
    return 1;
  if (!pause_is_on)
    draw_game_elements(); 
  initializing = false;

  message msg;
  int ipc_status, r;
  int timer_irq_set = BIT(*timer_bit_no);
  int keyboard_irq_set = BIT(*keyboard_bit_no);
  while (game_is_on) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:        
            if (msg.m_notify.interrupts & timer_irq_set) {
              timer_interrupt_handler();
              if (!pause_is_on) {
                erase_timer();
                draw_timer();
              }
            }
            if (msg.m_notify.interrupts & keyboard_irq_set) {
              keyboard_int_handler();
              process_key();
            }
            break;
        default:
            break;
      }
    }
  }
  if (unsubscribe_game_devices() != 0)
    return 1;
  return 0;
}

int (proj_main_loop)(int argc, char *argv[]) {
  uint16_t mode = 0x105;
  if (graphics_init(mode) != 0) {
    vg_exit();
    return 1;
  }
  initialize_game_elements();

  if (menu_loop() != 0) {
    vg_exit();
    return 1;
  }
  if (game_loop() != 0) {
    vg_exit();
    return 1;
  }

  if (!exited) {
    if (game_time == 0) {
      draw_win_menu();
      sleep(5);
    } else {
      draw_game_over_menu();
      sleep(5);
    }
  }

  if (vg_exit() != 0)
    return 1;
  return 0;
}
