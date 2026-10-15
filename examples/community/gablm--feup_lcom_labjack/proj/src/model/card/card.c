#include "card.h"

static uint32_t loaded_card_ammount = 0;

int card_base_init() {
  const xpm_map_t card_xpm[CARD_TYPE_NUM][CARD_VALUE_NUM] = {
    {
			clubs1_xpm, clubs2_xpm, clubs3_xpm, clubs4_xpm, clubs5_xpm, clubs6_xpm, clubs7_xpm, clubs8_xpm, clubs9_xpm, clubs10_xpm, clubs11_xpm, clubs12_xpm, clubs13_xpm
		},
    {
			diamond1_xpm, diamond2_xpm, diamond3_xpm, diamond4_xpm, diamond5_xpm, diamond6_xpm, diamond7_xpm, diamond8_xpm, diamond9_xpm, diamond10_xpm, diamond11_xpm, diamond12_xpm, diamond13_xpm
		},
    {
			hearts1_xpm, hearts2_xpm, hearts3_xpm, hearts4_xpm, hearts5_xpm, hearts6_xpm, hearts7_xpm, hearts8_xpm, hearts9_xpm, hearts10_xpm, hearts11_xpm, hearts12_xpm, hearts13_xpm
		},
    {
			spades1_xpm, spades2_xpm, spades3_xpm, spades4_xpm, spades5_xpm, spades6_xpm, spades7_xpm, spades8_xpm, spades9_xpm, spades10_xpm, spades11_xpm, spades12_xpm, spades13_xpm
		}
	};

  loaded_card_ammount = 0;
  for (int type = 0; type < CARD_TYPE_NUM; type++) {
    for (int value = 0; value < CARD_VALUE_NUM; value++) {
      sprite_t *sprite = sprite_create(card_xpm[type][value]);
      if (sprite == NULL)
        return 1;
      card_sprites[type][value] = sprite;
      loaded_card_ammount += 1;
    }
  }

  return 0;
}

void card_base_destroy() {
  uint32_t count = 0;
  for (int type = 0; type < CARD_TYPE_NUM; type++) {
    for (int value = 0; value < CARD_VALUE_NUM; value++) {
      if (count == loaded_card_ammount)
        return;
      sprite_destroy(card_sprites[type][value]);
      count++;
    }
  }
}

void card_queue_destroy(void *card) {
  card_destroy((card_t *) card);
}

void card_destroy(card_t *card) {
  if (card == NULL)
    return;

  free(card);
}

card_t *card_create(int value, card_type_t type) {
  card_t *card = (card_t *) malloc(sizeof(card_t));

  if (value < 0 || value >= 13)
    return NULL;

  if (value == 0)
    card->value = 11;
  else if (value >= 10)
    card->value = 10;
  else
    card->value = value + 1;

  card->type = type;
  card->is_double = 0;
  card->sprite_base = card_sprites[type][value];

  return card;
}
