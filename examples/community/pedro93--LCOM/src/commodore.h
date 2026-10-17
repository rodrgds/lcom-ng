#ifndef _COMMODORE_H
#define _COMMODORE_H_
#include "sprite.h"
/** @defgroup Commodore Commmodore

 * @{ Functions related to the game engine and AI 
 */

/**
 * @brief draws a sprite *spr at the parameter position of the screen
 * 
 * @param x value in screen to be displayed the sprite pointer
 * @param y value in screen to be displayed the sprite pointer
 * @param Sprite pointer containing the information required to generate an image on the screen in video mode
 * 
 * @return void
 */
void draw_sprite(int x,int y,Sprite *spr);

/**
 * @brief Draws an outline with a pixel thick of the color desired,starting at position (x,y)
 * 
 * @param x value in screen in which the outline will begin
 * @param y value in screen in which the outline will begin
 * @param width of outline
 * @param height of outline
 * @param colour outline
 *
 * @return void
 */
void draw_outline(int x,int y,int width,int height,unsigned long color);

/**
 * @brief Draws a square at position (x,y),with the width,height and color specified as argument
 *
 * @param x value in screen in which the square will begin
 * @param y value in screen in which the square will begin
 * @param width of square
 * @param height of square
 * @param colour square
 *
 * @return void
 */
void draw_square(int x,int y,int width,int height,unsigned color);

/**
 * @brief Draws the AI board where the game is going be played
 * it initializes every position of the AI_board structure
 * it initializes every position of the AI_board_aux with the ID 0
 *
 * @return void
 */
void draw_player1_board();

/**
 * @brief Draws the player1 board where the AI is going do is shots
 * it initializes every position of the player1_board structure
 * it initializes every position of the player1_board_aux with the ID 0
 *
*  @return void
 */
void draw_player2_board();

/**
* @brief Draws the black outline of 5 pixels surronding the AI_board
* 
* @return void
*/
void draw_player1_board_outline();

/**
* @brief Draws the black outline of 5 pixels surrounding the player1 board
* 
* @return void
*/
void draw_player2_board_outline();

/**
* @brief Fills all the pixels of the screen with the color blue
* 
* @return void
*/
void erase_screen();

/**
* @brief Draws the ships that stand next to each table
* 
* @return void
*/
void draw_ships();

/**
* @brief Draws the "player 1" and "computer" sprites on the game
* 
* @return void
*/
void draw_players();

/**
* @brief Draws the letter and number on the game tables
* 
* @return void
*/
void draw_letters_numbers();

/**
* @brief Draws the initial menu that appears before the game
* 
* @return void
*/
void menu();

/**
* @brief Initializes the virtual keyboard cursor 
* at the position (0,0) of the AI board
* 
* @return void
*/
void initialize_keyboard_cursor();

/**
* @brief Initializes the virtual keyboard cursor at the 
* menu before the game starts surrounding the "play" word
* 
* @return void
*/
void initialize_keyboard_cursor_for_menu();

/**
* @brief Function to randomly place the AI ships in the AI table
* It updates the information of the AI_table with 0's and 1's corresponding 0 to water and 1 to a ship
* It updates the AI_table_aux with the ships ID as it puts the ships in the table
*
* @return void
*/
void place_AI_ships();

/**
* @brief Function to randomly place the player1 ships in the player1 table
* It updates the information of the player1_table with 0's and 1's corresponding 0 to water and 1 to a ship
* It updates the player1_table_aux with the ships ID as it puts the ships in the table
* 
* @return void
*/
void place_player1_ships();

/**
* @brief Moves the virtual keyboard cursor of the player2 table by redrawing 
* the outline in the position desired and making the structure changes need to 
* refresh the x and y saved
*
* @param int value indicates in witch position to move the cursor (1-up || 2-left || 3-down || 4-right)
* @return void
*/
void move_keyboard_cursor(int value);

/**
* @brief Checks if any of the player1 ships has been destroyed and 
* if so draws the ship sprite next to the table as red
*
* @return void
*/
void check_player1_destroyed_ships();

/**
* @brief Checks if any of the AI ships has been destroyed and if 
* so draws the ship sprite next to the table as red
* 
* @return void
*/
void check_player2_destroyed_ships();

/**
* @brief Indicates that the volley special attack was made by the player1 
* - number 1 or 2 key pressed
* 
*@return void
*/
void set_volley_flag();

/**
* @brief Indicates that the scout special attack was made by the player1 
* - number 3 key pressed
* 
* @return void
*/
void set_scout_flag();

/**
* @brief Indicates that the bombardement special attack was made by the player1
* - number 4 key pressed
* 
* @return void
*/
void set_bombardement_flag();

/**
* @brief Indicates that the volley special attack was made by the AI
* 
* @return void
*/
void AI_set_volley_flag();

/**
* @brief Indicates that the scout special attack was made by the AI
* 
* @return void
*/
void AI_set_scout_flag();

/**
* @brief Indicates that the bombardement special attack was made by the AI
* 
* @return void
*/
void AI_set_bombardement_flag();

/**
* @brief Checks the square where the virtual keyboard cursor is 
* located after receiving a "enter" scancode
* 
* @return void
*/
void check_square_keyboard();

/**
* @brief Used as the interrupt_handler condition terminator
*
* @return number of tries 
*/
int check_number_of_tries();

/**
* @brief Initializes the AI_shot variable
* 
* @return void
*/
void initialize_AI_shot();

/**
* @brief AI engine
* 
* @return void
*/
void AI_turn();

/**
* @brief Calls the the functions to draw the player1 and AI table
* 
* @return void
*/
void draw_tables();

/**
* @brief Draws all of the graphical interface of the game by calling other functions
* 
* @return void
*/
int draw_graphical_interface();

/**
* @brief Starts the interrupt handler loop
* 
* @return 1 if the play option was selected,0 if the quit option was selected
*/
void player1_turn();

/**
* @brief 1st special attack
* Hits all of the squares in a randomly generated column or row
* 
* @return void
*/
void AI_volley();

/**
* @brief 1st special attack
* Hits all of the squares in a specified horizontal row
* 
* @return void
*/
void player1_horizontal_volley();

/**
* @brief 1st special attack
* Hits all of the squares in a specified vertical row
* 
* @return void
*/
void player1_vertical_volley();

/**
* @brief 2nd special attack
* Searches in the table for a piece of a ship with a random 
* id that hasn't be hit before and making it visible
* 
* @return void
*/
void AI_scout();

/**
* @brief 2nd special attack
* Searches in the table for a piece of a ship with a random 
* id that hasn't be hit before and making it visible
* 
* @return void
*/
void player1_scout();

/**
* @brief 3rd special attack
* Makes a explosion with the size of
* 3x3 square using the actual square as a middle point
* 
* @return void
*/
void AI_bombardement();

/**
* @brief 3rd special attack
* Makes a explosion with the size of
* 3x3 square using the actual square as a middle point
* 
* @return void
*/
void player1_bombardement();

/**
* @brief Draws a "you win" or "you lose" 
* message and the time of the game
* 
* @param int value containing RTC time in seconds when game starts
* @param int value containing RTC time in seconds when game ends
* 
* @return void
*/
void draw_end_of_game(unsigned int start,unsigned int end);

/**
* @brief Starts the game
* 
* @return void
*/
void start_commodore();


#endif