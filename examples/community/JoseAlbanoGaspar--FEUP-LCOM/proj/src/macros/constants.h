#ifndef _LCOM_CONST_H_
#define _LCOM_CONST_H_

#include <lcom/lcf.h>

/* ------------ MODES 115 and 14C -------------------- */

/* Arena */
#define ARENA_BACKGROUND_COLOR 0x000057FF

/* Score bar */
#define SCORE_BAR_TRIM_COLOR 0x00D85E18
#define SCORE_BAR_COLOR 0x00525252
#define SCORE_COLOR 0x000000

/* Snake */
#define SNAKE_COLOR 0x002ADF23 
#define SNAKE_HEAD_COLOR 0x00179B12
#define SNAKE_CONTOUR_COLOR 0x0018AB18
#define SNAKE_EYE_COLOR 0x00090B09

/* Mouse cursor */
#define MOUSE_CURSOR_COLOR 0x0008F300

/* Apple */
#define APPLE_COLOR 0x00DC1717

/* Enemy */
#define ENEMY_COLOR 0x009A13B5
#define ENEMY_EYE_COLOR 0x00000000
#define DEATH_COLOR 0x00FFFFFF

/* Menu Colors */

#define MENU_BACKGORUND 0x00FFFAFA
#define MENU_OPTION 0x00FF0000


/* ---------------------------------------------------- */

/* ------------------ MODES 105 ----------------------- */

/* Arena */
#define ARENA_BACKGROUND_COLOR_105 0x19 //0x19 //01 also looks good

/* Score bar */
#define SCORE_BAR_TRIM_COLOR_105 0x03
#define SCORE_BAR_COLOR_105 0x07 //38 07
#define SCORE_COLOR_105 0x00

/* Snake */
#define SNAKE_COLOR_105 0x12 //0x12 22
#define SNAKE_HEAD_COLOR_105 0x2A //10 10
#define SNAKE_CONTOUR_COLOR_105 0x2A //10
#define SNAKE_EYE_COLOR_105 0x00

/* Mouse cursor */
#define MOUSE_CURSOR_COLOR_105 0x32

/* Apple */
#define APPLE_COLOR_105 0x24 //04  

/* Enemy */
#define ENEMY_COLOR_105 0x21
#define ENEMY_EYE_COLOR_105 0x00
#define DEATH_COLOR_105 0x3F

/* Menu Colors */

#define MENU_BACKGORUND_105 0x3f
#define MENU_OPTION_105 0x24

/* ---------------------------------------------------- */

/* ------------------ MODES 110 ----------------------- */
// 5 - 5 - 5              0  101 01 | 00 000 | 1 0101
/* Arena */
#define ARENA_BACKGROUND_COLOR_110 0x0057

/* Score bar */
#define SCORE_BAR_TRIM_COLOR_110 0x3CC0
#define SCORE_BAR_COLOR_110 0x3DEF
#define SCORE_COLOR_110 0x0000

/* Snake */
#define SNAKE_COLOR_110 0x02E0 
#define SNAKE_HEAD_COLOR_110 0x0140
#define SNAKE_CONTOUR_COLOR_110 0x0140
#define SNAKE_EYE_COLOR_110 0x0000

/* Mouse cursor */
#define MOUSE_CURSOR_COLOR_110 0x03E0

/* Apple */
#define APPLE_COLOR_110 0x7C00

/* Enemy */
#define ENEMY_COLOR_110 0x441F //0x3C13
#define ENEMY_EYE_COLOR_110 0x0000
#define DEATH_COLOR_110 0xFFFF

/* Menu Colors */

#define MENU_BACKGORUND_110 0x7FFF
#define MENU_OPTION_110 0x7c00

/* ---------------------------------------------------- */

/* ------------------ MODES 11A ----------------------- */
// 5 - 6 - 5 
               //0111 1 | 011 111 | 0 1111
/* Arena */
#define ARENA_BACKGROUND_COLOR_11A 0x011F  //0x011F

/* Score bar */
#define SCORE_BAR_TRIM_COLOR_11A 0x38E3
#define SCORE_BAR_COLOR_11A 0x7BCF   //0x4189
#define SCORE_COLOR_11A 0x0000

/* Snake */
#define SNAKE_COLOR_11A 0x07C0 
#define SNAKE_HEAD_COLOR_11A 0x03E0
#define SNAKE_CONTOUR_COLOR_11A 0x03E0
#define SNAKE_EYE_COLOR_11A 0x0000

/* Mouse cursor */
#define MOUSE_CURSOR_COLOR_11A 0x07E0

/* Apple */
#define APPLE_COLOR_11A 0xF800

/* Enemy */
#define ENEMY_COLOR_11A 0xB015
#define ENEMY_EYE_COLOR_11A 0x0000
#define DEATH_COLOR_11A 0xFFFF

/* Menu Colors */

#define MENU_BACKGORUND_11A 0xffff
#define MENU_OPTION_11A 0xf800

/* ---------------------------------------------------- */
/* ----------------MENU SIZE BOXES--------------------- */

#define TITLE_WIDTH 400
#define OPTIONS_WIDTH 375

/* ---------------------------------------------------- */
/* --------------GAMEOVER SIZE PIXMAPS----------------- */

#define GAMEOVER_WIDTH 234
#define GAME_O_BACK_1_WIDTH 246
#define GAME_O_BACK_2_WIDTH 360

/* ---------------------------------------------------- */
/* --------------------NUMBER SIZES-------------------- */

#define NUMBER_WIDTH 26
#define NUMBER_HEIGHT 15

/* ---------------------------------------------------- */
/* --------------------SCORE  SIZES-------------------- */

#define SCORE_WIDTH 154
#define SCORE_HEIGHT 14




#define MODEINFO_SIZE		256

#endif /* _LCOM_CONST_H */


