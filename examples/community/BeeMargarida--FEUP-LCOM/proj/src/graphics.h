#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <minix/sysutil.h>
#include <minix/com.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include "read_xpm.h"
#include "rtc.h"


#define VBE_MODE        0x4F02
#define LINEAR_FRAME    0x105
#define LAST_VBE_MODE   0xFFFF


/** @defgroup graphics graphics
 * @{
 *
 * Functions that handle the graphic part of the project
 */

/**@brief  VRAM memory mapped */
static char *video_mem;
/**@brief Buffer for double buffering */
static char *buffer;

/**
 * @brief Struct that keeps information about an option of the menu
 */
typedef struct{
	unsigned short xi; /** < x of the upper-left corner of the rectangle  */
	unsigned short yi; /** < y of the upper-left corner of the rectangle  */
	int wd; /** < width of the pixmap */
	int ht; /** < height of the pixmap */

	char *pix; /** < pixmap */

}  Rectangle;

/**
 * @brief Struct that keeps information about the cursor of the menu controlled by the mouse
 */
typedef struct{

	unsigned short x; /** < x of the upper-left corner of the cursor */
	unsigned short y; /** < y of the upper-left corner of the cursor */
	unsigned short size; /** < size of the cursor's sides */

	int left; /** < keeps information about the state of the left button of the mouse */
	unsigned int color; /** < cursor's color if the left button isn't pressed */
	unsigned int color_p; /** < cursor's color if the left button is pressed */

} Cursor;

/**
 * @brief Struct that keeps information about the makers of the game
 */
typedef struct {

	int wd; /** < width of the footer */
	int ht; /** < height of the footer*/
	short xa; /** < x of the upper-left corner of the footer */
	short ya; /** < y of the upper-left corner of the footer */
	char *pix; /** < pixmap */
	short speed; /** < distance that moves in each interruption */
}Footer;

/**
 * @brief Struct that keeps information about the menu
 */
typedef struct{
	unsigned short xtitulo; /** < x of the upper-left corner of the title */
	unsigned short ytitulo; /** < y of the upper-left corner of the title */
	int wd; /** < width of the title */
	int ht; /** < height of the title */
	char *pix; /** < pixmap of the title */

	Cursor *cursor; /** < cursor of the game */

	Rectangle *Opcao1; /** < first option of the menu [PLAY] */
	Rectangle *Opcao2; /** < second option of the menu [EXIT] */

	Footer *footer; /** < footer of the menu */

	int checkOpcao1; /** < checks if option 1 of the menu has been selected */
	int checkOpcao2; /** < checks if option 2 of the menu has been selected */
} Menu;

/**
 * @brief Struct that keeps information about a bar of the game, controlled by one of the players
 */
typedef struct{

	short wd; /** < width of the bar */
	short ht; /** < height of the bar */

	short xa; /** < x of the upper-left corner of the bar */
	short ya; /** < y of the upper-left corner of the bar */
	short speed; /** distance that the bar moves in each interruption from the timer */

	int press; /** < checks if one of the keys to move the bar is pressed */
	unsigned long scancode; /** < scancode of the input from the kbd */

	unsigned int score; /** < game score of the player in control of the bar */

} Bar;

/**
 * @brief Struct that keeps information about ball of the game
 */
typedef struct {

	short side; /** < size of the side of the ball */

	short x; /** < x of the upper-left corner of the ball */
	short y; /** < y of the upper-left corner of the ball */

	short speed; /** < distance that the ball moves in each interruption from the timer */
	short angle; /** < angle of the trajectory of the ball */
	short dir; /** < direction of the ball, in horizontal [0 - left, 1 - right] */
	short vert; /** < direction of the ball, in vertical [0 - down, 1 - up] */

	unsigned int flag_score; /** < is 1 if one of the players scored */

}Ball;

/**
 * @brief Initializes the video module in graphics mode
 *
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen,
 *  and the number of colors
 *
 * @param mode 16-bit VBE mode to set
 * @return 0 if success, 1 if not success
 */
char *vg_init(unsigned short mode);
/**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 *
 * @return 0 upon success, non-zero upon failure
 */
int vg_exit();
/**
 * @brief Copies the buffer to the virtual address VRAM was mapped to
 *
 */
void actVideoMem();
/**
 * @brief Shows all the elements of the Menu
 *
 * Shows the title of the game, 2 options (Play and Exit), the name of the class and the footer with the names of the developers
 *
 * @param menu is a struct with all the information about the menu
 */
void menu_show(Menu *menu);
/**
 * @brief Shows all the elements of the Game
 *
 * Shows the bars, the ball, the score, the time, the separator between game and score. Only called when the game starts, right after the
 * option Play in the menu is choosen
 *
 * @param b1 is a struct with the information of one of the 2 bars of the game, this one on the left
 * @param b2 is a struct with the information of one of the 2 bars of the game, this one on the right
 * @param a is a struct with the information of the ball's game
 */
void game_show(Bar *b1, Bar *b2, Ball *a);
/**
 * @brief Paints a chosen color in the buffer
 *
 * In a chosen coordinate of x and y, paints a chosen color
 *
 * @param x horizontal coordinate
 * @param y vertical coordinate
 * @param color that will be used to paint the pixel
 */
