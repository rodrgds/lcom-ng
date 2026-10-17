#ifndef __DRAW_H
#define __DRAW_H
#include "../devices/graphics/vbe.h"
#include "../visuals/model.h"
#include "../visuals/sprite.h"
#include "visuals/xpms/background.xpm"
#include "visuals/xpms/gameover.xpm"
#include "visuals/xpms/leaderboard.xpm"
#include "visuals/xpms/menu.xpm"
#include "visuals/xpms/winscreen.xpm"
#include <lcom/lcf.h>

/**
 * @brief Draws a sprite on the screen.
 *
 * @param sprite The sprite to be drawn.
 * @return 0 on success, non-zero otherwise.
 */
int draw_sprite(Sprite *sprite);

/**
 * @brief Draws a sprite at a specific position on the screen.
 *
 * @param sprite The sprite to be drawn.
 * @param x The x-coordinate of the position.
 * @param y The y-coordinate of the position.
 * @return 0 on success, non-zero otherwise.
 */
int draw_sprite_pos(Sprite *sprite, int x, int y);

/**
 * @brief Prints the background game using an XPM map.
 *
 * @param xpm The XPM map representing the background game.
 * @return 0 on success, non-zero otherwise.
 */
int print_background_game(xpm_map_t xpm);

/**
 * @brief Draws a frame on the screen.
 */
void draw_frame();

/**
 * @brief Draws digits on the screen.
 *
 * @param number The number to be drawn.
 * @param x_pos The x-coordinate of the position.
 * @param y_pos The y-coordinate of the position.
 * @param offset The offset between digits.
 * @return 0 on success, non-zero otherwise.
 */
int draw_digits(int number, int x_pos, int y_pos, int offset);

/**
 * @brief Draws the leaderboard on the screen.
 */
void draw_leaderboard();
#endif
