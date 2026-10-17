#ifndef _GRID_H_
#define _GRID_H_

#include <lcom/lcf.h>

#include <stdio.h> 
#include <stdbool.h> 
#include <stdlib.h> 
#include <time.h> 

#include "libs/kbc/include/kbc.h"
#include "libs/kbc/include/i8042.h"
#include "libs/timer/include/i8254.h"
#include "libs/utils/include/utils.h"  
#include "libs/timer/include/timer.h" 
#include "libs/kbc/include/mouse.h" 
#include "libs/kbc/include/event_handler.h"
#include "libs/vbe/include/vbe_macros.h"
#include "libs/vbe/include/vbe_functions.h"
#include "xpm/numbers.h"
#include "xpm/highlighted_numbers.h"
#include "game/include/grid.h"
#include "game/include/sudoku.h"
#include "xpm/menu.h"
#include "xpm/difficulties.h"
#include "xpm/game_elements.h"

#define GRID_XPOS            327        /* X coordinate of the first pixel of the grid */
#define GRID_YPOS            181        /* X coordinate of the first pixel of the grid */
#define GRID_WIDTH           500        /* Width of the grid */
#define GRID_HEIGHT          500        /* Width of the grid */
#define GRID_COLOR           0xFF0000   /* Color of the grid */
#define COUNTER_XPOS         510        /* X coordinate of the first pixel of the counter */
#define COUNTER_YPOS         747        /* X coordinate of the first pixel of the counter */

int num_aux;                            /* Auxiliary variable that temporarily stores the input number */

/**
 * @brief Prints the main menu in the video mode
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (init_menu)(void);

/**
 * @brief Prints the difficulty menu in the video mode
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (init_diff_menu)(void);

/**
 * @brief Prints the initial unsolved puzzle in the video mode
 * 
 * @param x X coordinate of the first pixel of the grid
 * @param y Y coordinate of the first pixel of the grid
 * @param width Width of the grid
 * @param height Height of the grid
 * @param color Hex color of the grid
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (init_grid)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Highlights the current player position in the grid
 * 
 * @param x X coordinate of the first pixel of the grid
 * @param y Y coordinate of the first pixel of the grid
 * @param ind_x X position of the cell in the grid
 * @param ind_y Y position of the cell in the grid
 * @param grid NxN array containing the player's attempt
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (highlight_cell)(int x, int y, int ind_x, int ind_y, int grid[N][N]);

/**
 * @brief Writes or overwrites the value in the selected cell
 * 
 * @param ind_x X position of the cell in the grid
 * @param ind_y Y position of the cell in the grid
 * @param xpm XPM to be printed on the selected cell
 * 
 * @return Void function
 */
void (write_cell)(int ind_x, int ind_y, xpm_map_t xpm);

/**
 * @brief Removes the value of the selected cell
 * 
 * @param x X coordinate of the first pixel of the grid
 * @param y Y coordinate of the first pixel of the grid
 * @param ind_x X position of the cell in the grid
 * @param ind_y Y position of the cell in the grid
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (clear_cell)(int x, int y, int ind_x, int ind_y);

/**
 * @brief Parses a value and returns its matching xpm
 * 
 * @param num Number to be evaluated
 * 
 * @return Return a XPM
 */
xpm_map_t (number_selector)(int num);

/**
 * @brief Parses a value and returnsits matching highlighted xpm
 * 
 * @param num Number to be evaluated
 * 
 * @return Return a XPM
 */
xpm_map_t (high_num_selector)(int num);

/**
 * @brief Parses the scancode, checks for a movement in any diretcion 
 *  and updates current position
 * 
 * @param x pointer to the current X position in the menu
 * @param y pointer to the current position of difficulty
 * @param code 8-bit code read from the keyboard
 * 
 * @return Return 0 upon success and non-zero otherwise 
 */
int (menu_movement)(int *x, uint8_t code);

/**
 * @brief Parses the scancode, checks for a movement in any diretcion 
 *  and updates current position
 * 
 * @param x pointer to the current position of difficulty
 * @param code 8-bit code read from the keyboard
 * 
 * @return Return 0 upon success and non-zero otherwise 
 */
int (diff_movement)(int *x, uint8_t code);

/**
 * @brief Parses the scancode, checks for a movement in any diretcion 
 *  and updates current position
 * 
 * @param x pointer to the current X position in the grid
 * @param y pointer to the current Y position in the grid
 * @param code 8-bit code read from the keyboard
 * @param grid NxN array containing the player's attempt
 * @param comp NxN array containing the initial unsolved puzzle
 * 
 * @return Return 0 upon success and non-zero otherwise 
 */
int (grid_movement)(int *x, int *y, uint8_t code, int grid[N][N], int comp[N][N]);

/**
 * @brief Parses the scancode, checks for a number input and returns it
 * 
 * @param code 8-bit code read from the keyboard
 * 
 * @return Return an integer (Ranges from -1 to 9)
 */
int (number_input)(uint8_t code);

/**
 * @brief Compares two grids
 * 
 * @param grid NxN array containing the player's attempt
 * @param solution NxN array containing the generated solution
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (finished)(int grid[N][N], int solution[N][N]);

/**
 * @brief Parses cursor position and updates position in the grid
 * 
 * @param grid NxN array containing the player's attempt
 * @param x_cur mouse x coordinate
 * @param x_cur mouse y coordinate
 * @param pos_x pointer to x position in the grid
 * @param pos_y pointer to y position in the grid
 * 
 * @return Return 0 if in the grid and 1 if outside
 */
int (get_mouse_pos)(int grid[N][N],int x_cur,int y_cur,int *pos_x,int *pos_y);

/**
 * @brief Parses cursor position and selects option
 * 
 * @param x_cur mouse x coordinate
 * @param x_cur mouse y coordinate
 * @param option pointer to option value
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (get_mouse_option_diffmenu)(int x_cur,int y_cur,int *option);

/**
 * @brief Parses cursor position and selects option
 * 
 * @param x_cur mouse x coordinate
 * @param x_cur mouse y coordinate
 * @param option pointer to option value
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (get_mouse_option_initmenu)(int x_cur,int y_cur,int *option);

#endif /* _VBE_MACROS_H_ */
