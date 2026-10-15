#ifndef _DRAW_SCREENS_H_
#define _DRAW_SCREENS_H_

#define TIMER_INITIAL 60      
#define TIMER_X 10
#define TIMER_Y 10

#define MAX_HISTORY_SIZE 6

#include <lcom/lcf.h>
#include <stdint.h>
#include "drawingLogic.h"
#include "xpm_files.h"

/**
 * @brief Loads XPM images for digits and characters into arrays xpm_array_digits and xpm_array_char.
 * 
 * @return 0 on success or 1 if XPM is invalid.
 */
int (load_char_xpms)();

/**
 * @brief Calculates the x coordinate for the top left corner of a string for it to be centered on the screen.
 *
 * @param size_str Length of the string.
 * @param x_middle Pointer to an integer to store the value of coordinate x.
 * @return 0 on success.
 */
int (write_on_center)(int size_str, int* x_middle);

/**
 * @brief Calculates remaining time and draws it inside a rectangle.
 *
 * @param elapsed_time time in seconds since the timer started.
 * @return 0 on success or 1 if remaining time is negative or draw fails.
 */
int (draw_timer_display)(int elapsed_time);

/**
 * @brief Draws a mouse cursor at given coordinates.
 *
 * @param x int value of coordinate x of the cursor.
 * @param y int value of coordinate y of the cursor.
 * @param mouse_map pointer to XPM pixel map of the mouse. 
 * @param img a struct containing the images’s width, height and type information.
 * @return 0 on success and 1 if a draw function fails.
 */
int (draw_cursor)(int x, int y, uint8_t* mouse_map, xpm_image_t img);

/**
 * @brief draws a string using character XPMs.
 * 
 * @param string array of characters to draw.
 * @param size number of characters in string.
 * @param x starting x coordinate.
 * @param y starting y coordinate.
 * @return 0 on success and 1 if a draw function fails.
 */
int (draw_string)(char string[], int size, int x, int y);

/**
 * @brief Draws a main menu screen (title, "START" button and cursor). 
 * 
 * @param mouse_map pointer to XPM pixel map of the mouse. 
 * @param img a struct containing the images’s width, height and type information.
 * @param mouse_x int value of coordinate x of the cursor.
 * @param mouse_y int value of coordinate y of the cursor.
 * @param c_mouse pointer for checking if mouse needs to be redrawn (c_mouse > 0)
 * @param new_screen pointer to how many buffers left to atualize.
 * @return 0 on success and 1 if a draw function fails.
 */
int (draw_main_menu)(uint8_t* mouse_map, xpm_image_t img, int mouse_x, int mouse_y, int* c_mouse, int* new_screen);

/**
 * @brief Draws "YOU LOST" in the center of the screen. 
 * 
 * @param new_screen pointer to how many buffers left to atualize.
 * @return 0 on success and 1 if a draw function fails.
 */
int (draw_lose_menu)(int* new_screen);

/**
 * @brief Draws "YOU WON" in the center of the screen.
 * 
 * @param new_screen pointer to how many buffers left to atualize.
 * @return 0 on success and 1 if a draw function fails.
 */
int (draw_win_menu)(int* new_screen);

/**
 * @brief draws the main game screen, including drawing screen, contents drawn, mouse cursor,"CLEAR" button, timer, text field and history.
 * 
 * @param drawingScreen array of pixels representing the drawing screen area.
 * @param mouse_map pointer to XPM pixel map of the mouse.
 * @param img a struct cointaining the cursor image's width, height and type information.
 * @param x int value of coordinate x of the cursor.
 * @param y int value of coordinate y of the cursor.
 * @param elapsed_time number of timer interrupts since the countdown menu was started.
 * @param string current player guess.
 * @param size_str current lenght of string.
 * @param c_mouse pointer for checking if mouse and drawing area need to be redrawn (c_mouse > 0).
 * @param c_text pointer for checking if text or history area need to be redrawn (c_text > 0).
 * @param c_timer pointer for checking if timer needs to be redrawn (c_timer > 0).
 * @param new_screen pointer to how many buffers left to atualize.
 * @param history array with the MAX_HISTORY_SIZE previous tries submissions.
 * @param history_sizes array with the sizes of the MAX_HISTORY_SIZE previous tries.
 * @param history_count number of strings in history.
 * @return 0 on success and 1 if a draw function fails.

 */
int (draw_guessing_screen)(uint32_t drawingScreen[], uint8_t* mouse_map, xpm_image_t img, int x, int y, int elapsed_time, char string[], int size_str, int* c_mouse, int* c_text, int* c_timer, int* new_submit, int* new_screen, char history[MAX_HISTORY_SIZE][3 * MAX_STR_LEN], int history_sizes[MAX_HISTORY_SIZE], int history_count);

/**
 * @brief Draws the countdown and word to be drawn before game starts.
 * 
 * @param elapsed_time number of timer interrupts since the countdown menu was started.
 * @param subject word to be drawn/guessed.
 * @param new_screen pointer to how many buffers left to atualize.
 * @return 0 on success and 1 if a draw function fails.
 */
int (draw_countdown_screen)(int elapsed_time, char* subject, int* new_screen);

/**
 * @brief Draws a button with the string "CLEAR".
 * 
 * @return 0 on success and 1 if a draw function fails.
 */
int (draw_clear_button)();

/**
 * @brief Draws the history area where previous guesses are displayed.
 * 
 * @param x int value of coordinate x of the top left corner of history area.
 * @param y int value of coordinate y of the top left corner of history area.
 * @param history array with the MAX_HISTORY_SIZE previous tries submissions.
 * @param history_sizes array with the sizes of the MAX_HISTORY_SIZE previous tries.
 * @param history_count number of strings in history.
 * @return 0 on success and 1 if a draw function fails.
 */
int (draw_history)(int x, int y, char history[MAX_HISTORY_SIZE][3 * MAX_STR_LEN], int history_sizes[MAX_HISTORY_SIZE], int history_count);

#endif /*_DRAW_SCREENS_H_*/