void painter(unsigned short x, unsigned short y, unsigned long color);
/**
 * @brief Paints a chosen pixmap in the buffer
 *
 * In a chosen coordinate of xi and yi, paints a chosen pixmap of width wd and heigth ht
 *
 * @param xi horizontal coordinate of the upper-left corner of the pixmap
 * @param yi vertical coordinate of the upper-left corner of the pixmap
 * @param wd is the width of the pixmap
 * @param ht is the height of the pixmap
 * @param pix is the pixmap to be painted
 */
void test_xpm_show(int xi, int yi, int wd, int ht, char *pix);
/**
 * @brief Paints pixmaps of numbers correspondent with a given number in the buffer
 *
 * In a chosen coordinate of x and y, paints a chosen pixmap of the number a. If the number is bigger than 10, the unity's number
 * will start to be painted in the coordinate x1. This method uses the test_xom_show() function
 *
 * @param a number to be shown as pixmaps
 * @param x horizontal coordinate of the upper-left corner of the pixmap of the number to be painted
 * @param y vertical coordinate of the upper-left corner of the pixmap of the number to be painted
 * @param x1 horizontal coordinate of the upper-left corner of the pixmap of the unity's number to be painted, if the number >10
 */
void show_stuff(int a, int x, int y, int x1);
/**
 * @brief Paints the pixmap's of a given date and hour in the buffer
 *
 * Paints the pixmaps of the Date given, using the test_xpm_show and show_stuff functions
 *
 * @param d is a struct that contains the current day, month, year, hours, minutes and seconds.
 */
void show_date_and_hours(Date *d);
/**
 * @brief Paints the pixmap's of a given date in the buffer
 *
 * Paints the pixmaps of the Date given, using the test_xpm_show and show_stuff functions
 *
 * @param d is a struct that contains the current day, month and year
 */
void show_date(Date *d);
/**
 * @brief Paints the pixmap's of a given time in the buffer
 *
 * Paints the pixmaps of the time given, using the test_xpm_show function
 *
 * @param time is the time to be shown with pixmaps
 */
void show_time(int time);
/**
 * @brief Paints the pixmap's of the score of the game on the buffer
 *
 * Paints both bars score's pixmaps
 *
 * @param b1 is a struct that contains information about the bar on the left, in specific it's score
 * @param b2 is a struct that contains information about the bar on the right, in specific it's score
 */
void show_score(Bar *b1, Bar *b2);
/**
 * @brief Paints a rectangle in the buffer
 *
 * In a chosen coordinate of xi and yi, paints a rectangle of color "color" and of size "sizex per sizey"
 *
 * @param x horizontal coordinate of the upper-left corner of the rectangle
 * @param y vertical coordinate of the upper-left corner of the rectangle
 * @param sizex is the width of the rectangle
 * @param sizey is the height of the rectangle
 * @param color is the color to be painted
 */
void test_rectangle_draw(unsigned short x, unsigned short y, unsigned short sizex, unsigned short sizey, unsigned long color);
/**
 * @brief Paints the bar in the buffer
 *
 * Paints the bar given, using the painter function
 *
 * @param b1 is a struct that contains information about the bar, including it's current position.
 */
void paint_bar(Bar *b1);
/**
 * @brief Paints the ball in the buffer
 *
 * Paints the ball given, using the painter function
 *
 * @param a is a struct that contains information about the ball, including it's current position.
 */
void paint_ball(Ball *a);
/**
 * @brief Paints the footer in the buffer
 *
 * @param r is a struct that contains information about the footer, including it's current position.
 */
void paint_footer(Footer *r);
/**
 * @brief Paints the buffer in black
 *
 * Paints the buffer, in black, "clearing" the screen
 */
void clear_screen();
/**
 * @brief Paints the footer in black, in the buffer
 *
 * Paints the current position of the footer in black, "clearing" the footer because it will be moved of position.
 *
 * @param r is a struct that contains information about the footer, including it's current position.
 */
void clear_footer(Footer *r);
/**
 * @brief Paints the bar in black, in the buffer
 *
 * Paints the current position of the bar in black, "clearing" the bar because it will be moved of position.
 *
 * @param b1 is a struct that contains information about the bar on the left, including it's current position.
 * @param b2 is a struct that contains information about the bar on the right, including it's current position.
 * @param x chooses which of the bars will be cleared, 0 is the left, 1 is the right
 */
void clear_bar(Bar *b1, Bar *b2, int x);
/**
 * @brief Paints the ball in black, in the buffer
 *
 * Paints the current position of the ball in black, "clearing" the ball because it will be moved of position.
 *
 * @param a is a struct that contains information about the ball, including it's current position.
 */
void clear_ball(Ball *a);
/**
 * @brief Paints the cursor in black, in the buffer
 *
 * Paints the current position of the cursor in black, "clearing" the cursor because it will be moved of position.
 *
 * @param c is a struct that contains information about the cursor, including it's current position.
 */
void clear_cursor(Cursor *c);

#endif
