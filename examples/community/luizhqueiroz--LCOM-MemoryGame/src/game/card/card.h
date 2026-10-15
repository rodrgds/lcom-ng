/**
 * @file card
 * @brief Functions for the game cards
 */

#ifndef _LCOM_CARD_H_
#define _LCOM_CARD_H_

/** @defgroup Card Card
 * @{
 *
 * Functions for the game cards
 */

#include "../../images/sprite/sprite.h"
#include <stdlib.h>

typedef struct {
    Sprite *sprite;
    bool is_selected;
    bool is_clicked;
    bool is_turned;
    unsigned int id;
} Card;

/**
 * @brief Creates a card in memory for the game
 *
 * @param id The card id
 * @param card_sprite The sprite of the card
 * @return Return a pointer to the created card
 */
Card *(create_card)(unsigned int id, Sprite *card_sprite);

/**
 * @brief Destroy a card freeing its memory
 *
 * @param card A pointer to a card that need to be destroyed
 */
void (destroy_card)(Card *card);

/**@}*/

#endif
