#include "ev_listener.h"

uint32_t last = 0;
extern uint8_t scancode;

void handle_game_betting(app_t *app, interrupt_type_t interrupt) {
  mouse_info_t *info = mouse_get_info();
  if (interrupt == KEYBOARD) {
    vg_set_redraw();

    if (app->game.input_select)
      handle_bet_value(app, interrupt);

    if (scancode == KB_ESC) {
      if (!app->game.input_select) {
        app->state = MAIN_MENU;
        game_destroy(&app->game);
        return;
      }

      app->game.input_select = false;
    }

    if (scancode == KB_ENTER)
      app->game.input_select = true;
  }

  if (interrupt == MOUSE) {
    if (info == NULL)
      return;

    if (!app->game.input_select && cursor_box_colides(&app->cursor, 460, 785, 600, 840)) {
      app->game.input_select = true;

      vg_set_redraw();
    }
    else {
      handle_bet_value(app, interrupt);
    }
  }
}

void handle_bet_value(app_t *app, interrupt_type_t interrupt) {
  switch (interrupt) {
    case KEYBOARD:
      if (scancode >= KB_1 && scancode <= KB_0) {
        last = scancode == KB_0 ? 0 : scancode - KB_1 + 1;

        uint32_t new_bet = app->game.main_player.bet * 10 + last;
        if (new_bet > 1000) {
          last = 0;
          return;
        }

        app->game.main_player.bet = new_bet;
        return;
      }

      if (scancode == KB_BACKSPC) {
        if (app->game.main_player.bet == 0) {
          last = 0;
          return;
        }

        app->game.main_player.bet = (app->game.main_player.bet - last) / 10;
        last = app->game.main_player.bet % 10;
        return;
      }

      if (scancode == KB_ENTER) {
        handle_bet_value_check(app);
      }

      return;
    case MOUSE:
      if (cursor_sprite_colides(&app->cursor, app->button_bet)) {
        handle_bet_value_check(app);
      }
      return;
    default:
      return;
  }
}

void handle_bet_value_check(app_t *app) {
  vg_set_redraw();

  if (app->game.main_player.bet == 0) {
    banner_set_message(&app->banner, "Bet must be greater than ~0", 120);
    return;
  }

  if (app->game.main_player.bet > app->game.main_player.coins) {
    banner_set_message(&app->banner, "Your balance is insufficient", 120);
    app->game.main_player.bet = 0;
    last = 0;
    return;
  }

  app->game.main_player.coins -= app->game.main_player.bet;

  app->state = GAME_PLAY;
  app->game.input_select = false;

  game_give_card(app->game.cards, app->game.dealer);
  game_give_card(app->game.cards, app->game.dealer);

  game_give_card(app->game.cards, app->game.main_player.cards);
  game_give_card(app->game.cards, app->game.main_player.cards);

  app->game.main_player.cards_value = game_get_cards_value(app->game.main_player.cards);

  if (app->game.main_player.cards_value == 21) {
    add_dealer_single_animation(app);
  }
}
