#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

/** @file video_gr.h
 * @brief file containing the function declarations to handle video graphics
 */
#include "mouse.h"
#include "timer.h"
#include "highscore.h"

/** @defgroup video_gr video_gr
 * @{
 *
 * @brief functions for outputing data to screen in graphics mode
 */

/**@{
 * @brief Graphics info
 */
typedef struct {
	char *video_mem;						/**< @brief pointer to the video memory base address */
	char *cards_buffer;						/**< @brief pointer to the cards buffer */
	char *mouse_buffer;						/**< @brief pointer to the mouse buffer */
	char *time_buffer;						/**< @brief pointer to the time buffer */
	char *menu_buffer;						/**< @brief pointer to the menu buffer */
	char *backg_buffer;						/**< @brief pointer to the backg buffer */
	unsigned h_res;							/**< @brief horizontal screen resolution in pixels */
	unsigned v_res;							/**< @brief vertical screen resolution in pixels */
	unsigned short time_x;					/**< @brief horizontal position of the time */
	unsigned short time_y;					/**< @brief vertical position of the time */
	unsigned short width;					/**< @brief width of the game cards */
	unsigned short height;					/**< @brief heights of the game cards */
	unsigned short button_width;			/**< @brief width of the game buttons */
	unsigned short button_height;			/**< @brief height of the game buttons */
	unsigned short gap;						/**< @brief horizontal gap between consecutive buttons */
	unsigned short button_gap;				/**< @brief vertical and horizonal gap between two cards */
	char ***cards;							/**< @brief array with the game cards xpms */
	char ***numbers;						/**< @brief array with the game numbers xpms */
	char ***letters;						/**< @brief array with the game letters xpms */
} Graphics;
/** @}*/

/**
 * @brief creates a new graphics struct for the game
 * @return pointer to the new graphics struct
 */
Graphics *create_graphics();

/**
 * @brief destroys a previously created graphics struct
 * @param graphics struct to be destroyed
 */
void destroy_graphics(Graphics *graphics);

/**
 * @brief Initializes the video module in graphics mode
 * 
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen, 
 *  and the number of colors
 * 
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void *vg_init(unsigned short mode);

/**
 * @brief Creates and returns a buffer of the same size as video memory map
 *
 * @return video buffer address.
 */
char *get_video_buffer();

/**
 * @brief Copies a given buffer to another buffer
 *
 * @param destination buffer that will be copied to
 * @param origin buffer that will be copied from.
 */
void copy_buffer(char *destination, char *origin);

 /**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 * 
 * @return 0 upon success, non-zero upon failure
 */
int vg_exit(void);

/**
* @brief Returns the horizontal resolution of the screen of the last initialized video mode
*
* @return h_res of the screen
*/
unsigned get_h_res();

/**
* @brief Returns the vertical resolution of the screen of the last initialized video mode
*
* @return v_res of the screen
*/
unsigned get_v_res();

/**
* @brief Returns the number of bits per pixel of the screen of the last initialized video mode
*
* @return bits_per_pixel of the screen
*/
unsigned get_bits_per_pixel();

/**
* @brief Returns the physical address of video memory if some video mode has been initialized
*
* @return video memory physical address
*/
char *get_vram_phys_addr();

/**
* @brief Draws a rectangle with specified dimensions and position on the screen
*
* @param base address of buffer where to draw
* @param x column of the top left corner of the rectangle
* @param y line of the top left corner of the rectangle
* @param width width of the rectangle
* @param height height of the rectangle
* @param color color of the rectangle
*
* @return 0 upon success, 1 upon failure
*/
int vg_draw_rectangle(char *base, unsigned short x, unsigned short y, unsigned short width, unsigned short height, unsigned long color);

/**
* @brief Fills the screen with the given color
*
* @param base address of buffer where to draw
* @param color color of the screen
*/
void vg_fill_screen(char *base, unsigned long color);

/**
* @brief Draws a line with specified dimensions endpoints on the screen
*
* @param base address of buffer where to draw
* @param xi column of the initial line point
* @param yi line of the initial line point
* @param xf column of the final line point
* @param yf line of the final line point
* @param color color of the line
*
* @return 0 upon success, 1 upon failure
*/
int vg_draw_line(char *base, unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color);

/**
 * @brief draws a frame around a game card
 * @param base address of the buffer to draw the frame
 * @param x horizontal coordinate of the top left corner of the frame
 * @param y vertical coordinate of the top left corner of the frame
 * @param width width of the frame
 * @param height height of the frame
 * @param color color of the frame
 * @return 0 if the frame fits, 1 otherwise
 */
int vg_draw_frame(char *base, unsigned short x, unsigned short y, unsigned short width, unsigned short height, unsigned long color);

/**
 * @brief removes a frame from around a game card
 * @param base address of the buffer to remove the frame
 * @param origin address of the background buffer
 * @param x horizontal coordinate of the top left corner of the frame
 * @param y vertical coordinate of the top left corner of the frame
 * @param width width of the frame
 * @param height height of the frame
 * @return 0 if the frame fits, 1 otherwise
 */
int vg_remove_frame(char *base, char *origin, unsigned short x, unsigned short y, unsigned short width, unsigned short height);

