#pragma once

#include <lcom/lcf.h>
#include <math.h>

#include "devices/graphic/video.h"
#include "devices/rtc/rtc.h"
#include "game/display.h"
#include "game/game_logic.h"

/**
 * @brief Enumeration representing the different states of the game.
*/
typedef enum {
  BEGINNER,
  MEDIUM,
  HARD,
  MAIN_MENU,
  WIN,
  GAMEOVER,
  INSTRUCTIONS,
  EXIT
} game_state;

/**
 * @brief Structure representing a position in a 2D space.
*/
typedef struct {
  uint16_t x;
  uint16_t y;
} Position;

extern game_state state;
extern Position cursor_pos;

extern int mines, x_tiles, y_tiles;
extern int y_matrix, x_matrix;
extern int y_matrix_kbd, x_matrix_kbd;
extern char board_16x16[][16];
extern char board_20x16[20][16];
extern char board_28x20[28][20];
extern Time* timer;
extern Time* initialtimer;
extern int m1, m2, s1, s2;
extern int y_coord, x_coord;

/**
 * @brief This function updates the position of the cursor based on the provided movement values.
 * @param x_val The amount of movement in the x direction.
 * @param y_val The amount of movement in the y direction.
 */
void update_cursor(uint16_t x_val, uint16_t y_val);

/**
 * @brief This function starts a game on a 20x16 board.
*/
void start_game_20x16();

/**
 * @brief This function starts a game on a 28x20 board.
*/
void start_game_28x20();

/**
 * @brief This function starts a game with a 16x16 grid. Initializes the game timer, sets the number of tiles in the x and y directions, and places mines on the game board.
*/
void start_game_16x16();

/**
 * @brief This function draws the game over menu on the screen.
*/
void _gameover_menu();

/**
 * @brief This function takes the current position of the cursor and draws it on the screen.
*/
void cursor_draw();

/**
 * @brief This function calls the specific draw function for a 16x16 board to draw the game board on the screen.
*/
void _draw_board();

/**
 * @brief This function calls the specific draw function to draw the main menu.
*/
void _main_menu();

/**
 * @brief This function calls the specific draw function to draw the win menu.
*/
void _win_menu();

/**
 * @brief This function calls the specific erase function to remove the cursor.
*/
void cursor_erase();

/**
 * @brief This function calls the specific draw function to draw the instruction menu.
*/
void _instructions_menu();

/**
 * @brief This function checks the mouse clicks within the menu screen and updates the game state accordingly.
*/
void check_mouse_clicks_menu();

/**
 * @brief This function checks for a left mouse click in the beginner mode.
*/
void check_mouse_click_left_beginner();

/**
 * @brief This function checks for a right mouse click in the beginner mode.
*/
void check_mouse_click_right_beginner();


/**
 * @brief This function checks for a left mouse click in the medium mode.
*/
void check_mouse_click_left_medium();

/**
 * @brief This function checks for a right mouse click in the medium mode.
*/
void check_mouse_click_right_medium();

/**
 * @brief This function checks for a left mouse click in the hard mode.
*/
void check_mouse_click_left_hard();

/**
 * @brief This function checks for a right mouse click in the hard mode.
*/
void check_mouse_click_right_hard();

/**
 * @brief This function handles the left mouse click event and updates the game state accordingly.
*/
void check_mouse_click_left_beginner();

/**
 * @brief This function handles the right mouse click event and updates the game state accordingly.
*/
void check_mouse_click_right_beginner();

/**
 * @brief This function sets the game state to 'MAIN_MENU', allowing the player to return to the main menu after winning the game.
*/
void check_mouse_clicks_win();

/**
 * @brief This function sets the game state to 'MAIN_MENU', allowing the player to return to the main menu after the game is finished.
*/
void check_mouse_clicks_finished();

/**
 * @brief This function calculates the elapsed time since the game started and displays it in the format "mm:ss".
 * @param timer An object of type Time.
*/
void game_draw_time(Time* timer);


/**
 * @brief This function is responsible for detecting the 'W' key press on the keyboard and performing the corresponding actions based on the current position represented by the 'y_matrix_kbd' variable.
*/
void check_keyboard_w();

/**
 * @brief This function detects the 'A' key press on the keyboard and performs specific actions based on the current position represented by the 'x_matrix_kbd' variable. 
*/
void check_keyboard_a();

/**
 * @brief This function checks for the 'S' key press on the keyboard in a 16x16 grid.
*/
void check_keyboard_s_16x16();

/**
 * @brief This function checks for the 'S' key press on the keyboard in a 20x16 grid.
*/
void check_keyboard_s_20x16();

/**
 * @brief This function checks for the 'S' key press on the keyboard in a 28x20 grid.
*/
void check_keyboard_s_28x20();

/**
 * @brief This function checks for the 'D' key press on the keyboard in a 16x16 grid.
*/
void check_keyboard_d_16x16();

/**
 * @brief This function checks for the 'D' key press on the keyboard in a 20x16 grid.
*/
void check_keyboard_d_20x16();

/**
 * @brief This function checks for the 'D' key press on the keyboard in a 28x20 grid.
*/
void check_keyboard_d_28x20();

/**
 * @brief This function checks for the 'Enter' key press on the keyboard in a 16x16 grid.
*/
void check_keyboard_enter_16x16();

/**
 * @brief This function checks for the 'Enter' key press on the keyboard in a 20x16 grid.
*/
void check_keyboard_enter_20x16();

/**
 * @brief This function checks for the 'Enter' key press on the keyboard in a 28x20 grid.
*/
void check_keyboard_enter_28x20();

/**
 * This function checks for the 'F' key press on the keyboard in a 16x16 grid.
*/
void check_keyboard_f_16x16();

/**
 * This function checks for the 'F' key press on the keyboard in a 20x16 grid.
*/
void check_keyboard_f_20x16();

/**
 * This function checks for the 'F' key press on the keyboard in a 28x20 grid.
*/
void check_keyboard_f_28x20();

