#ifndef __SPRITE_H
#define __SPRITE_H

/** @defgroup sprite sprites
 * @{
 *
 * Functions for creating and manipulating Sprites (set of pixels with a given position)
 */

typedef struct {
	int x; /**< @brief current position in the x axis */
	int y; /**< @brief current position in the x axis */
	int width, height; /**< @brief current size */
	char *map; /**< @brief the pixmap */
} Sprite;

/**
 * @brief Draws a sprite
 *
 * Draws a sprite in the secondary buffer
 *
 * @param sp Sprite being drawn
 */
void draw_sprite(Sprite *sp);
static char *read_xpm(char *map[], int *wd, int *ht);
/**
 * @brief Erases a sprite
 *
 * Fills the position of the sprite with a given color
 *
 * @param sprt Sprite being erased
 * @param background The color of the background
 */
void erase_sprite(Sprite *sprt, unsigned long background);
/**
 * @brief Draws an asteroid
 *
 * Draws an asteroid in the secondary buffer
 *
 * @param sprt Asteroid being drawn
 * @return Returns 1 on collision, 0 otherwise
 */
int draw_asteroid(Sprite *sprt);
/**
 * @brief Draws the battleship
 *
 * Draws the battleship in the secondary buffer
 *
 * @param sprt Battleship being drawn
 * @return Returns 1 on collision, 0 otherwise
 */
int draw_battleship(Sprite *sprt);

/** Creates with random speeds (not zero) and position
 * (within the screen limits), a new sprite with pixmap "pic", in
 * memory whose address is "base";
 * Returns NULL on invalid pixmap.
 */
Sprite * create_sprite(char *pic[], int x, int y);

#endif
