#ifndef __GAME_H
#define __GAME_H

#include <lcom/lcf.h>
#include <stdint.h>

#include "drivers/timer/timer.h"
#include "drivers/kbc/kbc.h"
#include "drivers/kbc/keyboard.h"
#include "drivers/kbc/mouse.h"
#include "drivers/graphics/graphics.h"
#include "sprites/sprite.h"
#include "characters/character.h"


extern mouse_packet_t mouse_packet;
extern keyboard_packet_t keyboard_packet;
extern Sprite* background, * cursor, * logo, * play_text, * settings_text, * exit_text;
extern Sprite* play_texth, * settings_texth, * exit_texth;
extern Character* fireboy, * watergirl;
extern Sprite* ropes[10][10];


typedef enum {
    MAIN_MENU,
    WAITING_PLAYER,
    GAME,
    PAUSE_MENU,
    GAME_OVER,
    EXIT,
} game_state;

/** Load the sprites and maps, created the character, subscribes the drivers, changes the video mode
 * @brief Initializes everything needed for the game 
 * 
 * @return int 1 if an error occur, 0 otherwise
 */
int game_init();

/**
 * @brief creates a loop where the game will be played and where it detects all the drivers interruptions and process them controlling all the elements and the states of the game
 * 
 * @return int 1 if an error occurs, 0 otherwise
 */
int game_loop();

/** unsubscribes the drivers, change video mode to text mode, destroys pointers and clean alocated memory
 * @brief exits the game mode 
 * 
 * @return int 1 if an error occurs 0 otherwise
 */
int game_exit();

/**
 * @brief call the functions to draw in the screen and controlls what needs to be drawn
 * 
 * @return int 1 if an error occurs, 0 otherwise
 */
int draw_screen();

/**
 * @brief draws the main menu and controlls if an option is chosen
 * 
 * @return int 1 if an error occur, 0 otherwise
 */
int draw_main_menu();

/**
 * @brief draws the elements of the game
 * 
 * @return int 1 if an error occurs, 0 otherwise
 */
int draw_game();

int draw_waiting_player_background();

/**
 * @brief draws the pause menu
 * 
 * @return int 1 if an error occur, 0 otherwise
 */
int draw_pause_menu();

/**
 * @brief draws the game over screen and controlls if any of the options is chosen
 * 
 * @return int 1 if an error occurs, 0 otherwise
 */
int draw_game_over();

/**
 * @brief moves the fireboy depending of the keyborad key
 * 
 * @param key keyboard key pressed
 * @return int 1 if an error occurs, 0 otherwise
 */
int fireboy_move(keyboard_key key);

/**
 * @brief moves the watergirl depending of the keyborad key
 * 
 * @param key keyboard key pressed
 * @return int 1 if an error occurs, 0 otherwise
 */
int watergirl_move(keyboard_key key);

/**
 * @brief Draws the ropes on the screen. And controlls whether or not the characters can or have cut them.
 */
void draw_ropes();

/**
* @brief Exists multiplayer mode
*/
void exit_multiplayer();

/**
* @brief Retrives information from the serial port to check what the remote player has done and replicates it
*/
void handle_remote_player();

/**
*@brief Starts multiplayer session
*/
void start_multiplayer();

/**
* @brief Exists the game
*/
int exit_game();

/**
*@brief Restarts the game
*/
int restart_game();

#endif /* __GAME_H */
