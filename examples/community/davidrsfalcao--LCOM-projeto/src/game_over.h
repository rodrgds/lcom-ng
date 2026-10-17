#ifndef __GAME_OVER_H
#define __GAME_OVER_H

#include "game.h"
#include "imgs_game_over.h"
#include "numbers.h"
#include "mouse.h"
#include "cursor.h"
#include "keyboard.h"
#include "math.h"

typedef struct image
{
	unsigned short x;
	unsigned short y;
	int height;
	int width;
	char **sprite;
}image;
/**
 * @brief when a player loses the game_over menu will appear
 * @return 0 on success or 1 on non-success
 */
int game_over();
/**
 * @brief draws the image of the game_over menu
 * @param t1 image that will be draw
 */
void draw_image(image t1);
/**
 * @brief shows the pontuation that the player achieve
 */
void pontuation();
/**
 * @brief draws the options on the Game-over menu
 *
 * @param ti image of the option that will be draw
 * @param option the number that identifies the options drawn
 */
void draw_option(image ti, unsigned short option);

#endif
