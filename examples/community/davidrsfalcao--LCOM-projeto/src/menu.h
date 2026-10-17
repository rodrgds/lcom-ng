#ifndef __MENU_H
#define __MENU_H

#include "game.h"
#include "imgs_menu.h"
#include "mouse.h"
#include "cursor.h"
#include "game_over.h"
#include "RTC.h"

typedef struct img
{
	unsigned short x;
	unsigned short y;
	int height;
	int width;
	char **sprite;
}img;


/**
 * @brief shows the date
 */
void data_sistema();
/**
 * @brief the main menu
 * @return 0 on success or non-zero otherwise
 */
int menu();
/**
 * @brief draw the backgroud of the menu
 *
 * @param ti the background image
 */
void draw_img(img ti);

#endif
