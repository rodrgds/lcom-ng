/**
 * @file hud.h
 * @brief HUD rendering and management.
 */

#ifndef __UI_H
#define __UI_H

#include <stdint.h>
#include <stdbool.h>
#include "rtc.h"

/**
 * @brief Initializes HUD assets and state.
 */
void hud_init();

/**
 * @brief Renders the  HUD during gameplay.
 * * @param suns Current number of suns.
 * @param scr Current score.
 * @param lvl Current level.
 * @param selected_plant The ID of the currently selected plant.
 * @param is_frozen Indicates if the board is currently frozen.
 * @param is_shovel_mode Indicates if the shovel tool is currently active.
 * @param time Pointer to the current RTC date structure for the clock.
 */
void ui_render_hud(int suns, int scr, int lvl, int selected_plant, bool is_frozen, bool is_shovel_mode, rtc_date *time);

/**
 * @brief Renders the overlay displayed when a level is completed.
 * * @param suns_spent Total amount of suns spent during the level.
 */
void ui_render_level_complete(int suns_spent);

/**
 * @brief Renders the overlay displayed when losing in a level.
 * * @param suns_spent Total amount of suns spent during the run.
 */
void ui_render_game_over(int suns_spent);

#endif
