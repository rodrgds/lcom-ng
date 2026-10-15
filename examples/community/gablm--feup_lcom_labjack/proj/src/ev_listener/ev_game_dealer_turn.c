#include "ev_listener.h"

extern uint8_t scancode;
extern int timer_counter;

void handle_dealer_card(void *ptr) {
  if (ptr == NULL)
    return;

  app_t *app = (app_t *) ptr;

  game_give_card(app->game.cards, app->game.dealer);

  timer_counter = 0;
}

void add_dealer_animation(app_t *app) {
  sprite_t *rotate_1 = sprite_create((xpm_map_t) rotate1_xpm);
  sprite_t *rotate_2 = sprite_create((xpm_map_t) rotate2_xpm);
  sprite_t *card_back = app->game.card_back;

  size_t card_pos = app->game.dealer->curr_size;
  uint32_t x = 500 + card_pos * card_back->img.width * 0.5;
  uint32_t y = 240;
  size_t animation_size = abs(GAME_DECK_DRAW_X - x) + 6;

  animation_t *move_card = animation_create(animation_size,
                                            handle_dealer_card);

  for (size_t i = 0; i < animation_size - 8; i += 15) {
    animation_add_frame(move_card, card_back,
                        GAME_DECK_DRAW_X + app->game.card_back->img.width * 0.02 - i,
                        GAME_DECK_DRAW_Y - app->game.card_back->img.height * 0.1, 0);
  }

  for (int i = 0; i < 3; i++) {
    animation_add_frame(move_card, rotate_1, x, y, 0);
  }

  for (int i = 0; i < 3; i++) {
    animation_add_frame(move_card, rotate_2, x, y, 0);
  }

  app->game.curr_anim = move_card;
}

void activate_dealer_turn(void *ptr) {
  if (ptr == NULL)
    return;

  app_t *app = (app_t *) ptr;

  app->game.dealer_turn = 1;
  app->game.dealer_ignore_last = 0;
  app->state = GAME_DEALER_TURN;

  timer_counter = 0;
}

void add_dealer_single_animation(app_t *app) {
  sprite_t *rotate_1 = sprite_create((xpm_map_t) rotate1_xpm);
  sprite_t *rotate_2 = sprite_create((xpm_map_t) rotate2_xpm);
  sprite_t *card_back = app->game.card_back;

  size_t card_pos = app->game.dealer->curr_size - 1;
  uint32_t x = 500 + card_pos * card_back->img.width * 0.5;
  uint32_t y = 240;

  animation_t *move_card = animation_create(6, activate_dealer_turn);

  for (int i = 0; i < 3; i++) {
    animation_add_frame(move_card, rotate_1, x, y, 0);
  }

  for (int i = 0; i < 3; i++) {
    animation_add_frame(move_card, rotate_2, x, y, 0);
  }

  app->game.curr_anim = move_card;
  app->game.dealer_ignore_last = 1;
}

void handle_dealer_turn(app_t *app, interrupt_type_t interrupt) {
  if (animation_running(app->game.curr_anim))
    return;

  if (interrupt == KEYBOARD) {
    if (scancode == KB_ESC) {
      app->state = MAIN_MENU;
      vg_set_redraw();
    }
  }

  if (interrupt == TIMER) {
    if (timer_counter % 20 == 0) {
      app->game.dealer_value = game_get_cards_value(app->game.dealer);

      vg_set_redraw();

      if (app->game.dealer_value >= 17) {
        if (app->game.dealer_value > 21 || app->game.dealer_value < app->game.main_player.cards_value) {
          app->game.main_player.won_coins = app->game.main_player.bet * 2;
          app->game.main_player.game_over_state = PLAYER_WIN;
					com_send_msg((proto_msg_type_t) app->game.main_player.game_over_state, app->game.main_player.won_coins);
        }
        else if (app->game.dealer_value > app->game.main_player.cards_value) {
          app->game.main_player.won_coins = 0;
          app->game.main_player.game_over_state = PLAYER_LOSS;
					com_send_msg((proto_msg_type_t) app->game.main_player.game_over_state, app->game.main_player.bet);
        }
        else {
          app->game.main_player.won_coins = app->game.main_player.bet;
          app->game.main_player.game_over_state = PLAYER_DRAW;
					com_send_msg((proto_msg_type_t) app->game.main_player.game_over_state, app->game.main_player.won_coins);
        }

        app->game.main_player.coins += app->game.main_player.won_coins;
        app->state = GAME_OVER;
      }
      else {
        add_dealer_animation(app);
      }

      timer_counter = 0;
    }
  }
}
