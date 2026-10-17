#ifndef __VIDEO_TXT_H
#define __VIDEO_TXT_H

static unsigned scr_width;	/* Width of screen in columns */
static unsigned scr_lines;

#include "vt_info.h"
/** @defgroup video_txt video_txt
 * @{
 *
 * Functions for outputing data to screen in text mode
 */

/**
 * @brief Initializes the video module in text mode
 * 
 * Maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen
 * 
 * @param vi_p address of vt_info_t structure with the necessary information
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
char *vt_init(vt_info_t *vi_p);

/**
 * @brief Fills the screen with the input character, with the specified attribute
 * 
 * @param ch character to fill the screen with
 * @param attr attribute of character to fill the screen with
 */
void vt_fill(char ch, char attr);

/**
 * @brief Blanks screen
 * 
 */
void vt_blank(void);

/**
 * @brief Prints a character on the screen
 * 
 * Prints a character, with the specified attribute, at the specified position 
 * in the screen in text mode, by writing to the corresponding location in VRAM
 * 
 * @param ch character to print 
 * @param r row where to print (row 0 is the first one)
 * @param c column where to print (column 0 is the first one)
 * @param attr attribute of character to print
 * @return 0 upon success, non-zero upon failure
 */
int vt_print_char(char ch, int r, int c, char attr);

/**
 * @brief Prints a string on the screen
 * 
 * Prints a string, with the specified attribute, at the specified position in 
 * the screen in text mode, by writing to the corresponding location in VRAM
 * 
 * @param str string to print 
 * @param r row where to print (row 0 is the first one)
 * @param c column where to print (column 0 is the first one)
 * @param attr attribute of character to print
 * @return 0 upon success, non-zero upon failure
 */
int vt_print_string(char *str, int r, int c, char attr);

/**
 * @brief Prints an integer on the screen
 * 
 * Prints an integer, with the specified attribute, at the specified position in the screen 
 * in text mode, by writing to the corresponding location in VRAM
 * 
 * @param n integer number to print 
 * @param r row where to print (row 0 is the first one)
 * @param c column where to print (column 0 is the first one)
 * @param attr attribute of character to print
 * @return 0 upon success, non-zero upon failure
 */
int vt_print_int(int n, int r, int c, char attr);

/**
 * @brief Draws a frame on the screen
 * 
 * Draws a frame, with the specified dimensions and attribute, at the specified position
 * in the screen in text mode, by writing to the corresponding location in VRAM
 * 
 * @param width frame width in number of columns
 * @param height frame heigth in number of lines
 * @param r row where to print top left corner (row 0 is the first one)
 * @param c column where to print top left corner (column 0 is the first one)
 * @param attr attribute of frame to draw
 * @return 0 upon success, non-zero upon failure
 */
int vt_draw_frame(int width, int height, int r, int c, char attr);

#endif /* __VIDEO_TXT_H */
