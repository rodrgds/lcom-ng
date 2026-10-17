#ifndef SPRITE_H
#define SPRITE_H

#define TRANSPARENCY_COLOR_8_8_8_8 0xFF000000 /**< @brief Transparent color */

/**
 * @defgroup sprite
 * @{
 * @brief Functions for the sprites
 */

/**
 * @brief Struct type to safe the sprites caracteristics.
 */
typedef struct {
    int xi, yi, xf, yf; // current position
    int width, height; // dimensions
    int xspeed, yspeed; // current speed
    uint32_t *map; // the pixmap
} Sprite;

/**
 * @brief Struct type to safe the foods caracteristics.
 */
typedef struct {
    Sprite* sprite;
    bool healthy;
} Food;

/**
 * @brief This function creates a sprite with the corresponding xpm.
 * @param pic xpm with the picture.
 * @param x inicial horizontal position of the xpm.
 * @param y inicial vertical position of the xpm.
 * @param xspeed speed that the xpm moves in the horizontal.
 * @param yspeed speed that the xpm moves in the vertical.
 * @return Sprite with its respective caracteristics, NULL if invalid pixmap
 */
Sprite *create_sprite(xpm_image_t *pic, int x, int y, int xspeed, int yspeed);

/**
 * @brief This function creates the food with the corresponding xpm.
 * @param pic xpm with the picture.
 * @param x inicial horizontal position of the xpm.
 * @param y inicial vertical position of the xpm.
 * @param xspeed speed that the xpm moves in the horizontal.
 * @param yspeed speed that the xpm moves in the vertical.
 * @param healthy  
 * @return Food with its respective caracteristics.
 */
Food *create_food(xpm_image_t *pic, int x, int y, int xspeed, int yspeed,bool healthy);

/**
 * @brief This function destroys a sprite created before.
 * @param sp sprite to be destroyed.
 */
void destroy_sprite(Sprite *sp);

/**
 * @brief This function destroys a food created before.
 * @param food food to be destoyed.
 */
void destroy_food(Food *food);

/**
 * @brief This function draws a sprite on the screen.
 * @param sprite sprite to be drawn.
 * @return Zero when done.
 */
int draw_sprite(Sprite *sprite);

/**
* @brief This function updates the screen with the movement of the sprites.
* @return Zero when done.
*/
int erase_sprite(Sprite *sprite, Sprite *background);

/** @} end of sprite */

#endif //SPRITE_H
