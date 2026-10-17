#pragma once

#include "devices/graphic/video.h"
#include "images/buttons/faceGameOver.xpm"
#include "images/buttons/faceRestart.xpm"
#include "images/buttons/instructionsButton.xpm"
#include "images/buttons/playButton.xpm"
#include "images/buttons/quitButton.xpm"
#include "images/cells/bannerCell.xpm"
#include "images/cells/mineCell.xpm"
#include "images/cells/normalCell.xpm"
#include "images/cells/pressCell.xpm"
#include "images/cells/selected.xpm"
#include "images/cells/oneCell.xpm"
#include "images/cells/twoCell.xpm"
#include "images/cells/threeCell.xpm"
#include "images/cells/fourCell.xpm"
#include "images/cells/fiveCell.xpm"
#include "images/cells/sixCell.xpm"
#include "images/cells/sevenCell.xpm"
#include "images/cells/eightCell.xpm"
#include "images/boards/board_16x16.xpm"
#include "images/boards/board_20x16.xpm"
#include "images/boards/board_28x20.xpm"
#include "images/boards/grid_16x16.xpm"
#include "images/boards/grid_20x16.xpm"
#include "images/boards/grid_28x20.xpm"
#include "images/letters/colon.xpm"
#include "images/numbers/zero.xpm"
#include "images/numbers/one.xpm"
#include "images/numbers/two.xpm"
#include "images/numbers/three.xpm"
#include "images/numbers/four.xpm"
#include "images/numbers/five.xpm"
#include "images/numbers/six.xpm"
#include "images/numbers/seven.xpm"
#include "images/numbers/eight.xpm"
#include "images/numbers/nine.xpm"
#include "images/font.xpm"
#include "images/font_white.xpm"
#include "images/menu.xpm"
#include "images/handCursor.xpm"
#include "images/emptyCursor.xpm"
#include "images/instructionsPage.xpm"
#include "images/title.xpm"
#include "images/gameOver.xpm"
#include "images/win.xpm"

/**
 * @brief This function loads various XPM images into memory. Each XPM image is loaded using the 'xpm_load' function and stored in the corresponding variable.
 * @return 0 if all XPM images are successfully loaded.
*/
int load_xpms();

/**
 * @brief This function is responsible for drawing the main menu on the screen.
*/
void draw_main_menu();

/**
 * @brief This function is responsible for drawing the cursor on the screen at the specified coordinates (x, y).
 * @param x An unsigned 16-bit integer representing the x-coordinate of the cursor's position.
 * @param y An unsigned 16-bit integer representing the y-coordinate of the cursor's position.
*/
void draw_cursor(uint16_t x, uint16_t y);

/**
 * @brief This function is responsible for erasing the cursor from the screen at the specified coordinates (x, y).
 * @param x An unsigned 16-bit integer representing the x-coordinate of the cursor's position.
 * @param y An unsigned 16-bit integer representing the y-coordinate of the cursor's position.
*/
void erase_cursor(uint16_t x, uint16_t y);

/**
 * @brief This function draws the 16x16 game board on the screen.
*/
void draw_board_16x16();

/**
 * @brief This function draws the 20x16 game board on the screen.
*/
void draw_board_20x16();

/**
 * @brief This function draws the 28x20 game board on the screen.
*/
void draw_board_28x20();

/**
 * @brief This function draws the "face restart" image on the screen at a specific position in the 16x16 game board.
*/
void draw_faceRestart_16x16();

/**
 * @brief This function draws the "face restart" image on the screen at a specific position in the 20x16 game board.
*/
void draw_faceRestart_20x16();

/**
 * @brief This function draws the "face restart" image on the screen at a specific position in the 28x20 game board.
*/
void draw_faceRestart_28x20();

/**
 * @brief This function draws the "pressed cell" image on the screen at the specified position.
 * @param x The x-coordinate of the position to draw the image.
 * @param y The y-coordinate of the position to draw the image.
 */
void draw_pressCell(int x, int y);

/**
 * @brief This function displays the "win menu" image on the screen.
*/
void draw_win_menu();

/**
 * @brief This function displays the "gameover menu" image on the screen.
*/
void draw_gameover_menu();

/**
 * @brief This function displays the "instructions menu" image on the screen.
*/
void draw_instructions_menu();

/**
 * @brief This function iterates over the board array and uses the 'draw_xpm' function to draw the appropriate XPM image for each cell based on its content.
 * @param board 16x16 character array representing the state of the game board. Each element of the array corresponds to a cell in the grid and contains a character representing the cell's content.
*/
void draw_grid_16x16(char board[16][16]);

/**
 * @brief This function iterates over the board array and uses the 'draw_xpm' function to draw the appropriate XPM image for each cell based on its content.
 * @param board 20x16 character array representing the state of the game board. Each element of the array corresponds to a cell in the grid and contains a character representing the cell's content.
*/
void draw_grid_20x16(char board[20][16]);

/**
 * @brief This function iterates over the board array and uses the 'draw_xpm' function to draw the appropriate XPM image for each cell based on its content.
 * @param board 28x20 character array representing the state of the game board. Each element of the array corresponds to a cell in the grid and contains a character representing the cell's content.
*/
void draw_grid_28x20(char board[28][20]);

/**
 * @brief This function selects the appropriate XPM image based on the provided number and uses the draw_xpm function to draw it on the screen at the specified position.
 * @param number The number to be drawn on the screen. It should be an integer between 0 and 9.
 * @param x The X-coordinate of the position where the number will be drawn.
 * @param y The Y-coordinate of the position where the number will be drawn.
*/
void draw_number(int number, int x, int y);

/**
 * @brief This function draws a colon symbol on the screen at the specified position.
 * @param x The X-coordinate of the position.
 * @param y The Y-coordinate of the position.
*/
void draw_colon(int x, int y);

/**
 * @brief This function draws the final time on the screen.
 * @param m1 The first digit of the minutes.
 * @param m2 The second digit of the minutes.
 * @param s1 The first digit of the seconds.
 * @param s2 The second digit of the seconds.
*/
void draw_final_time(int m1, int m2, int s1, int s2);

/**
 * @brief This function is used to visually represent a selected cell on the screen.
*/
void draw_selected_cell(int x, int y);
