#ifndef __UI_H
#define __UI_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "i8042.h"
#include "keyboard.h"
#include "player.h"
#include "astro.h"
#include "shots.h"
#include "boost.h"
#include "cursor.h"
#include "points.h"
#include "rtc.h"

//sprite from https://hdwallpaperim.com/wp-content/uploads/2017/08/104598-space-pixel_art-horizon-stars-748x421.jpg
#include "xpm/background.xpm" 
#include "xpm/menu_background.xpm"

//sprite from https://www.reddit.com/r/PixelArt/comments/g948la/nebula_i_made_for_a_space_shooter_im_working_on/
#include "xpm/day_bg_image.xpm"


#include "xpm/game_over.xpm"
#include "xpm/game_over_new_hs.xpm"




/**
 * @defgroup UI UI
 * @{
 * 
 * Functions related to the visual aspects of the game
 */

/**
 * @brief Initializes the game sprites
 * 
 * @param astros Array of the astros
 * @param shots Array of the missile shots
 * @param shot_animations Array of the missile animations
 * @param player Player of the game
 * @param boost Boost of the game
 * @return Return 0 upon success and non-zero otherwise 
 */
int (initialize_game_sprites)(struct astro astros[], struct shot shots[], struct shot_animation shot_animations[], struct player *player,struct boost *boost);

/**
 * @brief Initializes the game background according to the real time minutes
 * 
 * @param imgBG XPM image of the game screen's background, containing information like the width and height
 * @return uint8_t* Background colors
 */
uint8_t* (initialize_game_background)(xpm_image_t *imgBG);

/**
 * @brief Initializes the menu's background
 * 
 * @param imgmenuBG XPM image of the menu's background, containing information like the width and height
 * @return uint8_t* Background colors
 */
uint8_t* (initialize_menu_background)(xpm_image_t *imgmenuBG);

/**
 * @brief Draws the entire game map with all the elements current state and position
 * 
 * @param astros Array of the astros
 * @param shots Array of the missile shots
 * @param shot_animations Array of the missile animations
 * @param player Player of the game
 * @param boost Boost of the game
 * @param state State that holds information about what keys are pressed at the moment
 * @param cursor Cursor struct
 * @param bg Background colors
 * @param points Player points
 * @return Return 0 upon success and non-zero otherwise 
 */
int (vg_draw_frame)(struct astro astros[], struct shot shots[], struct shot_animation shot_animations[], struct player *player,struct boost *boost, struct keyboard_state *state, struct cursor *cursor, uint8_t *bg, int *points);

/**
 * @brief Draws a spaceship to show the selected button in the menu
 * 
 * @param option Selected button index
 * @param replacementbg Menu background colors
 * @return Return 0 upon success and non-zero otherwise 
 */
int (menu_option_sprite)(int option, uint8_t *replacementbg);

/**
 * @brief Updates the struct state depending on the scan codes read from the keyboard
 * 
 * @param scanCode Scan code read from the keyboard output buffer
 * @param state Struct state that holds information about what keys of the kbd are pressed 
 */
void (state_scancode)(uint8_t scanCode, struct keyboard_state *state);

/**
 * @brief Draws the score and the highscore in the game over screen
 * 
 * @param points Points of the player
 * @param highscore Highest score ever achieved
 */
void (draw_scores)(int points, int highscore);

/**
 * @brief Checks if the mouse is placed on top of the boost and if the right button is being pressed, if so it catches the boost destroying it
 * and giving the player extra speed
 * 
 * @param boost Boost of the game
 * @param cursor Struct cursor
 * @param bg Background colors
 */
void (capture_boost)(struct boost *boost, struct cursor *cursor, uint8_t *bg);

/**
 * @brief Draws the astros and checks for collisions with the player and shots
 * 
 * @param astros Array of the astros
 * @param shots Array of the missile shots
 * @param shot_animations Array of the missile animations
 * @param player Player of the game
 * @param bg Background colors
 */
void (draw_astros)(struct astro astros[], struct shot shots[], struct shot_animation shot_animations[], struct player *player, uint8_t *bg);

/**
 * @brief Draw shots according to their current state
 * 
 * @param shots Array of the missile shots
 * @param shot_animations Array of the missile animations
 * @param player Player of the game
 * @param bg Background colors
 */
void (draw_shots)(struct shot shots[], struct shot_animation shot_animations[], struct player *player, uint8_t *bg);

/**
 * @}
 */

#endif /* __UI_H */
