#ifndef _VIDEO_GR_
#define _VIDEO_GR_


#include <lcom/lcf.h>

#include "images/choose_mode.h"

#include "images/play_img.h"
#include "images/menu_background.h"
#include "images/leaderboard_img.h"
#include "images/exit_img.h"
#include "images/menu_img.h"

#include "images/background.h"

#include "images/pontuation.h"

#include "images/leaderboard.h"

#include "images/timer_numbers/number_0.h"
#include "images/timer_numbers/number_1.h"
#include "images/timer_numbers/number_2.h"
#include "images/timer_numbers/number_3.h"
#include "images/timer_numbers/number_4.h"
#include "images/timer_numbers/number_5.h"
#include "images/timer_numbers/number_6.h"
#include "images/timer_numbers/number_7.h"
#include "images/timer_numbers/number_8.h"
#include "images/timer_numbers/number_9.h"

#include "images/buttons/red_button.h"
#include "images/buttons/green_button.h"
#include "images/buttons/blue_button.h"
#include "images/buttons/yellow_button.h"
#include "images/buttons/black_button.h"
#include "images/buttons/white_button.h"

#include "images/letters/0.h"
#include "images/letters/1.h"
#include "images/letters/2.h"
#include "images/letters/3.h"
#include "images/letters/4.h"
#include "images/letters/5.h"
#include "images/letters/6.h"
#include "images/letters/7.h"
#include "images/letters/8.h"
#include "images/letters/9.h"

#include "images/letters/a.h"
#include "images/letters/b.h"
#include "images/letters/c.h"
#include "images/letters/d.h"
#include "images/letters/e.h"
#include "images/letters/f.h"
#include "images/letters/g.h"
#include "images/letters/h.h"
#include "images/letters/i.h"
#include "images/letters/j.h"
#include "images/letters/k.h"
#include "images/letters/l.h"
#include "images/letters/m.h"
#include "images/letters/n.h"
#include "images/letters/o.h"
#include "images/letters/p.h"
#include "images/letters/q.h"
#include "images/letters/r.h"
#include "images/letters/s.h"
#include "images/letters/t.h"
#include "images/letters/u.h"
#include "images/letters/v.h"
#include "images/letters/w.h"
#include "images/letters/x.h"
#include "images/letters/y.h"
#include "images/letters/z.h"

#include <math.h>

#include "Game.h"
#include "elements.h"


#define COLOR_BLACK 0x0000 /**< @brief black color */

#define IGNORE_COLOR_INDEXED 0 /**< @brief color of a pixel to be ignored when drawing a xpm image in indexed mode */
#define IGNORE_COLOR_DIRECT 0xfffff0 /**< @brief color of a pixel to be ignored when drawing a xpm image in direct mode */


unsigned int vram_sz, bits_p_pixel;
uint16_t h_res, v_res;
uint8_t* vg_ptr;
uint8_t* vg_bg_buffer;
uint8_t* vg_mouse_buffer;
uint8_t* vg_elem_buffer;
static char *video_mem;
uint8_t bytes_per_pixel;
vbe_mode_info_t vmi_p;

/**
 * function that sets the video buffer for the game calling the draw functions according to the current image state of the game
 * @param game current game whose buffer is to be set
 * @return return 0 upon success and non-zero otherwise
*/
int (vg_set_buffer)(Game* game);

/**
 * function that draws all the elements from the game buffer
 * @return return 0 upon success and non-zero otherwise
*/
int (vg_draw_buffer)();

/**
 * function that draws all the elements from the mouse buffer
 * @return return 0 upon success and non-zero otherwise
*/
int (vg_draw_mouse_buffer)();

/**
 * function that exits video graphics mode and returns to text mode
 * @return return 0 upon success and non-zero otherwise
*/
int (my_vg_exit)();

