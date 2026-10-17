#ifndef CARD_H
#define CARD_H

#include "../../sprites/sprites.h"

/**
 * @brief Structure representing an animation for a card.
 */
typedef struct {
  int frame;      /**< Current frame index of the animation. */
  int start;      /**< Starting frame index. */
  int end;         /**< Ending frame index. */
  int step;        /**< Step direction (1 for forward, -1 for backward). */
  int last_tick;   /**< Last update tick (for timing control). */
  bool active;      /**< Whether the animation is currently active. */
  Fruta fruta;      /**< The fruit type associated with this animation. */
  Sprite** frames;  /**< Array of sprite frames for the animation. */
} Animation;

/**
 * @brief Structure representing a card in the game.
 */
typedef struct {
    Sprite* sprite; /**< Pointer to the current sprite of the card. */
    bool isFaceUp;  /**< True if the card is face-up. */
    Fruta fruit;    /**< Type of fruit displayed on the card. */
    int xPos, yPos;  /**< Coordinates where the card is drawn. */
    bool isAnimating; /**< True if the card is currently animating. */
    bool isMatched;   /**< True if the card has been matched. */
    Animation anim;   /**< Animation data associated with the card. */
} Card;

/**
 * @brief Initializes a card with the specified fruit and position.
 * 
 * @param card Pointer to the Card to initialize.
 * @param fruit The fruit type to assign to the card.
 * @param x X-coordinate for drawing the card.
 * @param y Y-coordinate for drawing the card.
 */
void card_init(Card* card, Fruta fruit, int x, int y);

/**
 * @brief Draws the card on the screen at its current position.
 * 
 * @param card Pointer to the Card to draw.
 */
void card_draw(const Card* card);

/**
 * @brief Flips the card, changing its face-up state.
 * 
 * @param card Pointer to the Card to flip.
 */
void card_turn(Card* card);

/**
 * @brief Checks if the card is currently face-up.
 * 
 * @param card Pointer to the Card to check.
 * @return true if the card is face-up, false otherwise.
 */
bool card_is_turned_over(const Card* card);

/**
 * @brief Updates the animation frame of the card.
 * 
 * Should be called regularly to animate the card when `isAnimating` is true.
 * 
 * @param card Pointer to the Card whose animation is being updated.
 */
void animation_update(Card* card);

/**
 * @brief Starts the flip animation for the card.
 * 
 * @param card Pointer to the Card to animate.
 * @param frame_count Total number of frames in the animation.
 * @param forward Direction of the animation (true for forward, false for reverse).
 */
void animation_start(Card* card, int frame_count, bool forward);

#endif
