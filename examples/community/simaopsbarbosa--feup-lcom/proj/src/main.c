#include <lcom/lcf.h>
#include <lcom/timer.h>

#include "controllers/KBC/keyboard.h"
#include "controllers/KBC/mouse.h"
#include "controllers/video/video.h"

#include "model/state.h"
#include "model/modes.h"
#include "view/view.h"

extern GameState gameState;
extern GameMode gameMode;

extern HandlerFn mouse_ih_array[6];
extern HandlerFn keyboard_ih_array[6];
extern HandlerFn display_ih_array[6];

extern bool new_packet;

uint8_t mask_timer = 0;
uint8_t mask_keyboard = 0;
uint8_t mask_mouse = 0;

int cursor_x = 40;
int cursor_y = 40;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  // lcf_trace_calls("/home/lcom/labs/proj/debug/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  // lcf_log_output("/home/lcom/labs/proj/debug/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int game_setup() {
  if (timer_set_frequency(0, 30) != 0) return 1;

  if (set_frame_buffers(0x115) != 0) return 1;
  if (enable_graphic_mode(0x115) != 0) return 1;

  if (timer_subscribe_int(&mask_timer) != 0) return 1;
  if (keyboard_subscribe_interrupts(&mask_keyboard) != 0) return 1;
  if (mouse_subscribe_int(&mask_mouse) != 0) return 1;
  
  if (mouse_write_command(MOUSE_ENABLE_DATA_REPORT) != 0) return 1;

  view_init();
  
  return 0;
}

int game_exit() {
  destroy_xpms();
  if (vg_exit() != 0) return 1;

  if (mouse_write_command(MOUSE_DISABLE_DATA_REPORT) != 0) return 1;

  if (timer_unsubscribe_int() != 0) return 1;
  if (keyboard_unsubscribe_interrupts() != 0) return 1;
  if (mouse_unsubscribe_int() != 0) return 1;

  return 0;
}



int (proj_main_loop)(int argc, char *argv[]) {
  if (game_setup() != 0) return game_exit();

  draw_player_menu_frame();

  int ipc_status;
  message msg;
  while (gameState == RUNNING) {
    
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("Error");
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch(_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 
          if (msg.m_notify.interrupts & mask_timer) {
            update_timer_state();
            if (timer_ih_array[gameMode] != NULL) {
              timer_ih_array[gameMode]();
            }
          }
          if (msg.m_notify.interrupts & mask_keyboard){
            update_keyboard_state();
            if (keyboard_ih_array[gameMode] != NULL) {
              keyboard_ih_array[gameMode]();
            }
          }
          if (msg.m_notify.interrupts & mask_mouse){
            update_mouse_state();
            if (mouse_ih_array[gameMode] != NULL && new_packet) {
              mouse_ih_array[gameMode]();
            }
          }
    }
  }
 }
  
  if (game_exit() != 0) return 1;

  return 0;
}