/**
 * function that draws an horizontal line in the elements buffer
 * @param x x coordinate corresponding to the start of the line to be drawn
 * @param y y coordinate corresponding to the start of the line to be drawn
 * @param len length of the line to be drawn
 * @param color color of the line to be drawn
 * @return return 0 upon success and non-zero otherwise
*/
int (vg_draw_hline_2)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * function that draws an horizontal line in the mouse buffer
 * @param x x coordinate corresponding to the start of the line to be drawn
 * @param y y coordinate corresponding to the start of the line to be drawn
 * @param len length of the line to be drawn
 * @param color color of the line to be drawn
 * @return return 0 upon success and non-zero otherwise
*/
int (vg_draw_hline_3)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * function that draws a rectangle in the background buffer recieving the address of the rectangle
 * @param r address of the rectangle to be drawn
 * @return return 0 upon success and non-zero otherwise
*/
int (draw_rectangle)(Rectangle * r);

/**
 * function that draws a rectangle in the elements buffer recieving the address of the rectangle
 * @param r address of the rectangle to be drawn
 * @return return 0 upon success and non-zero otherwise
*/
int (draw_rectangle_2)(Rectangle * r);

/**
 * function that draws the mouse rectangle in the mouse buffer
 * @param pos position where the mouse rectangle is to be drawn
 * @param width width of the mouse rectangle to be drawn
 * @param height height of the mouse rectangle to be drawn
 * @param color color of the mouse rectangle to be drawn
 * @return return 0 upon success and non-zero otherwise
*/
int (my_draw_mouse)(Position pos, uint16_t width, uint16_t height, uint32_t color);

/**
 * function that draws a rectangle in the background buffer
 * @param pos position where rectangle is to be drawn
 * @param width width of rectangle to be drawn
 * @param height height of rectangle to be drawn
 * @param color color of the rectangle to be drawn
 * @return return 0 upon success and non-zero otherwise
*/
int (my_draw_rectangle)(Position pos, uint16_t width, uint16_t height, uint32_t color);

/**
 * function that draws a rectangle in the elements buffer
 * @param pos position where rectangle is to be drawn
 * @param width width of rectangle to be drawn
 * @param height height of rectangle to be drawn
 * @param color color of the rectangle to be drawn
 * @return return 0 upon success and non-zero otherwise
*/
int (my_draw_rectangle_2)(Position pos, uint16_t width, uint16_t height, uint32_t color);

/**
 * function that draws a given number of rectangles
 * @param mode graphics mode that is running
 * @param width width of a rectangle to be drawn
 * @param height height of a rectangle to be drawn
 * @param color color of a rectangle to be drawn
 * @return return 0 upon success and non-zero otherwise
*/
int (vg_draw_n_rectangles)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step);

/**
 * function that draws a pixel line in the background buffer
 * @param x x coordinate of the pixel to be drawn
 * @param y y coordinate of the pixel to be drawn
 * @param color color of the pixel to be drawn
 * @return return 0 upon success and non-zero otherwise
*/
int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * function that draws a pixel line in the elements buffer
 * @param x x coordinate of the pixel to be drawn
 * @param y y coordinate of the pixel to be drawn
 * @param color color of the pixel to be drawn
 * @return return 0 upon success and non-zero otherwise
*/
int (draw_pixel_2)(uint16_t x, uint16_t y, uint32_t color);

/**
 * function that draws a pixel line in the mouse buffer
 * @param x x coordinate of the pixel to be drawn
 * @param y y coordinate of the pixel to be drawn
 * @param color color of the pixel to be drawn
 * @return return 0 upon success and non-zero otherwise
*/
int (draw_pixel_3)(uint16_t x, uint16_t y, uint32_t color);

/**
 * function that draws the hitbox rectangle of the game mouse
 * @param game address of the game whose mouse is to be drawn
 * @return return 0 upon success and non-zero otherwise
*/
int (draw_mouse)(Game *game);

/**
 * function that draws the game by setting its buffer and drawing the mouse button buffer
 * @param game address of the game to be drawn
 * @return return 0 upon success and non-zero otherwise
*/
int (draw_game)(Game *game);

/**
 * function that clears the background buffer
*/
void (vg_bg_clear)();

/**
 * function that draws the elements of the game accordingt to its current state
 * @param game address of the game whose elements are to be drawn
 * @return return 0 upon success and non-zero otherwise
*/
int (draw_elem)(Game * game);

