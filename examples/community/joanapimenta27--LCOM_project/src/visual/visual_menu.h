/**
 * @file visual_menu.h
 * @brief Header file for the visual menu module.
 */
#ifndef _VISUAL_MENU_H
#define _VISUAL_MENU_H

#include <lcom/lcf.h>
#include "visual.h"
#include "../game/game_play.h"
#include "../game/scores.h"

/**
 * Importing sprite pointers representing different menu elements.
 */
sprite_t *menu_exit;
sprite_t *menu_title;
sprite_t *menu_background;
sprite_t *menu_levels;
sprite_t *menu_level_1;
sprite_t *menu_level_2;
sprite_t *menu_level_3;
sprite_t *menu_back;
sprite_t *player;
sprite_t *menu_scoreboard;
sprite_t *frame;
sprite_t *slide_container;
sprite_t *slide;

/**
 * Draws the menu header.
 * @param background_color_array The color array for the background.
 * @return 0 upon success, non-zero otherwise.
 */
int(menu_draw_header)(uint32_t *background_color_array);

/**
 * Draws the menu options.
 * @param selected The index of the selected option.
 * @param game_color_array The color array for the game.
 * @return 0 upon success, non-zero otherwise.
 */
int(menu_draw_options)(int selected, uint32_t *game_color_array);

/**
 * Undraws the menu header.
 * @param game_color_array The color array for the game.
 * @return 0 upon success, non-zero otherwise.
 */
int(menu_undraw_header)(uint32_t *game_color_array);

/**
 * Undraws the menu options.
 * @param game_color_array The color array for the game.
 * @return 0 upon success, non-zero otherwise.
 */
int(menu_undraw_options)(uint32_t *game_color_array);

/**
 * Undraws the pre-level options.
 * @param color_array The color array for the game.
 * @return 0 upon success, non-zero otherwise.
 */
int (pre_level_undraw_options)(uint32_t *color_array);

/**
 * Draws the pre-level options.
 * @param selected The index of the selected option.
 * @param game_color_array The color array for the game.
 * @return 0 upon success, non-zero otherwise.
 */
int (pre_level_draw)(uint8_t selected, uint32_t *game_color_array);

/**
 * Draws level 1.
 * @param background_color_array The color array for the background.
 * @param game_color_array The color array for the game.
 * @return 0 upon success, non-zero otherwise.
 */
int(level_1_draw)(uint32_t *background_color_array, uint32_t *game_color_array);

/**
 * Draws level 2.
 * @param background_color_array The color array for the background.
 * @param game_color_array The color array for the game.
 * @return 0 upon success, non-zero otherwise.
 */
int(level_2_draw)(uint32_t *background_color_array, uint32_t *game_color_array);

/**
 * Draws level 3.
 * @param background_color_array The color array for the background.
 * @param game_color_array The color array for the game.
 * @return 0 upon success, non-zero otherwise.
 */
int(level_3_draw)(uint32_t *background_color_array, uint32_t *game_color_array);

/**
 * Draws the pause menu options.
 * @param selected The index of the selected option.
 * @param game_color_array The color array for the game.
 * @return 0 upon success, non-zero otherwise.
 */
int (draw_pause_menu_options)(int selected, uint32_t *game_color_array);

/**
 * Draws the win menu options.
 * @param selected The index of the selected option.
 * @param game_color_array The color array for the game.
 * @return 0 upon success, non-zero otherwise.
 */
int (draw_win_menu_options)(int selected, uint32_t *game_color_array);

/**
 * Undraws the pause menu options.
 * @param color_array The color array for the game.
 * @return 0 upon success, non-zero otherwise.
 */
int (pause_menu_undraw_options)(uint32_t *color_array);

/**
 * Draws the dead options.
 * @param selected The index of the selected option.
 * @param game_color_array The color array for the game.
 * @return 0 upon success, non-zero otherwise.
 */
int (draw_dead_options)(int selected, uint32_t *game_color_array);

/**
 * Undraws the dead options.
 * @param game_color_array The color array for the game.
 * @return 0 upon success, non-zero otherwise.
 */
int(undraw_dead_options)(uint32_t *game_color_array);

/**
 * Draws the scoreboard.
 * @param selected The index of the selected option.
 * @param color_array The color array for the game.
 * @return 0 upon success, non-zero otherwise.
 */
int (scoreboard_draw)(int selected, uint32_t *color_array);

/**
 * Undraws the scoreboard.
 * @param game_color_array The color array for the game.
 * @return 0 upon success, non-zero otherwise.
 */
int(undraw_scoreboard)(uint32_t *game_color_array);

/**
 * Draws the scoreboard scroller.
 * @param game_color_array The color array for the game.
 * @param scroller The index of the scroller.
 */
void draw_scoreboard_scroller(uint32_t *game_color_array, int scroller);

#endif
