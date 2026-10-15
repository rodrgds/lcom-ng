#ifndef SCORE_H
#define SCORE_H

#include <lcom/lcf.h>
#include <stddef.h>
#include <stdlib.h>
#include "sprite.h"
#include "spriteLoader.h"

/**
 * @file score.h
 * @brief Score management in the game
 * 
 * This module provides functions to create, manage, and draw the score in the game.
 * It handles the current score, increments it, and draws the score on the screen.
 */

struct score_imp;
typedef struct score_imp Score;

/**
 * @brief Creates a new Score object
 * 
 * @param x The x-coordinate of the score
 * @param y The y-coordinate of the score
 * @return Pointer to the created Score object, or NULL if the creation failed
 */
Score *create_score(SpriteLoader *loader);

/**
 * @brief Destroys a Score object, freeing all allocated memory
 * 
 * @param score Pointer to the Score object to be destroyed
 */
void destroy_score(Score *score);

/**
 * @brief Increments the score by 1
 * 
 * The score is incremented every 30 frames.
 * 
 * @param score Pointer to the Score object
 */
void increment_score(Score *score);

/**
 * @brief Gets the current score value
 * 
 * @return The current score value, or 0 if the score is NULL
 */
uint16_t get_score_value(Score *score);

void draw_score(Score *score, int x, int y);


#endif // SCORE_H

