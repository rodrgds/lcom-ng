#ifndef _SPRITES_H_
#define _SPRITES_H_

#include <lcom/lcf.h>
#include "stdint.h"
#include "stdio.h"
#include "../controllers/video/video_gr.h"
#include "../imgs/cursor.xpm"
#include "../imgs/xpm.xpm"
#include "../imgs/titlescreen1.xpm"
#include "../imgs/titlescreen2.xpm"
#include "../imgs/daybg_1.xpm"
#include "../imgs/nightbg_1.xpm"
#include "../imgs/level1.xpm"
#include "../imgs/level2.xpm"
#include "../imgs/level3.xpm"
#include "../imgs/level4.xpm"
#include "../imgs/level5.xpm"
#include "../imgs/level6.xpm"
#include "../imgs/level7.xpm"
#include "../imgs/paused1.xpm"
#include "../imgs/paused2.xpm"
#include "../imgs/paused3.xpm"
#include "../imgs/soul_blast_powerup.xpm"
#include "../imgs/jump_powerup.xpm"
#include "../imgs/soul_blast_attack.xpm"
#include "../imgs/attack_anim.xpm"
#include "../imgs/mouse.xpm"
#include "../imgs/bully1.xpm"
#include "../imgs/crawlie1.xpm"
#include "../imgs/fly1.xpm"
#include "../imgs/flyknight1.xpm"
#include "../imgs/gunkybully1.xpm"
#include "../imgs/hpfull.xpm"
#include "../imgs/hpempty.xpm"
#include "../imgs/gameover.xpm"
#include "../imgs/boss_attack.xpm"
#include "../imgs/boss_crouch.xpm"
#include "../imgs/boss.xpm"
#include "../imgs/boss_stand.xpm"
#include "../imgs/victory.xpm"


/**
 * @brief Structure representing a sprite
 */
typedef struct Sprite{
    uint16_t height; /**< The height of the sprite. */
    uint16_t width; /**< The width of the sprite. */
    uint32_t *colors; /**< The array of colors representing the pixels of the sprite. */
} Sprite; 


/**
 * @brief Creates a sprite from an XPM file.
 *
 * @param sprite The XPM map representing the sprite.
 * @return A pointer to the created sprite.
 */
Sprite *createSprite(xpm_map_t sprite);

/**
 * @brief Draws a sprite on the screen at the specified coordinates.
 *
 * @param sp The sprite to be drawn.
 * @param x The x-coordinate of the top-left corner of the sprite.
 * @param y The y-coordinate of the top-left corner of the sprite.
 * @return 0 upon success, non-zero otherwise.
 */
int drawSprite(Sprite *sp, int x, int y);

/**
 * @brief Draws a horizontally flipped sprite on the screen at the specified coordinates.
 *
 * @param sp The sprite to be drawn.
 * @param x The x-coordinate of the top-left corner of the sprite.
 * @param y The y-coordinate of the top-left corner of the sprite.
 * @return 0 upon success, non-zero otherwise.
 */
int draw_sprite_flipped(Sprite *sp, int x, int y);

/**
 * @brief Draws a sprite upwards on the screen at the specified coordinates.
 *
 * @param sp The sprite to be drawn.
 * @param x The x-coordinate of the top-left corner of the sprite.
 * @param y The y-coordinate of the top-left corner of the sprite.
 * @return 0 upon success, non-zero otherwise.
 */
int draw_sprite_up(Sprite *sp, int x, int y);

/**
 * @brief Draws a sprite downwards on the screen at the specified coordinates.
 *
 * @param sp The sprite to be drawn.
 * @param x The x-coordinate of the top-left corner of the sprite.
 * @param y The y-coordinate of the top-left corner of the sprite.
 * @return 0 upon success, non-zero otherwise.
 */
int draw_sprite_down(Sprite *sp, int x, int y);

/**
 * @brief Frees the memory occupied by a sprite.
 *
 * @param sp The sprite to be destroyed.
 */
void destroy_sprite(Sprite *sp);

/**
 * @brief Loads all the sprites used in the program.
 */
void loadSprites();

/**
 * @brief Unloads all the sprites used in the program.
 */
void unloadSprites();

#endif
