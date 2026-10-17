/**
 * @file hud.h
 * @brief Heads-Up Display (HUD) drawing functions for the game.
 */

#ifndef HUD_H
#define HUD_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "../graphics/graphics.h"
#include "../graphics/font8x8.h"
#include "../game_core/types.h"

/**
 * @brief Draws a single character at the specified position with the given color.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param c Character to draw.
 * @param color Color index.
 */
void draw_char(uint16_t x, uint16_t y, char c, uint8_t color);

/**
 * @brief Draws a string of text at the specified position with the given color.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param text Null-terminated string to draw.
 * @param color Color index.
 */
void draw_text(uint16_t x, uint16_t y, const char *text, uint8_t color);

/**
 * @brief Draws a number at the specified position with the given color.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param number Number to draw.
 * @param color Color index.
 */
void draw_number(uint16_t x, uint16_t y, int number, uint8_t color);

/**
 * @brief Draws a health bar at the specified position.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param width Width of the health bar.
 * @param height Height of the health bar.
 * @param hp Health value (0-100).
 */
void draw_hp_bar(uint16_t x, uint16_t y, int width, int height, int hp);

/**
 * @brief Draws the entire HUD (score, health, bullets, etc.).
 */
void draw_hud(void);

#endif
