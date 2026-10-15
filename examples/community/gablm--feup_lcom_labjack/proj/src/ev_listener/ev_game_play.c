#include "ev_listener.h"

extern uint8_t scancode;
extern int timer_counter;

void handle_hit(void *ptr) {
  if (ptr == NULL)
    return;

  app_t *app = (app_t *) ptr;
  player_t *player = &app->game.main_player;

  game_give_card(app->game.cards, player->cards);
  player->cards_value = game_get_cards_value(player->cards);

  if (player->cards_value > 21) {
    player->won_coins = 0;
    player->game_over_state = PLAYER_LOSS;
    app->state = GAME_OVER;
    com_send_msg((proto_msg_type_t) app->game.main_player.game_over_state, app->game.main_player.bet);
  }

  if (player->cards_value == 21) {
    add_dealer_single_animation(app);
  }

  vg_set_redraw();
}

void add_hit_animation(app_t *app) {
  sprite_t *rotate_1 = sprite_create((xpm_map_t) rotate1_xpm);
  sprite_t *rotate_2 = sprite_create((xpm_map_t) rotate2_xpm);
  sprite_t *card_back = app->game.card_back;
  animation_t *move_card = animation_create(8, handle_hit);

  size_t card_pos = app->game.main_player.cards->curr_size;

  uint32_t x = 500 + card_pos * card_back->img.width * 0.5;
  uint32_t y = 500 - card_back->img.height * 0.09;

  for (int i = 0; i < 4; i++) {
    animation_add_frame(move_card, rotate_1, x, y, 0);
  }

  for (int i = 0; i < 4; i++) {
    animation_add_frame(move_card, rotate_2, x, y, 0);
  }

  app->game.curr_anim = move_card;
}

void handle_double(void *ptr) {
  if (ptr == NULL)
    return;

  app_t *app = (app_t *) ptr;

  player_t *player = &app->game.main_player;

	player->coins -= player->bet;
  player->bet *= 2;

  game_give_card(app->game.cards, player->cards);

  card_t *card = queue_at(player->cards, player->cards->curr_size - 1);
  if (card != NULL)
    card->is_double = true;

  player->cards_value = game_get_cards_value(player->cards);

  if (player->cards_value > 21) {
    player->won_coins = 0;
    player->game_over_state = PLAYER_LOSS;
    app->state = GAME_OVER;
    com_send_msg((proto_msg_type_t) app->game.main_player.game_over_state, app->game.main_player.bet);
  }
  else {
    add_dealer_single_animation(app);
  }

  vg_set_redraw();
}

void add_double_animation(app_t *app) {
  sprite_t *rotate_1 = sprite_create((xpm_map_t) rotate1_xpm);
  sprite_t *rotate_2 = sprite_create((xpm_map_t) rotate2_xpm);
  sprite_t *card_back = app->game.card_back;
  animation_t *move_card = animation_create(8, handle_double);

  uint32_t x = 505;
  uint32_t y = 500 - card_back->img.height * 0.09 + 30;

  for (int i = 0; i < 4; i++) {
    animation_add_frame(move_card, rotate_1, x, y, 1);
  }

  for (int i = 0; i < 4; i++) {
    animation_add_frame(move_card, rotate_2, x, y, 1);
  }

  app->game.curr_anim = move_card;
}

void handle_game_playing(app_t *app, interrupt_type_t interrupt) {
  if (animation_running(app->game.curr_anim))
    return;

  if (interrupt == KEYBOARD) {
    switch (scancode) {
      case KB_ESC:
        app->state = MAIN_MENU;
        vg_set_redraw();
        break;
      // Hit
      case KB_1:
      case KB_ENTER:
        add_hit_animation(app);
        break;
      // Stand
      case KB_2:
        timer_counter = 0;
        add_dealer_single_animation(app);
        vg_set_redraw();
        break;
      // Double
      case KB_3:
        if (app->game.main_player.cards->curr_size != 2) {
          banner_set_message(&app->banner, "You can only do this in the 1st round", 60);
          break;
        }

        if (app->game.main_player.coins < app->game.main_player.bet) {
          banner_set_message(&app->banner, "Insufficient balance to double", 60);
          break;
        }

        add_double_animation(app);
        break;
      // Surrender
      case KB_4:
        if (app->game.main_player.cards->curr_size != 2) {
          banner_set_message(&app->banner, "You can only do this in the 1st round", 60);
          break;
        }
        app->game.main_player.won_coins = app->game.main_player.bet / 2;
        app->game.main_player.coins += app->game.main_player.won_coins;
        app->game.main_player.game_over_state = PLAYER_SURRENDER;
        app->state = GAME_OVER;
        com_send_msg((proto_msg_type_t) app->game.main_player.game_over_state, app->game.main_player.won_coins);
        break;
      default:
        break;
    }
  }

  if (interrupt == MOUSE) {
    // Hit
    if (cursor_sprite_colides(&app->cursor, queue_at(app->buttons_game_playing, 0))) {
      add_hit_animation(app);
    }

    // Stand
    if (cursor_sprite_colides(&app->cursor, queue_at(app->buttons_game_playing, 1))) {
      timer_counter = 0;
      add_dealer_single_animation(app);
      vg_set_redraw();
      return;
    }

    // Double
    if (cursor_sprite_colides(&app->cursor, queue_at(app->buttons_game_playing, 2))) {
      if (app->game.main_player.cards->curr_size != 2) {
        banner_set_message(&app->banner, "You can only do this in the 1st round", 60);
        return;
      }

      if (app->game.main_player.coins < app->game.main_player.bet) {
        banner_set_message(&app->banner, "Insufficient balance to double", 60);
        return;
      }

      add_double_animation(app);
      return;
    }

    // Surrender
    if (cursor_sprite_colides(&app->cursor, queue_at(app->buttons_game_playing, 3))) {
      if (app->game.main_player.cards->curr_size != 2) {
        banner_set_message(&app->banner, "You can only do this in the 1st round", 60);
        return;
      }

      app->game.main_player.won_coins = app->game.main_player.bet / 2;
      app->game.main_player.coins += app->game.main_player.won_coins;
      app->game.main_player.game_over_state = PLAYER_SURRENDER;
      app->state = GAME_OVER;
      com_send_msg((proto_msg_type_t) app->game.main_player.game_over_state, app->game.main_player.won_coins);
    }
  }
}
