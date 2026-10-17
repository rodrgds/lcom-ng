#include <stdio.h>
#include <stdlib.h>
#include "Bitmap.h"



#define SUDOKU 0x117
#define ONE 0x82
#define TWO 0x83
#define THREE 0x84
#define FOUR 0x85
#define FIVE 0x86
#define SIX 0x87
#define SEVEN 0x88
#define EIGHT 0x89
#define NINE 0x8a
#define ZERO 0x8b
#define BACKSPACE 0x8e


typedef struct{
	int available;
	unsigned short value;
	int x;
	int y;
	int hovered;
}Square;

typedef struct{
	Square board[3][3][9]; //[x][y][square]
	unsigned char * background;
	int time;
}Sudoku;

typedef struct{
	short x;
	short y;
	short x_max;
	short y_max;
	int lb;
	short h;
	short w;
} Mouse;


typedef struct{
	Bitmap*background;
	Bitmap*mouse;
	Bitmap*play;
	Bitmap*play_glow;
	Bitmap*quit;
	Bitmap*quit_glow;
	Bitmap*easy;
	Bitmap*easy_glow;
	Bitmap*medium;
	Bitmap*medium_glow;
	Bitmap*hard;
	Bitmap*hard_glow;
	Bitmap*back;
	Bitmap*back_glow;
	Bitmap*game_background;
	Bitmap*board;
	Bitmap*square;
	Bitmap*zero;
	Bitmap*one;
	Bitmap*two;
	Bitmap*three;
	Bitmap*four;
	Bitmap*five;
	Bitmap*six;
	Bitmap*seven;
	Bitmap*eight;
	Bitmap*nine;
	Bitmap*one_glow;
	Bitmap*two_glow;
	Bitmap*three_glow;
	Bitmap*four_glow;
	Bitmap*five_glow;
	Bitmap*six_glow;
	Bitmap*seven_glow;
	Bitmap*eight_glow;
	Bitmap*nine_glow;
	Bitmap*dots;
	Bitmap*time;
	Bitmap*winmenu;
	Bitmap*slash;
	Bitmap*highscores;
} Bmps;


typedef struct{
	Mouse mouse;
	Bmps bmps;
	Sudoku sudoku;
	unsigned long irq_set_mouse;
	unsigned long irq_set_timer;
	unsigned long irq_set_kb;
	unsigned long irq_set_rtc;
} Game;


