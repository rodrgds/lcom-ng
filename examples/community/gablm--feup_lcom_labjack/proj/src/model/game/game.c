#include "game.h"

int game_add_deck(queue_t *cards, bool shuffle) {
  for (int type = 0; type < CARD_TYPE_NUM; type++) {
    for (int value = 0; value < CARD_VALUE_NUM; value++) {
      card_t *card = card_create(value, type);
      if (card == NULL)
        return 1;

      if (queue_push(cards, card)) {
        free(card);
        return 1;
      }
    }
  }

  if (shuffle)
    queue_shuffle(cards);

  return 0;
}

int game_init(game_t *game) {
  if (game == NULL)
    return 1;
  if (card_base_init()) {
    game_destroy(game);
    return 1;
  }

  game->input_select = 0;
  game->dealer_turn = 0;
  game->dealer_ignore_last = 0;
  game->curr_anim = NULL;

  game->card_back = sprite_create((xpm_map_t) back_red_1_xpm);
  if (game->card_back == NULL)
    return 1;

  game->cards = queue_create(GAME_DECK_AMOUNT * GAME_DECK_SIZE);
  if (game->cards == NULL) {
    game_destroy(game);
    return 1;
  }

  game->dealer = queue_create(GAME_DECK_AMOUNT * GAME_DECK_SIZE);
  if (game->cards == NULL) {
    game_destroy(game);
    return 1;
  }

  for (int i = 0; i < GAME_DECK_AMOUNT; i++) {
    game_add_deck(game->cards, true);
  }

  if (player_init(&game->main_player) || player_init(&game->other_player)) {
    game_destroy(game);
    return 1;
  }

  return 0;
}

void game_destroy(game_t *game) {
  if (game == NULL)
    return;

  player_destroy(&game->main_player);
  player_destroy(&game->other_player);

  if (game->cards != NULL)
    queue_destroy(&game->cards, card_queue_destroy);
  if (game->dealer != NULL)
    queue_destroy(&game->dealer, card_queue_destroy);

  card_base_destroy();
}

int game_give_card(queue_t *deck, queue_t *receiver) {
  if (deck == NULL || deck->curr_size == 0)
    return 1;
  if (receiver == NULL)
    return 1;

  card_t *card = queue_pop(deck);
  if (card == NULL)
    return 1;

  return queue_push(receiver, card);
}

int game_draw_deck(game_t *game) {
  if (game == NULL)
    return 1;

  for (size_t i = 0; i < MIN(game->cards->curr_size, GAME_DECK_DRAW_AMOUNT); i++) {
    uint32_t x = GAME_DECK_DRAW_X + (i + 1) * game->card_back->img.width * 0.02;
    uint32_t y = GAME_DECK_DRAW_Y - (i + 1) * game->card_back->img.height * 0.1;
    sprite_move(game->card_back, x, y);
    if (sprite_draw(game->card_back))
      return 1;
  }

  return 0;
}

int game_draw_dealer(game_t *game, font_t *font) {
  uint32_t card_value = 0;

  for (size_t i = 0; i < queue_size(game->dealer); i++) {
    card_t *card = queue_at(game->dealer, i);

    if (card == NULL)
      return 1;

    sprite_t *sprite;

    if (i == 0)
      card_value = card->value;

    if (i > 0 && game->dealer_ignore_last)
      break;

    sprite = (i == 0 || game->dealer_turn) ? card->sprite_base : game->card_back;

    sprite_move(sprite, 500 + i * sprite->img.width * 0.5, 240);
    sprite_draw(sprite);
  }

  font_print_number(font, game->dealer_turn ? game_get_cards_value(game->dealer) : card_value, 505, 315, 0xffffff);

  return 0;
}

uint32_t game_get_cards_value(queue_t *cards) {
  if (cards == NULL)
    return 0;

  uint32_t cards_value = 0;
  uint32_t aces = 0;

  for (size_t i = 0; i < cards->curr_size; i++) {
    card_t *card = queue_at(cards, i);
    if (card == NULL)
      continue;

    cards_value += card->value;
    if (card->value == 11)
      aces++;
  }

  while (cards_value > 21 && aces > 0) {
    cards_value -= 10;
    aces--;
  }

  return cards_value;
}
