#ifndef SPRITE_H
#define SPRITE_H

#include <lcom/lcf.h>
#include "../devices/video/video.h"

#include "../imgs/cursor.xpm"
#include "../imgs/background.xpm"
#include "../imgs/logo.xpm"
#include "../imgs/buttons/single_machine.xpm"
#include "../imgs/buttons/quit.xpm"
#include "../imgs/buttons/return.xpm"
#include "../imgs/buttons/undo.xpm"
#include "../imgs/clock.xpm"
#include "../imgs/board.xpm"
#include "../imgs/board_dark.xpm"
#include "../imgs/selected.xpm"
#include "../imgs/pieces/bB.xpm"
#include "../imgs/pieces/bK.xpm"
#include "../imgs/pieces/bN.xpm"
#include "../imgs/pieces/bP.xpm"
#include "../imgs/pieces/bQ.xpm"
#include "../imgs/pieces/bR.xpm"
#include "../imgs/pieces/wB.xpm"
#include "../imgs/pieces/wK.xpm"
#include "../imgs/pieces/wN.xpm"
#include "../imgs/pieces/wP.xpm"
#include "../imgs/pieces/wQ.xpm"
#include "../imgs/pieces/wR.xpm"
#include "../imgs/chars/numbers.xpm"
#include "../imgs/black_wins.xpm"
#include "../imgs/white_wins.xpm"

#define SQUARE_SIZE 108

/**
 * @brief Struct that represents a sprite
*/
typedef struct {
    uint16_t height, width;
    int x, y;  
    uint32_t *colors;
} Sprite;

Sprite *cursor;
Sprite *background;
Sprite *logo;
Sprite *single_machine;
Sprite *return_button;
Sprite *quit;
Sprite *undo;
Sprite *clock_background;
Sprite *board_img;
Sprite *board_dark_img;
Sprite *sel_img;
Sprite *bB, *bK, *bN, *bP, *bQ, *bR, *wB, *wK, *wN, *wP, *wQ, *wR;
Sprite *numbers[10];
Sprite *black_wins;
Sprite *white_wins;


/**
 * @brief Creates a sprite based on a xpm
 * @param xpm Xpm of the sprite
 * @return Returns a pointer to the created sprite
*/
Sprite *create_sprite(xpm_map_t xpm);

/**
 * @brief Destroys a sprite
 * @param sp Sprite to be destroyed
*/
void destroy_sprite(Sprite *sp);

/**
 * @brief Draws a sprite on the screen
 * @param sp Sprite to be drawn
 * @param x X coordinate of the sprite
 * @param y Y coordinate of the sprite
 * @return Returns 0 upon success and 1 otherwise
*/
int draw_sprite(Sprite *sp, int x, int y);

/**
 * @brief Loads all the sprites
*/
void load_sprites();

/**
 * @brief Dumps all the sprites
*/
void dump_sprites();

#endif
