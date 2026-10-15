#include "ev_listener.h"

extern uint8_t scancode;
extern uint8_t uart_response;

void handle_main_menu(app_t *app, interrupt_type_t interrupt) {
  switch (interrupt) {
    case KEYBOARD:
      switch (scancode) {
        case KB_ESC:
        case KB_2:
          app->state = EXIT;
          break;
        case KB_1:
        case KB_ENTER:
          app->state = GAME_BET;

          if (game_init(&app->game)) {
            app->state = EXIT;
            game_destroy(&app->game);
            return;
          }

          vg_set_redraw();
          break;
        default:
          break;
      }

      return;

    case MOUSE:
      if (cursor_sprite_colides(&app->cursor, queue_at(app->buttons_main_menu, 0))) {
        app->state = GAME_BET;

        if (game_init(&app->game)) {
          app->state = EXIT;
          game_destroy(&app->game);
          return;
        }

        vg_set_redraw();
      }

      if (cursor_sprite_colides(&app->cursor, queue_at(app->buttons_main_menu, 1))) {
        app->state = EXIT;
      }
      break;
    case UART:
      break;
    default:
      return;
  }
}
