/**
 * @file game.h
 * @brief Defines game states, UI button coordinates for game controls, and function prototypes for game logic.
 */

#ifndef GAME_H
#define GAME_H

#include <lcom/lcf.h>
#include <lcom/xpm.h>

#include "controllers/video/graphics.h"


// Pause button coordinates
#define PAUSE_BTN_X1   805
#define PAUSE_BTN_X2   920
#define PAUSE_BTN_Y1   910
#define PAUSE_BTN_Y2   975

// Resume button coordinates
#define FAST_BTN_X1  1085
#define FAST_BTN_X2  1200
#define FAST_BTN_Y1  910
#define FAST_BTN_Y2  975

// Fast button coordinates
#define RESUME_BTN_X1    945
#define RESUME_BTN_X2    1055
#define RESUME_BTN_Y1    910
#define RESUME_BTN_Y2    975

typedef enum {
    PLAY_PAUSED,
    PLAY_NORMAL,
    PLAY_FAST
} PlayState;

extern PlayState play_state;

/**
 * @brief Sets the current play state of the game.
 * @param new_state The new PlayState to set.
 */
void set_play_state(PlayState new_state);

/**
 * @brief Loads assets required for the game screen (e.g., backgrounds).
 */
void load_game_assets(void);

/**
 * @brief Draws the main game screen elements (e.g., background).
 */
void draw_game(void);

/**
 * @brief Draws an alternate game background or UI element (referred to as "characters2").
 * @note The function name might be misleading as it likely draws a background/UI, not characters.
 */
void draw_characters2(void);

/**
 * @brief Draws another alternate game background or UI element (referred to as "characters3").
 * @note The function name might be misleading as it likely draws a background/UI, not characters.
 */
void draw_characters3(void);

#endif // GAME_H