/**
 * function that draws the elements of the running state of the game
 * @param game address of the game whose elements are to be drawn
 * @return return 0 upon success and non-zero otherwise
*/
int (draw_run_elem)(Game * game);

/**
 * function that draws the button corresponding to a button of the game
 * @param button address of the button to be drawn
 * @param video_mode graphics mode where the game is running
 * @return return 0 upon success and non-zero otherwise
*/
int (draw_button)(Button * button, Video_Mode video_mode);

/**
 * function that draws the elements of the menu state of the game
 * @param game address of the game whose elements are to be drawn
 * @return return 0 upon success and non-zero otherwise
*/
int (draw_menu_elem)(Game * game);

/**
 * function that draws the elements of the pontuation state of the game
 * @param game address of the game whose elements are to be drawn
 * @return return 0 upon success and non-zero otherwise
*/
int (draw_pontuation)(Game * game);

/**
 * function that draws the score of the game
 * @param game address of the game whose score is to be drawn
 * @param x x position of where the score number is to be drawn
 * @param y y position of where the score number is to be drawn
 * @param score_number actual number to be drawn
 * @return return 0 upon success and non-zero otherwise
*/
int (draw_score)(Game * game, uint16_t x, uint16_t y, uint16_t score_number);

/**
 * function that draws the elements of the leaderboard state of the game
 * @param game address of the game whose elements are to be drawn
 * @return return 0 upon success and non-zero otherwise
*/
int (draw_leaderboard)(Game * game);

/**
 * function that draws the top 3 score records of the leaderboard of the game
 * @param game address whose top 3 is to be drawn
 * @return return 0 upon success and non-zero otherwise
*/
int (draw_top_3)(Game * game);

/**
 * function that draws the background of the game accordingt to its current state
 * @param game address of the game whose background is to be drawn
 * @return return 0 upon success and non-zero otherwise
*/
int (draw_bg)(Game * game);

/**
 * function that draws the background of the menu state of the game
 * @param game address of the game whose background is to be drawn
 * @return return 0 upon success and non-zero otherwise
*/
int (draw_menu_bg)(Game * game);

/**
 * function that draws the background of the running state of the game
 * @param game address of the game whose background is to be drawn
 * @return return 0 upon success and non-zero otherwise
*/
int (draw_run_bg)(Game * game);

/**
 * function that prints the xpm images in the screen
 * @param xpm xpm image to be printed
 * @param x x position where the image will start being printed
 * @param y y position where the image will start being printed
 * @param in_bg true if the image is to be printed in the background, false otherwise
 * @param vm video graphics mode where the game is running
 * @return return 0 upon success and non-zero otherwise
*/
int (print_xpm_custom)(xpm_map_t xpm, uint16_t x, uint16_t y, bool in_bg, Video_Mode vm);

/**
 * function that draws the timer countdown numbers of the game when it is running
 * @param game address of the game whose timer countdown numbers are to be drown
 * @return return 0 upon success and non-zero otherwise
*/
int (draw_timer_number)(Game * game); //numero em segundos

/**
 * function that draws a letter given the breakcode of the key received from the keyboard
 * @param game address of the game whose graphics mode is to be checked
 * @param key breakcode of the key received from the keyboard
 * @param x address of the x position where the letter will be drawn
 * @param y y position where the letter will be drawn
 * @return return 0 upon success and non-zero otherwise
*/
int (draw_letter)(Game * game, Keyboard_Key key, uint16_t * x, uint16_t y);

/**
 * function that draws the xpm image pixel by pixel
 * @param x x position where the image will start being printed
 * @param y y position where the image will start being printed
 * @return return 0 upon success and non-zero otherwise
*/
int (draw_xpm_)(xpm_map_t xpm, int x, int y);

/**
 * function that draws a pixel
 * @param x x coordinate of the pixel to be drawn
 * @param y y coordinate of the pixel to be drawn
 * @param color color of the pixel to be drawn
 * @return return 0 upon success and non-zero otherwise
*/
int (draw_pixel_)(int x, int y, uint32_t color);


#endif /* _VIDEO_GR_ */
