#ifndef _LCOM_CARD_H_
#define _LCOM_CARD_H_

#include <lcom/lcf.h>
#include "../sprite/sprite.h"
#include "../../assets/cards/cards.h"
#include "../../data_structures/queue/queue.h"

#define CARD_TYPE_NUM 4
#define CARD_VALUE_NUM 13

/**
 * @defgroup card Card
 * @ingroup models
 * @{
 */

/**
 * @brief Enum for the different card types.
 */
typedef enum {
	CLUBS,    /**< Clubs suit */
	DIAMOND,  /**< Diamond suit */
	HEARTS,   /**< Hearts suit */
	SPADES,   /**< Spades suit */
} card_type_t;

/**
 * @brief Structure representing a card.
 */
typedef struct {
	int         value;         /**< The value of the card */
	int         is_double;     /**< Indicates if the card value is doubled */
	card_type_t type;          /**< The type/suit of the card */
	sprite_t    *sprite_base;  /**< Pointer to the sprite representing the card */
} card_t;

sprite_t *card_sprites[CARD_TYPE_NUM][CARD_VALUE_NUM];

/**
 * @brief Initializes the card base by loading card sprites.
 * @return 0 on success, 1 on failure.
 */
int			card_base_init();

/**
 * @brief Destroys all loaded card sprites.
 */
void		card_base_destroy();

/**
 * @brief Destroys a card in the queue.
 * @param card Pointer to the card to be destroyed.
 */
void		card_queue_destroy(void *card);

/**
 * @brief Destroys a card.
 * @param card Pointer to the card to be destroyed.
 */
void		card_destroy(card_t *card);

/**
 * @brief Creates a card with the specified value and type.
 * @param value The value of the card (0-12, corresponding to Ace-King).
 * @param type The type/suit of the card.
 * @return Pointer to the created card, or NULL on failure.
 */
card_t	*card_create(int value, card_type_t type);

/** @} */

#endif /* _LCOM_EV_CARD_H_ */
