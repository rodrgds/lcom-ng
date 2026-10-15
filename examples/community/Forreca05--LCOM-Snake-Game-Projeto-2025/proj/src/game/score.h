/**
 * @file score.h
 * @brief Defines the Score struct and related functions for handling the player's score.
 *
 * This header provides the definition of the Score struct and function prototypes
 * for creating, drawing, and destroying the score in the game.
 */
#ifndef SCORE_H
#define SCORE_H

#include <stdlib.h>
#include "../controllers/gpu/sprites/text.h"
#include "UTILS.h"

#define MAX_DIGIT_COUNT 8

/**
 * @struct Score
 * @brief Represents the player's score in the game.
 */
typedef struct Score Score;

struct Score {
    Text* scoreSprite;           /**< Pointer to the text sprite displaying the score */
    int score;                   /**< Current score value */

    /**
     * @brief Draws the score on the screen.
     * @param self Pointer to the Score.
     */
    void (*draw)(Score* self);

    /**
     * @brief Destroys the score and frees its resources.
     * @param self Pointer to the Score to destroy.
     */
    void (*destroy)(Score* self);
};
/**
 * @brief Creates a new Score with the given initial value.
 * @param score Initial score value.
 * @param x x position to put the Score.
 * @param y y position to put the Score.
 * @return Pointer to the created Score.
 */
Score* createScore(int score,int x,int y);


#endif
