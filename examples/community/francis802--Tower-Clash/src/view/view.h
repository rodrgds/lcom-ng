/**
 * @file view.h
 *
 * @brief File containing the functions to draw the game menus, sprites. Also
 * contains the functions to swap and configure the frame buffers. 
*/

#ifndef _LCOM_MENU_H_
#define _LCOM_MENU_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include <math.h>
#include "config.h"
#include "controller/video/graphics.h"
#include "controller/mouse/mouse.h"
#include "controller/rtc/rtc.h"
#include "model/sprite.h"
#include "model/model.h"

/**
 * @brief Struct that represents a velocity vector for a sprite
 * 
*/
struct velocity_vector
{
    int vx;
    int vy;
};

/**
 * @brief Struct that represents a sprite position
 *
 */
struct sprite_pos
{
    int x;
    int y;
};

/**
 * @brief This function calls one of the other draw functions, depending on the current
 * game state.
 */
void draw_new_frame();

/**
 * @brief This function draws the initial menu, which is the first menu to be displayed
 * when the game starts.
 */
void draw_initial_menu();

/**
 * @brief This function draws the game, in which all other components like the towers,
 * the background and the projectiles are drawn.
 */
void draw_game();

/**
 * @brief This function draws the finish menu, which is the menu displayed when the
 * game is over.
 */
void draw_finish_menu();

/**
 * @brief This function draws the pause menu, which is the menu displayed when the
 * game is paused.
 */
void draw_pause_menu();

/**
 * @brief This function draws the mouse, which is a sprite that follows the mouse
 * position. It changes the mouse sprite depending on the current game state and
 * the mouse position on the screen.
 */
void draw_mouse();

/**
 * @brief This function draws the help page, which contains the game instructions.
 */
void draw_help_page();

/**
 * @brief This function swaps the main frame buffer with the secondary frame buffer,
 * when the double buffer mode is enabled.
 */
void swap_buffers();

/**
 * @brief This function prints to the console the current time, in the format YYYY/MM/DD HH:MM:SS.
 * It uses the RTC to get the current time.
*/
void display_real_time();

/**
 * @brief This function draws a sprite on the screen, at the given position.
 * 
 * @param sprite Sprite to be drawn
 * @param x top left X coordinate of the sprite
 * @param y top left Y coordinate of the sprite
 * @return int 0 if no errors occurred, 1 otherwise
*/
int draw_sprite_xpm(Sprite *sprite, int x, int y);

/**
 * @brief This function allocates the neccessary memory and configures the frame buffers. It can
 * create one or two frame buffers, depending on the mode selected. It also
 * configures the sky color to be drawn, since this function is called only once
 * at the start of the game, so the sky color is only set once.
 * 
 * @param mode Mode to be set
 * 
 * @return int 0 if no errors occurred, 1 otherwise
*/
int set_frame_buffers(uint16_t mode);

/**
 * @brief This function sets the cannonball initial position, which is the position
 * where the cannonball is fired from. It depends on the current cannon position
 * and the tower that is shooting.
*/
void set_cannonball_initial_position(int value);


/**
 * @brief This function updates the cannonball position, depending on the current
 * cannonball position, the cannonball velocity vector and the elapsed time
 * since the cannonball was fired.
*/
void update_cannonball_position();

/**
 * @brief This function detects colisions between the cannonball and the towers. It
 * checks if the cannonball is inside the tower hitbox.
 * 
 * @return true if a colision was detected, false otherwise
*/
bool collision_detected();


/** 
 * @brief Player shooting mechanics
 * 
 * Aux function to use when a player is about to shoot the cannonball.
*/
void player_mechanics();

/** 
 * @brief Bot shooting mechanics
 * 
 * Aux function to use when the bot is about to shoot the cannonball.
*/
void bot_mechanics();

/** Checks if game is over
 * 
 * Aux function that checks if the game is over and who won.    
*/
void endOfGame();

/** 
 * @brief Set up game variables
 * 
 * This function sets up the game variables, variables that will be used during the game.
*/
void setGameVariables();

/** 
 * @brief Draws the health bars
 * 
 * This function draws the health bars of the towers.
*/
void draw_health_bars();



#endif
