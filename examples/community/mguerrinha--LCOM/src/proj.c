#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include "configs.h"
#include "controller/timer/timer.h"
#include "controller/graphics/graphics.h"
#include "controller/keyboard/keyboard.h"
#include "controller/mouse/mouse.h"
#include "controller/rtc/rtc.h"
#include "view/view.h"
#include "model/model.h"
#include "game/game.h"

uint8_t timer_id, keyboard_id, mouse_id;
extern SystemState systemState;
extern PlayerInfo player1_info;
extern PlayerInfo player2_info;
extern int timer_counter;
extern int game_counter;
extern MenuState menuState;
extern MouseInfo mouse_info;
extern vbe_mode_info_t mode_info;
extern Arena arena_matrix;
extern enum direction aux_dir_p1;
extern enum direction aux_dir_p2;
Sprite *numSprites[10];

int (main)(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/shared/src/trace.txt");
  lcf_log_output("/home/lcom/labs/shared/src/output.txt");
  if (lcf_start(argc, argv))
    return 1;
  lcf_cleanup();
  return 0;
}

/**
 * @brief Function that initializes all components of the game.
 * @return 0 if the game was successfully initialized, 1 otherwise.
 */
int (initialize_game)() {
  if (timer_set_frequency(0, GAME_FREQUENCY)) return 1;

  if (choose_frame_buffer(GRAPHICS_MODE)) return 1;

  if (set_graphics_mode(GRAPHICS_MODE)) return 1;

  setup_sprites();

  if (timer_subscribe_int(&timer_id)) return 1;
  if (keyboard_subscribe_int(&keyboard_id)) return 1;
  if (write_to_mouse(0xEA)) return 1;
  if (write_to_mouse(0xF4)) return 1;
  if (mouse_subscribe_int(&mouse_id)) return 1;

  rtc_initialize();
  
  return 0;
}

/**
 * @brief Function that shuts down all components of the game.
 * @return 0 if the game was successfully shut down, 1 otherwise.
 */
int (shutting_down_game)() {
  if (set_text_mode()) return 1;

  free_controls_menu_buffer();
  free_end_menu_buffer();
  free_menu_buffer();
  free_sprites();

  if (mouse_unsubscribe_int()) return 1;
  if (write_to_mouse(0xF5)) return 1;
  if (keyboard_unsubscribe_int()) return 1;
  if (timer_unsubscribe_int()) return 1;

  return 0;
}

/**
 * @brief Main loop of the game, that handles all the interrupts and the game logic.
 */
int(proj_main_loop)(int argc, char *argv[]) {
  if (initialize_game()) return shutting_down_game();

  draw_next_frame();
  int ipc_status;
  message msg;

  while (systemState == RUN) {
    if (driver_receive(ANY, &msg, &ipc_status)) {
      printf("driver_receive failed");
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & timer_id) {
            change_timer_state();
            if (menuState == PLAY) {
              move_player(&player1_info, &aux_dir_p1);
              move_player(&player2_info, &aux_dir_p2);
            }
          }
          if (msg.m_notify.interrupts & keyboard_id) change_kb_state();
          if (msg.m_notify.interrupts & mouse_id)  {
            change_mouse_state();
          }
          change_rtc_state();
          draw_next_frame();
          break;
      }
    }
  }

  if (shutting_down_game()) return 1;

  return 0;
}
