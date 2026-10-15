#include "ev_listener.h"

extern uint8_t scancode;
extern int timer_counter;
extern datetime_t curr_time;
int state_changed = 0;

handler listeners[] = {
  handle_main_menu,
  handle_game_betting,
  handle_game_playing,
  handle_game_over,
  handle_dealer_turn,
  NULL
};

void handle_interrupt(app_t *app, interrupt_type_t interrupt) {
  handle_general(app, interrupt);
  handler hd = listeners[app->state];
  if (hd == NULL)
    return;
  if (state_changed) {
    state_changed = 0;
    return;
  }

  hd(app, interrupt);
}

void handle_general(app_t *app, interrupt_type_t interrupt) {
	uint8_t byte;

  switch (interrupt) {
    case KEYBOARD:
      kbc_ih();
      break;
    case MOUSE:
      mouse_ih();

      if (mouse_read_packet()) {
        mouse_info_t *info = mouse_get_info();
        if (info == NULL)
          return;

        uint32_t new_x = app->cursor.x + info->delta_x;
        uint32_t new_y = app->cursor.y - info->delta_y;

        cursor_move(&app->cursor, new_x, new_y);
        app->cursor.state = POINTER;

        vg_set_redraw();
      }
      break;
    case UART:
      if (uart_ih()) {
        while (!uart_get_byte(&byte)) {
					com_add_byte(byte);
					com_handle_packet(app);
				}
      }
      break;
    case TIMER:
      timer_int_handler();
      banner_update_timeout(&app->banner);
      if (app->state != MAIN_MENU)
        animation_run(&app->game.curr_anim, app);
      draw_screen(app);
      break;
    case RTC:
      rtc_ih();
      if (curr_time.seconds == 0 && curr_time.minutes % 5 == 0 && app->game.dealer != NULL) {
        banner_set_message(&app->banner, "You received a bonus ~50", 120);
        app->game.main_player.coins += 50;
      }
      break;
  }
}
