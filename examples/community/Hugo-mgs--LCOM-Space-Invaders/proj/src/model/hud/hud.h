#pragma once

#include <lcom/lcf.h>
#include "../../sprites/sprite.h"
#include "../player/player.h"

/**
 * Draws the HUD for the player, including its lives and score.
 *
 * @param player Pointer to the Player structure whose HUD information will be displayed.
 */
void drawHud(Player *player);
 
/**
 * Draws the current wave number on the screen.
 *
 * @param wave The current wave number to display.
 */
void drawWave(int wave);
 
/**
 * @brief Draws a number on the screen at the specified coordinates.
 *
 * @param number The number to be drawn.
 * @param x The x-coordinate where the number will be drawn.
 * @param y The y-coordinate where the number will be drawn.
 */
void draw_number(int number, int x, int y);
