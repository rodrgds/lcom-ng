#ifndef _LCOM_SPRITE_H_
#define _LCOM_SPRITE_H_

/**
 * @file sprite.h
 * 
 * @brief This file contains the sprite's functions.
*/


/**
 * @struct Sprite
 * @brief Struct that has information about a Sprite
*/
typedef struct Sprite{
    uint16_t height, width;
    uint32_t *colors;
    int x;
    int y;
    enum xpm_image_type type;
} Sprite; 

#include "../macros.h"

#include "assets/images/darkMain.xpm"
#include "assets/images/lightMain.xpm"
#include "assets/images/sunsetMain.xpm"
#include "assets/title.xpm"
#include "assets/board.xpm"

#include "assets/images/helpMenu.xpm"

#include "assets/cursor/cursor.xpm"
#include "assets/cursor/hand_cursor.xpm"
#include "assets/cursor/crosshair.xpm"

#include "assets/buttons/button_local.xpm"
#include "assets/buttons/button_help.xpm"
#include "assets/buttons/button_quit.xpm"
#include "assets/buttons/button_next.xpm"
#include "assets/buttons/button_remote.xpm"

#include "assets/boats/boat_2p_w.xpm"
#include "assets/boats/boat_3p_w.xpm"
#include "assets/boats/boat_4p_w.xpm"
#include "assets/boats/boat_5p_w.xpm"
#include "assets/boats/boat_2p_b.xpm"
#include "assets/boats/boat_3p_b.xpm"
#include "assets/boats/boat_4p_b.xpm"
#include "assets/boats/boat_5p_b.xpm"

#include "assets/boats/boat_2p_w_V.xpm"
#include "assets/boats/boat_3p_w_V.xpm"
#include "assets/boats/boat_4p_w_V.xpm"
#include "assets/boats/boat_5p_w_V.xpm"
#include "assets/boats/boat_2p_b_V.xpm"
#include "assets/boats/boat_3p_b_V.xpm"
#include "assets/boats/boat_4p_b_V.xpm"
#include "assets/boats/boat_5p_b_V.xpm"

#include "assets/popups/sure_quit_text.xpm"
#include "assets/popups/sure_next_player.xpm"
#include "assets/popups/sure_go_game.xpm"
#include "assets/popups/player1.xpm"
#include "assets/popups/player2.xpm"
#include "assets/popups/player1_won.xpm"
#include "assets/popups/player2_won.xpm"
#include "assets/popups/waiting1.xpm"
#include "assets/popups/myTurn.xpm"
#include "assets/choosing.xpm"

#include "assets/game_board.xpm"

#include "assets/small_boats/small_boat_2p_b.xpm"
#include "assets/small_boats/small_boat_3p_b.xpm"
#include "assets/small_boats/small_boat_4p_b.xpm"
#include "assets/small_boats/small_boat_5p_b.xpm"

#include "assets/small_boats/small_boat_2p_b_V.xpm"
#include "assets/small_boats/small_boat_3p_b_V.xpm"
#include "assets/small_boats/small_boat_4p_b_V.xpm"
#include "assets/small_boats/small_boat_5p_b_V.xpm"

#include "assets/numbers/zero.xpm"
#include "assets/numbers/one.xpm"
#include "assets/numbers/two.xpm"
#include "assets/numbers/three.xpm"
#include "assets/numbers/four.xpm"
#include "assets/numbers/five.xpm"
#include "assets/numbers/six.xpm"
#include "assets/numbers/seven.xpm"
#include "assets/numbers/eight.xpm"
#include "assets/numbers/nine.xpm"
#include "assets/numbers/ten.xpm"

#include "../video_graphics/graphics.h"

/**
 * @brief Create a sprite object.
 * 
 * @param sprite : Strite.
 * @param x : The initial X position of the sprite.
 * @param y : The initial Y position of the sprite.
 * @return Sprite : The created Sprite, or NULL if allocation fails.
 */
Sprite *create_sprite(xpm_map_t sprite, int x, int y);

/**
 * @brief Destroys a sprite object.
 * 
 * @param sp : Sprite to be destroyed.
 */
void destroy_sprite(Sprite *sp);

/**
 * @brief Loads all game sprites.
 * 
 */
void load_game_sprites();

/**
 * @brief Loads boat sprites.
 * 
 * @param x The initial X position of the boat sprites.
 * @param y The initial Y position of the boat sprites.
 */
void load_boat_sprites(int x, int y);

/**
 * @brief Loads small boat sprites.
 * 
 * @param x The initial X position of the small boat sprites.
 * @param y The initial Y position of the small boat sprites.
 */
void load_small_boat_sprites(int x, int y);

/**
 * @brief Destroys all game sprites.
 * 
 */
void destroy_game_sprites();

/**
 * @brief Draws a sprite on the screen
 * 
 * @param sp : Sprite do be drawn.
 */
void draw_sprite(Sprite *sp);


#endif
