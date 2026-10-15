#include "card.h"

Card *(create_card)(unsigned int id, Sprite *card_sprite) {
    if (card_sprite == NULL) {
        return NULL;
    }

    Card *card = (Card *) malloc (sizeof(Card));

    if (card == NULL) {
        return NULL;
    }

    card->sprite = card_sprite;

    card->id = id;
    card->is_selected = false;
    card->is_clicked = false;
    card->is_turned = false;

    return card;
}

void (destroy_card)(Card *card) {
    if (card == NULL) {
        return;
    }

    card->sprite = NULL;

    free(card);
    card = NULL;
}
