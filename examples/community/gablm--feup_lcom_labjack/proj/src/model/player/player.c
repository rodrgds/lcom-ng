#include "player.h"

int player_init(player_t *player) {
  if (player == NULL)
    return 1;

  player->coins = PLAYER_STARTING_COINS;
  player->bet = 0;
  player->cards_value = 0;
  player->game_over_state = NOT_ENDED;
  player->won_coins = 0;
  player->cards = queue_create(PLAYER_MAX_DECK_SIZE);

  return player->cards == NULL;
}

void player_destroy(player_t *player) {
  if (player == NULL)
    return;
  if (player->cards == NULL)
    return;

  queue_destroy(&player->cards, card_queue_destroy);
}

void player_draw(player_t *player) {
  if (player == NULL)
    return;

  for (size_t i = 0; i < player->cards->curr_size; i++) {
    card_t *card = queue_at(player->cards, i);
    if (card == NULL)
      continue;

    uint32_t x = 500 + i * card->sprite_base->img.width * 0.5;
    uint32_t y = 500 - card->sprite_base->img.height * 0.09;

    if (card->is_double) {
      sprite_move(card->sprite_base, 505, y + 30);
      sprite_draw_rotate(card->sprite_base);
    }
    else {
      sprite_move(card->sprite_base, x, y);
      sprite_draw(card->sprite_base);
    }
  }
}
