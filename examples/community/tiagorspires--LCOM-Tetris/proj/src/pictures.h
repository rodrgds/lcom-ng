#pragma once

#include <stddef.h>

// needs the typedef xpm_row_t
#include "lcom/xpm.h"

/** @defgroup xpm xpm
 * @{
 *  @author Joao Cardoso (jcard@fe.up.pt)
 */

/** Format of a xpm-like (there is a real xpm format) pic:
 * <pre>
 * static xpm_row_t const picture_name[] = {
 * "number_of_x_pixels number_of_y_pixels number_of_colors",
 * "one_char_symbol the_char_symbol_color",
 * ... exactly number_of_colors lines as above
 * "any of the above one_char_symbol, exactly number_of_x_pixels times",
 * ... exactly number_of_y_pixels lines as above
 * };
 *
 * Example:
 *
 * static xpm_row_t const pic1[] = {   // the name of the picture, "pic1" in this case
 * "32 13 4",                          // number of pixels in the x and y dimension,
 *                                        and the number of colors, which follows.
 * ". 0",                              // mapping between symbols and colors; in this
 *                                        case, the "." will be mapped to color 0
 * "x 2",                              // and the "x" to color 2
 * ...                                 // the next symbol/color pairs
 * "................................", // first row, exactly 32 pixels, 
 *                                        all color 0, BLACK
 * "..............xxx...............", // next row has three GREEN pixels
 *                                        at the center
 * </pre>
 * Any real xpm picture can be read, as long as there are only 16
 * colors on it, and the colors are the ones available by default.
 *
 *  To use other/more colors than the basic ones, the graphic card
 *  palette must be programmed.
 * 
 */

  static xpm_row_t const P[] = {
  "7 9 2",
  "  0",
  "1 1",
  "111111 ",
  "1111111",
  "11   11",
  "1111111",
  "111111 ",
  "11     ",
  "11     ",
  "11     ",
  "       "
};

static xpm_row_t const L[] = {
  "7 9 2",
  "  0",
  "1 1",
  "11     ",
  "11     ",
  "11     ",
  "11     ",
  "11     ",
  "11     ",
  "1111111",
  "1111111",
  "       "
};

static xpm_row_t const A[] = {
  "7 9 2",
  "  0",
  "1 1",
  "   1   ",
  "  111  ",
  " 1   1 ",
  "1111111",
  "1111111",
  "1     1",
  "1     1",
  "1     1",
  "       "
};

static xpm_row_t const Y[] = {
  "7 9 2",
  "  0",
  "1 1",
  "1     1",
  " 1   1 ",
  "  1 1  ",
  "   1   ",
  "   1   ",
  "   1   ",
  "   1   ",
  "   1   ",
  "       "
};

static xpm_row_t const E[] = {
  "7 9 2",
  "  0",
  "1 1",
  "1111111",
  "1111111",
  "11     ",
  "111111 ",
  "111111 ",
  "11     ",
  "1111111",
  "1111111",
  "       "
};

static xpm_row_t const X[] = {
  "7 9 2",
  "  0",
  "1 1",
  "1     1",
  " 1   1 ",
  "  1 1  ",
  "   1   ",
  "  1 1  ",
  " 1   1 ",
  "1     1",
  "1     1",
  "       "
};

static xpm_row_t const I[] = {
  "7 9 2",
  "  0",
  "1 1",
  " 11111 ",
  "   1   ",
  "   1   ",
  "   1   ",
  "   1   ",
  "   1   ",
  "   1   ",
  " 11111 ",
  "       "
};

static xpm_row_t const T[] = {
  "7 9 2",
  "  0",
  "1 1",
  "1111111",
  "1111111",
  "   1   ",
  "   1   ",
  "   1   ",
  "   1   ",
  "   1   ",
  "   1   ",
  "       "
};

static xpm_row_t const R[] = {
  "7 9 2",
  "  0",
  "1 1",
  "111111 ",
  "1111111",
  "11   11",
  "1111111",
  "11111  ",
  "11  11 ",
  "11   11",
  "11    1",
  "       "
};

static xpm_row_t const S[] = {
  "7 9 2",
  "  0",
  "1 1",
  " 11111 ",
  "1111111",
  "11     ",
  " 11111 ",
  "     11",
  "11    1",
  "1111111",
  " 11111 ",
  "       "
};


static xpm_row_t const mouse_cursor[] = {
  "30 43 3",
  "  0",
  ". 1",
  "X 2",
  "  X                           ",
  " XX                           ",
  " X.X                          ",
  " X..X                         ",
  " X...X                        ",
  " X....X                       ",
  " X.....X                      ",
  " X......X                     ",
  " X.......X                    ",
  " X........X                   ",
  " X.........X                  ",
  " X..........X                 ",
  " X...........X                ",
  " X............X               ",
  " X.............X              ",
  " X..............X             ",
  " X...............X            ",
  " X................X           ",
  " X.................X          ",
  " X..................X         ",
  " X...................X        ",
  " X....................X       ",
  " X.....................X      ",
  " X......................X     ",
  " X.......................X    ",
  " X........................X   ",
  " X.........................X  ",
  " X..........................X ",
  " X...............XXXXXXXXXXXX ",
  " X.......X......X             ",
  " X......XX......X             ",
  " X.....X XX.....X             ",
  " X....X   X.....X             ",
  " X...X    X.....X             ",
  " X..X     X.....X             ",
  " X.X      X.....X             ",
  " XX       X.....X             ",
  "          X.....X             ",
  "          X.....X             ",
  "          X.....X             ",
  "          X.....X             ",
  "           XXXXXX             ",
  "                              ",
};

