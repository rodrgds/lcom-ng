#include "ev_listener.h"

extern uint8_t scancode;

void handle_game_over_rebet(app_t *app, interrupt_type_t interrupt) {
  app->game.main_player.bet = 0;
  app->game.main_player.cards_value = 0;
  app->game.dealer_value = 0;

  queue_destroy(&app->game.main_player.cards, card_queue_destroy);
  app->game.main_player.cards = queue_create(PLAYER_MAX_DECK_SIZE);

  queue_destroy(&app->game.dealer, card_queue_destroy);
  app->game.dealer = queue_create(GAME_DECK_AMOUNT * GAME_DECK_SIZE);

  if (app->game.cards->curr_size < GAME_DECK_SIZE)
    game_add_deck(app->game.cards, true);

  app->state = GAME_BET;
  vg_set_redraw();
}

void handle_game_over(app_t *app, interrupt_type_t interrupt) {
  if (interrupt == KEYBOARD) {
    if (scancode == KB_1 || scancode == KB_ENTER) {
      if (app->game.main_player.coins <= 0) {
        banner_set_message(&app->banner, "Insufficient balance to rebet", 60);
        return;
      }

      app->game.dealer_turn = false;
      handle_game_over_rebet(app, interrupt);
    }

    if (scancode == KB_ESC || scancode == KB_2) {
      app->state = EXIT;
      game_destroy(&app->game);
      vg_set_redraw();
    }
  }

  if (interrupt == MOUSE) {
    if (cursor_sprite_colides(&app->cursor, queue_at(app->buttons_game_over, 0))) {
      if (app->game.main_player.coins <= 0) {
        banner_set_message(&app->banner, "Insufficient balance to rebet", 60);
        return;
      }

      app->game.dealer_turn = false;
      handle_game_over_rebet(app, interrupt);
    }

    if (cursor_sprite_colides(&app->cursor, queue_at(app->buttons_game_over, 1))) {
      app->state = EXIT;
      game_destroy(&app->game);
      vg_set_redraw();
    }
  }
}
