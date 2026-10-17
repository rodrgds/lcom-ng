#include "controller/graphics_card/VBE.h"
#include "controller/graphics_card/graphics_card.h"
#include "controller/keyboard/i8042.h"
#include "controller/timer/i8254.h"
#include "controller/timer/timer.h"
#include "controller/keyboard/keyboard.h"
#include "controller/mouse/mouse.h"
#include "controller/uart/ser_port.h"
#include "model/model.h"
#include "model/base_frame.h"

#include "global_vars.h"
#include <lcom/lcf.h>
#include <lcom/video_gr.h>


/**
 * @file proj.c
 *
 * \dot
 * digraph MyFiles {
 *   // Graph directives and node/edge definitions

 *   // Example graph attributes for layout
 *   rankdir=TB;
 *   splines=polyline; // Use curved edges
 *   nodesep=10; // Adjust the distance between nodes
 *   ranksep=0.5; // Adjust the distance between ranks
 *   node [shape=rectangle]; // Set node shape
 *   edge [color=blue]; // Set edge color
 * }
 * \enddot
 */


uint8_t irq_set_timer, irq_set_keyboard, irq_set_uart;
int irq_set_mouse, ipc_status, r;
message msg;
extern uint16_t bytes_per_pixel;
extern uint16_t h_res;
extern uint16_t v_res;
extern SystemState systemState;
extern struct leaderboardValue leaderboard[5];

int(main)(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/g1/proj/src/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/g1/proj/src/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();
  return 0;
}

int (start_settings)() {
  if (vg_init(VBE_MODE) == NULL)
    return EXIT_FAILURE;

  setup_sprites();
  config_uart();

  if (set_data_reporting(TRUE) != OK) 
    return EXIT_FAILURE;
  if (keyboard_subscribe_int(&irq_set_keyboard) != OK)
    return EXIT_FAILURE;
  if (timer_subscribe_int(&irq_set_timer) != OK)
    return EXIT_FAILURE;
  if (mouse_subscribe_int(&irq_set_mouse) != OK)
    return EXIT_FAILURE;
  if (timer_set_frequency(SEL_TIMER0, FPS) != OK)
    return EXIT_FAILURE;
  if (subscribe_uart_int(&irq_set_uart) != OK)
    return EXIT_FAILURE;

  create_frame_buffer(h_res, v_res, bytes_per_pixel);
  loadLeaderboardFromFile(leaderboard);
  return 0;  
}

int (reset_settings)() {
  if (keyboard_unsubscribe_int(&irq_set_keyboard) != OK)
    return EXIT_FAILURE;
  if (timer_unsubscribe_int() != OK)
    return EXIT_FAILURE;
  if (mouse_unsubscribe_int(&irq_set_mouse) != OK)
    return EXIT_FAILURE;  
  if (set_data_reporting(FALSE) != OK) 
    return EXIT_FAILURE;
  if (vg_exit() != EXIT_SUCCESS)
    return EXIT_FAILURE;

  saveLeaderboardToFile(leaderboard);
  return EXIT_SUCCESS;
}

int (proj_main_loop)(int argc, char **argv) {
  if (start_settings() != 0) return 1;
  while (systemState == RUNNING) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE:
        if (msg.m_notify.interrupts & irq_set_mouse) {
          update_mouse_state();
        }
        if (msg.m_notify.interrupts & irq_set_timer) {
          update_timer_state();
        } 
        if (msg.m_notify.interrupts & irq_set_keyboard) {
          update_keyboard_state();
        }
        if (msg.m_notify.interrupts & irq_set_uart) {
          update_uart_state();
        }
          break;
        default:
          break;
      }
    } else {
    }
  }

  if (reset_settings() != 0) return 1;
  return 0;

}
