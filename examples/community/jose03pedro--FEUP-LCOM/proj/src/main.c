/**
 * @file main.c
 * @author
 * @brief 
 * @version 0.1
 * @date 2023-05-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "devices/keyboard/keyboard.h"
#include "devices/mouse/mouse.h"
#include "devices/timer/timer.h"
#include "devices/video/graphics.h"
#include "game_logic.h"
#include "game_view.h"
#include <lcom/lcf.h>

extern GameState gameState;

int(main)(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/g2/proj/src/minix_debug/trace.txt");
  lcf_log_output("/home/lcom/labs/g2/proj/src/minix_debug/output.txt");
  if (lcf_start(argc, argv))
    return 1;
  lcf_cleanup();
  return 0;
}

/**
 * @brief Calls all the functions needed to set up the environment for the game to run.
 *
 */
int init() {
  timer_set_frequency(TIMER, FPS);
  set_frame_buffers(MODE);
  set_graphic_mode(MODE);
  sprites_creation();
  timer_subscribe_interrupts();
  mouse_subscribe_interrupts();
  keyboard_subscribe_interrupts();
  mouse_write(ENABLE_STREAM_MODE);
  return mouse_write(ENABLE_DATA_REPORT);
}

/**
 * @brief Calls all the functions needed to stop execution. Exits minix graphic mode, and unsubscribes interrupts.
 *
 */
int die() {
  vg_exit();
  timer_unsubscribe_interrupts();
  mouse_unsubscribe_interrupts();
  keyboard_unsubscribe_interrupts();
  return mouse_write(DISABLE_DATA_REPORT);
}

/**
 * @brief The main loop of the game.
 *
 */
int(proj_main_loop)(int argc, char *argv[]) {
  init();
  draw_new_screen();

  // driver receive interrupts
  int ipc_status;
  message msg;
  while (gameState == RUNNING) {

    if (driver_receive(ANY, &msg, &ipc_status) != 0)
      continue;

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & TIMER_MASK)
            update_timer();
          if (msg.m_notify.interrupts & KEYBOARD_MASK)
            update_keyboard();
          if (msg.m_notify.interrupts & MOUSE_MASK)
            update_mouse();
      }
    }
  }

  return die();
}
