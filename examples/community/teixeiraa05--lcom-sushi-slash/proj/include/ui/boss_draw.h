#ifndef __BOSS_DRAW_H
#define __BOSS_DRAW_H

#include <stdbool.h>

#include "../game/boss.h"
#include "../init.h"

/**
 * @file boss_draw.h
 * @brief Declares boss encounter and boss reward popup drawing helpers.
 */

/** @brief Color used for boss timer and progress text. */
#define BOSS_UI_TEXT_COLOR 0xFFFFFF

/** @brief Color used for the boss reward popup. */
#define BOSS_REWARD_POPUP_COLOR 0x33CC66

/**
 * @brief Draw the active boss wave and current key prompt.
 *
 * @param boss Active boss to draw.
 * @param active Whether boss contains an active encounter.
 * @param assets Shared assets used for drawing.
 * @param current_tick Current gameplay/session tick.
 * @return 0 on success, non-zero on failure.
 */
int ui_boss_draw(const boss_t *boss, bool active, const Assets *assets, uint32_t current_tick);

/**
 * @brief Draw the boss reward popup when visible.
 *
 * @param points Reward points to display.
 * @param ticks_left Remaining popup ticks.
 * @param assets Shared assets containing the game font.
 */
void ui_boss_reward_popup_draw(uint32_t points, uint8_t ticks_left, const Assets *assets);

#endif /* __BOSS_DRAW_H */
