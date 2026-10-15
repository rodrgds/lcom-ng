#ifndef SPRITE_H
#define SPRITE_H

/**
 * @file sprite.h
 * @brief Sprite management functions, including creation,
 * destruction, and rendering to the screen buffer.
 */

#include <lcom/lcf.h>
#include <stdint.h>
#include <lcom/xpm.h>
#include "../view/graphics.h"

// Game element sprites
#include "../images/mouse_cursor.xpm"
#include "../images/gameElements/closed_cell.xpm"
#include "../images/gameElements/open_cell.xpm"
#include "../images/gameElements/flagged_cell.xpm"
#include "../images/gameElements/bomb_cell.xpm"
#include "../images/gameElements/exploded_cell.xpm"
#include "../images/gameElements/cell_1.xpm"
#include "../images/gameElements/cell_2.xpm"
#include "../images/gameElements/cell_3.xpm"
#include "../images/gameElements/cell_4.xpm"
#include "../images/gameElements/cell_5.xpm"
#include "../images/gameElements/cell_6.xpm"
#include "../images/gameElements/cell_7.xpm"
#include "../images/gameElements/cell_8.xpm"
#include "../images/gameElements/game_over.xpm"
#include "../images/gameElements/game_win.xpm"
#include "../images/numbers/number_0.xpm"
#include "../images/numbers/number_1.xpm"
#include "../images/numbers/number_2.xpm"
#include "../images/numbers/number_3.xpm"
#include "../images/numbers/number_4.xpm"
#include "../images/numbers/number_5.xpm"
#include "../images/numbers/number_6.xpm"
#include "../images/numbers/number_7.xpm"
#include "../images/numbers/number_8.xpm"
#include "../images/numbers/number_9.xpm"
#include "../images/gameElements/bomb.xpm"
#include "../images/gameElements/game_background.xpm"
#include "../images/gameElements/lamp.xpm"
#include "../images/gameElements/lamp_off.xpm"
#include "../images/numbers/colon.xpm"
#include "../images/gameElements/big_border.xpm"
#include "../images/gameElements/medium_border.xpm"
#include "../images/gameElements/small_border.xpm"
#include "../images/gameElements/timer_border.xpm"

// Menu element sprites
#include "../images/menuElements/menu_background.xpm"
#include "../images/menuElements/start_button.xpm"
#include "../images/menuElements/start_button_hover.xpm"
#include "../images/menuElements/left_arrow.xpm"
#include "../images/menuElements/left_arrow_hover.xpm"
#include "../images/menuElements/right_arrow.xpm"
#include "../images/menuElements/right_arrow_hover.xpm"
#include "../images/menuElements/normal_button.xpm"
#include "../images/menuElements/deceiver_button.xpm"
#include "../images/menuElements/easy_button.xpm"
#include "../images/menuElements/medium_button.xpm"
#include "../images/menuElements/hard_button.xpm"
#include "../images/menuElements/exit_button.xpm"
#include "../images/menuElements/exit_button_hover.xpm"
#include "../images/gameElements/leave.xpm"

/**
 * @brief Structure representing a graphical sprite.
 */
typedef struct {
    int x, y;               /**< Current position of the sprite (top-left corner). */
    int xspeed, yspeed;     /**< Speed of movement per frame in x and y directions. */
    int width, height;      /**< Width and height of the sprite in pixels. */
    uint8_t *map;           /**< Pixel map of the sprite (indexed color). */
} Sprite;

/**
 * @brief Creates and initializes a new sprite from an XPM image.
 * 
 * Loads the image from the provided XPM array, initializes the sprite with
 * the given position and speed, and returns a pointer to the created sprite.
 * The sprite is not drawn on the screen upon creation.
 * 
 * @param pic XPM image data.
 * @param x Initial x position.
 * @param y Initial y position.
 * @param xspeed Initial speed in the x direction.
 * @param yspeed Initial speed in the y direction.
 * @return Pointer to the created Sprite, or NULL on failure.
 */
Sprite *create_sprite(const char *pic[], int x, int y, int xspeed, int yspeed);

/**
 * @brief Destroys a sprite and frees its allocated memory.
 * 
 * @param sp Pointer to the Sprite to destroy.
 */
void destroy_sprite(Sprite *sp);

/**
 * @brief Draws a sprite to the provided video memory buffer.
 * 
 * @param sp Pointer to the Sprite to draw.
 * @param base Pointer to the base video memory where the sprite will be drawn.
 * @return 0 on success, non-zero otherwise.
 */
int (draw_sprite)(Sprite *sp, uint8_t *base);

#endif /* SPRITE_H */

