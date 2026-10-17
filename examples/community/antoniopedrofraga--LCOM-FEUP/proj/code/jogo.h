#ifndef __JOGO_H
#define __JOGO_H

#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "i8042.h"
#include "i8254.h"
#include "timer.h"
#include "video_gr.h"
#include "kb.h"
#include "mouse.h"
#include "Bitmap.h"
#include "sudoku.h"
#include <time.h>
#include <string.h>

/** @defgroup JOGO JOGO
 * @{
 * Functions of Game Engine
 */

/**
 * @brief Draws the game chronometer on double buffer
 *
 * @param game game data structure
 * @param double_buffer pointer to double buffer memory address
 */
void display_time(Game*game,unsigned char**double_buffer);
/**
 * @brief Copies double buffer data to VM
 *
 * @param double_buffer pointer to double buffer memory address
 */
void draw_background(unsigned char*double_buffer);

/**
 * @brief Checks if the mouse is hovering a bitmap.
 *
 * @param bmp used to get width and height of hovered bitmap
 * @param bmp2 used to be drawn on VM (a sign that we are hovering bmp)
 * @param mouse used to get mouse coords
 * @param x used to know where the bmp is drawn
 * @param y used to know where the bmp is drawn
 * @return 1 if is hovered and mouse left button is pressed, 0 if its not, and -1 if bmp2 couldn't be drawn
 */
int mouse_hover(Bitmap*bmp,Bitmap*bmp2,Mouse mouse, int x, int y);

/**
 * @brief Checks if the mouse is hovering a sudoku
 *
 * @param bmp used to get width and height of hovered bitmap
 * @param mouse used to get mouse coords
 * @param x used to know where the bmp is drawn
 * @param y used to know where the bmp is drawn
 * @param hovered used to know if a square can bee hovered (if it's not loaded on a pre-made sudoku)
 * @return 1 if is hovered and mouse left button is pressed, 0 if its not, and -1 if bmp couldn't be drawn
 */
int mouse_hover_board(Bitmap*bmp,Mouse mouse, int x, int y,int hovered);

/**
 * @brief Updates mouse coords and pressed buttons, and draws the mouse bitmap on Video Memory. Also controls the mouse draw area.
 *
 * @param game Data structure to be updated with the new mouse coords
 * @param array Previously received mouse packets
 */

int draw_mouse(Game*game, int array[3]);


/**
 * @brief Updates text file with the new best times when someone wins
 *
 * @param game Data structure
 */

int update_highScores(Game*game);

/**
 * @brief Listen the keyboard when a sudoku board square is pressed
 *
 * @param game Data structure (used to access some bitmaps)
 * @param x pos of square on a sudoku board (from 0 to 2). It's an easy way to access the 3D array
 * @param y pos of square on a sudoku board (from 0 to 2). It's an easy way to access the 3D array
 * @param squares square number to be accessed (from 0 to 9). It's an easy way to access the 3D array
 * @param double_buffer used to keep game chronometer updated on the screen
 * @return value to be filled on sudoku board
 */

int listen_kb(Game*game,int x,int y,int squares,unsigned char**double_buffer);

/**
 * @brief Draws number on a sudoku board square
 * @param game Data Structure used to access bitmaps
 * @param x used to know where the bmp is drawn
 * @param y used to know where the bmp is drawn
 * @param choice value to be drawn (from 1 to 9)
 * @param double_buffer Bitmaps are drawn on this memory address
 * @param correct value to know if the value is respecting sudoku rules
 */

int draw_on_board(Game*game,int x, int y, int choice,unsigned char **double_buffer, int correct);

/**
 * @brief Checks if any of the sudoku board squares are being hovered
 * @param game Data Structure used to access the 3D array values and to draw some bitmaps
 * @param double_buffer memory address where the bitmaps will be drawn
 */

int hover_squares(Game*game,unsigned char**double_buffer);

/**
 * @brief Returns a number bitmap
 * @param game Data Structure used to access loaded bitmap
 * @param choice number value to be converted in bitmap
 * @return returns a bitmap of a given number
 */

Bitmap*get_bitmap(Game*game,int choice);

/**
 * @brief Displays a two algarism number on screen
 * @param game Data Structure used to access bitmaps
 * @param number number to be drawn
 * @param x x pos to draw bitmap
 * @param y y pos to draw bitmap
 * @param double_buffer Memory Address to draw bitmap
 */


int display_number(Game*game,int number, int x, int y,unsigned char**double_buffer);

/**
 * @brief Loads sudoku pre-made game from text file according difficulty
 * @param game Data Structure used to fill 3D array
 * @param difficulty 1 for easy, 2 for medium, 3 for hard
 */

int load_sudoku(Game*game,int difficulty);

/**
 * @brief Gives default values to the sudoku board squares on the 3D game array
 * @param game Data Structure used to fill 3D array
 */

void clean_board(Game*game);

/**
 * @brief Used to draw game interface after an update
 * @param game Data Structure used to access some bitmaps and get mouse coords
 * @param array Used to access mouse packets previously received
 * @param double_buffer Memory Adress containing data to be copied to Video Memory
 */

int draw_game(Game*game,int array[3], unsigned char*double_buffer);

/**
 * @brief Used to draw the winner menu
 * @param game Data Structure used to access some bitmaps
 */
int winner_menu(Game*game);

/**
 * @brief It's the game menu, where the sudoku board is shown
 * @param game Data Structure passed by reference to be accessed and updated
 * @param difficulty 1 for easy, 2 for medium, 3 for hard
 */

int menu3(Game*game,int difficulty);

/**
 * @brief It's the menu where the player choose his difficulty
 * @param game Data Structure passed by reference to be accessed and updated
 */

int menu2(Game*game);

/**
 * @brief Display the data that is received by the Real Time Counter
 * @param game Data Structure passed by reference to be accessed
 * @param double_buffer Where the date will be drawn
 */

int display_date(Game*game,unsigned char*double_buffer);

/**
 * @brief Displays the Best Times ever made playing this sudoku
 * @param game Data Structure passed by reference to be accessed
 * @param double_buffer Where the times will be drawn
 */

int display_hs(Game*game,unsigned char ** double_buffer);

/**
 * @brief Displays the first menu, used to call another menus
 * @param game Data Structure passed by reference to be accessed
 */

int menu1(Game*game);


/**
 * @brief Used to check if a number it's well placed on the sudoku board, and if it respects all sudoku rules
 * @param game Data Structure passed by reference to be accessed
 * @param x x coords of square
 * @param y x coords of square
 * @param squares number of a square to be accessed
 * @return 0 if the square doesn't respect sudoku rules, and 1 otherwise
 */

int according_rulles(Game*game,int x, int y, int squares);

/**
 * @brief Used to check if the sudoku board is well filled, and to draw a disrespectful number.
 * @param game Data Structure passed by reference to be accessed
 * @param double_buffer used to draw data on this memory address
 */

int check_game(Game*game,unsigned char**double_buffer);

#endif

/**@}*/
