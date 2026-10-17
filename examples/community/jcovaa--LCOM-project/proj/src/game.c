#include "game.h"
#include "drivers/video/graphics.h"
#include "drivers/kbc/kbc_utils.h"
#include "drivers/kbc/keyboard.h"
#include "drivers/kbc/mouse.h"
#include "drivers/timer/timer.h"
#include "states/start_menu.h"
#include "states/instructions.h"
#include "states/show_original.h"
#include "states/user_paint.h"
#include "states/results.h"
#include "ui_assets.h"
#include "grid.h"
#include "levels.h"

extern uint16_t x_resolution;
extern uint16_t y_resolution;

extern int timer_counter;

extern struct packet mouse_packet;  
extern uint8_t packet_byte;
extern uint16_t mouse_x;
extern uint16_t mouse_y;

int game_loop() {
  GameState state = START_MENU;
  bool exit_game = false;

  uint8_t irq_kb, irq_timer, irq_mouse;

  if (mouse_init() != 0) {
    return 1;
  }

  if (timer_subscribe_int(&irq_timer) != 0) {
    return 1;
  }

  if (keyboard_subscribe_interrupts(&irq_kb) != 0) {
    return 1;
  }

  if (mouse_subscribe_interrupts(&irq_mouse)) {
    return 1;
  }

  init_start_menu();
  
  int ipc_status;
  message msg;
  
  while (!exit_game) {
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_timer) {
            timer_counter++;

            clear_secondary_buffer();

            switch (state) {
              case START_MENU:
                draw_start_menu();
                break;
              case INSTRUCTIONS:
                draw_instructions();
                break;
              case SHOW_ORIGINAL:
                draw_show_original(&state);
                break;
              case USER_PAINT:
                draw_user_paint(&state);
                break;
              case RESULTS:
                draw_results(&state);
                break;
              case EXIT_GAME:
                exit_game = true;
                break;
              default:
                break;
            }

            draw_mouse_cursor(mouse_x, mouse_y);

            copy_buffer_to_video();
          }

          if (msg.m_notify.interrupts & irq_kb) {
            kbc_ih();

            switch (state) {
              case START_MENU:
                handle_menu_input(&state);
                break;
              case INSTRUCTIONS:
                handle_instructions_input(&state);
                break;
              case USER_PAINT:
                handle_user_paint_input(&state);
                break;
              case RESULTS:
                handle_results_input(&state);
                break;
              default:
                break;
            }
          }

          if (msg.m_notify.interrupts & irq_mouse) {
            mouse_ih();
            mouse_packet_sync();

            if (packet_byte == 3) {
              packet_byte = 0;
              mouse_parse_packet();
              mouse_update_position();
            }

            switch (state) {
              case START_MENU:
                handle_menu_input(&state);
                break;
              case RESULTS:
                handle_results_input(&state);
                break;
              default:
                break;
            }
          }
        break;
      }
    }
  }

  if (mouse_unsubscribe_interrupts() != 0) {
    return 1;
  }

  if (keyboard_unsubscribe_interrupts() != 0) {
    return 1;
  }

  if (mouse_reset() != 0) {
    return 1;
  }

  if (kbc_flush_input() != 0) {
    return 1;
  }

  if (timer_unsubscribe_int() != 0) {
    return 1;
  }

  return 0;
}
