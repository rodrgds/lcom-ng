/**
 * @file game.c
 * @brief Implements core game logic, state management, and drawing of game-specific elements.
 * This includes handling game play states (paused, normal, fast) and loading/drawing game assets.
 */

#include "xpms/selected1.xpm" 
#include "xpms/selected2.xpm"
#include "xpms/selected3.xpm"


#include <lcom/lcf.h>
#include <lcom/xpm.h>

#include "game.h"
#include "controllers/video/graphics.h"

uint8_t *bg_game, *sel2, *sel3;
xpm_image_t game_bg, selected2, selected3;

PlayState play_state = PLAY_NORMAL;

/**
 * @brief Sets the current play state of the game.
 * Also prints a message to the console indicating the new state.
 * @param new_state The new PlayState to set.
 */
void set_play_state(PlayState new_state) {
    play_state = new_state;

    switch (new_state) {
        case PLAY_PAUSED:
            printf("Game paused.\n");
            break;
        case PLAY_NORMAL:
            printf("Game resumed at normal speed.\n");
            break;
        case PLAY_FAST:
            printf("Game set to fast speed.\n");
            break;
    }
}

/**
 * @brief Loads game-specific assets, primarily XPM images for backgrounds.
 * Prints an error message if loading fails.
 */
void load_game_assets() {
    bg_game = xpm_load(selected1_xpm, XPM_5_6_5, &game_bg);
    sel2 = xpm_load(selected2_xpm, XPM_5_6_5, &selected2);
    sel3 = xpm_load(selected3_xpm, XPM_5_6_5, &selected3);
    if (!bg_game)
        printf("Failed to load game background XPM!\n");
}

/**
 * @brief Draws the main game background.
 * Scales the background to fit the screen resolution.
 */
void draw_game() {
    vg_draw_scaled_pixmap(bg_game, game_bg.width, game_bg.height, (int)xpm_source_bpp,
                      0, 0, mode_info.XResolution, mode_info.YResolution);
}

/**
 * @brief Draws an alternate game background/UI element ("selected2").
 * Scales the image to fit the screen resolution.
 */
void draw_characters2(){
    vg_draw_scaled_pixmap(sel2, selected2.width, selected2.height, 2, 0, 0, mode_info.XResolution, mode_info.YResolution);
}

/**
 * @brief Draws another alternate game background/UI element ("selected3").
 * Scales the image to fit the screen resolution.
 */
void draw_characters3(){
    vg_draw_scaled_pixmap(sel3, selected3.width, selected3.height, 2, 0, 0, mode_info.XResolution, mode_info.YResolution);
}