/**
* @brief Draws a pixmap on the screen
*
* @param base address of buffer where to draw
* @param xi column of the top left corner of the pixmap
* @param yi line of the top left corner of the pixmap
* @param pixmap image to be drawn
* @param width width of the pixmap
* @param height height of the pixmap
*
* @return 0 upon success, 1 upon failure
*/
int vg_draw_pixmap(char *base, unsigned short xi, unsigned short yi, char *pixmap, unsigned short width, unsigned short height);

/**
* @brief Draws a pixmap on the screen with black background
*
* @param base address of buffer where to draw
* @param xi column of the top left corner of the pixmap
* @param yi line of the top left corner of the pixmap
* @param pixmap image to be drawn
* @param width width of the pixmap
* @param height height of the pixmap
*
* @return 0 upon success, 1 upon failure
*/
int vg_draw_pixmap_black(char *base, unsigned short xi, unsigned short yi, char *pixmap, unsigned short width, unsigned short height);

/**
* @brief Removes a pixmap from the screen, assuming the background is black
*
* @param base address of buffer where to draw
* @param origin address of buffer with the background
* @param xi column of the top left corner of the pixmap
* @param yi line of the top left corner of the pixmap
* @param width width of the pixmap
* @param height height of the pixmap
*
* @return 0 upon success, 1 upon failure
*/
int vg_clear_pixmap(char *base, char *origin, unsigned short xi, unsigned short yi, unsigned short width, unsigned short height);

/**
 * @brief draws the mouse pointer
 * @param base buffer to draw the mouse
 * @param origin buffer with the background
 * @param mouse mouse we want to draw
 */
void vg_draw_mouse(char *base, char *origin, Mouse *mouse);

/**
 * @brief draws the game time
 * @param base buffer to draw the time
 * @param origin background buffer
 * @param timer timer we want to draw
 * @param numbers game xpm numbers
 * @param xi horizontal coordinate of the top left corner of the first digit of the time
 * @param yi vertical coordinate of the top left corner of the first digit of the time
 */
void vg_draw_time(char *base, char *origin, Timer *timer, char ***numbers, unsigned short xi, unsigned short yi);

/**
 * @brief draws a game highscore
 * @param base buffer to draw the highscore
 * @param origin background buffer
 * @param highscore highscore we want to draw
 * @param numbers game xpm numbers
 * @param letters game xpm letters
 * @param xi horizontal coordinate of the top left corner of the first digit of the highscore
 * @param yi vertical coordinate of the top left corner of the first digit of the highscore
 */
void vg_draw_highscore(char *base, char *origin, HighScore *highscore, char ***numbers, char ***letters, unsigned short xi, unsigned short yi);

/**
 * @brief draws the game background
 * @param base buffer to draw the background
 */
void vg_draw_background(char *base);

/**
 * @brief draws a random background
 * @param base buffer to draw the background
 */
void vg_draw_random_background(char *base);

/**
 * @brief draws a random squares background
 * @param base buffer to draw the background
 * @param min_number minimum number of squares to draw
 * @param max_number maximum number of squares to draw
 * @param min_side minimum side of each square
 * @param max_side maximum side of each square
 * @param height height of the background on the screen
 * @return 0 upon success, 1 upon failure
 */
unsigned short vg_draw_random_square_background(char *base, unsigned short min_number, unsigned short max_number, unsigned short min_side, unsigned short max_side, unsigned short height);

/**
 * @brief draws the game buttons
 * @param base buffer to draw the buttons
 * @param card_gap gap between consecutive cards
 * @param card_width width of a card
 * @param card_height height of a card
 * @param play_pause indicator of the game being in play or pause mode
 */
void vg_draw_buttons(char *base, unsigned short card_gap, unsigned short card_width, unsigned short card_height, char play_pause);

/**
 * @brief draws the initial game menu
 * @param base buffer to draw the menu
 * @param origin background buffer
 * @param letters xpm letters
 */
void vg_draw_menu_init(char *base, char *origin, char ***letters);

/**
 * @brief draws the new game menu
 * @param base buffer to draw the menu
 * @param origin background buffer
 * @param letters xpm letters
 */
void vg_draw_menu_new(char *base, char *origin, char ***letters);

/**
 * @brief draws the load game menu
 * @param base buffer to draw the menu
 * @param origin background buffer
 * @param numbers xpm numbers
 * @param letters xpm letters
 */
void vg_draw_menu_load_save(char *base, char *origin, char ***numbers, char ***letters);

/**
 * @brief draws the single player game menu
 * @param base buffer to draw the menu
 * @param origin background buffer
 * @param name player name
 * @param letters xpm letters
 */
void vg_draw_menu_single(char *base, char *origin, unsigned short name[5], char ***letters);

/**
 * @brief draws the hot seat game menu
 * @param base buffer to draw the menu
 * @param origin background buffer
 * @param number number of players
 * @param names player names
 * @param numbers xpm numbers
 * @param letters xpm letters
 */
void vg_draw_menu_hot(char *base, char *origin, unsigned short number, unsigned short names[9][5], char ***numbers, char ***letters);

/**
 * @brief draws the sudden death game menu
 * @param base buffer to draw the menu
 * @param origin background buffer
 * @param name player name
 * @param letters xpm letters
 */
void vg_draw_menu_sudden(char *base, char *origin, unsigned short name[5], char ***letters);

/** @} end of video_gr */
 
#endif /* __VIDEO_GR_H */
