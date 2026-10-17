#ifndef __SPACE_SHIP_H
#define __SPACE_SHIP_H

typedef struct space_ship
{
	unsigned short x;
	unsigned short y;
	int height;
	int width;
	char **sprite;


}space_ship;

/**
 *@brief moves the spaceship
 *@param nv1 the spaceship that will be moved
 *@param delta the number of pixels that the spaceship will be moved
 *@return 0 on success or non-zero otherwise
 */
int move_spaceship(space_ship *nv1, short delta);
/**
 * @brief draws a spaceship
 * @param nv1 the spaceship that will be draw
 */
void draw_spaceship(space_ship nv1);


#endif
