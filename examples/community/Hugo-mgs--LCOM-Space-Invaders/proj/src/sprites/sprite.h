#pragma once

#include <lcom/lcf.h>

#include "../controllers/video/video.h"

#include "../img/mouse/cursor.xpm"
#include "../img/airship/airship.xpm"
#include "../img/airship/airship_projectile.xpm"
#include "../img/aliens/alien1.xpm"
#include "../img/aliens/alien2.xpm"
#include "../img/aliens/alien3.xpm"
#include "../img/aliens/alien_explosion.xpm"
#include "../img/barrier/stage0.xpm"
#include "../img/barrier/stage1.xpm"
#include "../img/barrier/stage2.xpm"
#include "../img/barrier/stage3.xpm"
#include "../img/aliens/alien_shoot.xpm"
#include "../img/menu/play.xpm"
#include "../img/menu/play_highlight.xpm"
#include "../img/menu/leaderboard.xpm"
#include "../img/menu/leaderboard_highlight.xpm"
#include "../img/menu/exit.xpm"
#include "../img/menu/exit_highlight.xpm"
#include "../img/menu/logo.xpm"
#include "../img/symbols/heart_life.xpm"
#include "../img/symbols/heart_nolife.xpm"
#include "../img/symbols/n0.xpm"
#include "../img/symbols/n1.xpm"
#include "../img/symbols/n2.xpm"
#include "../img/symbols/n3.xpm"
#include "../img/symbols/n4.xpm"
#include "../img/symbols/n5.xpm"
#include "../img/symbols/n6.xpm"
#include "../img/symbols/n7.xpm"
#include "../img/symbols/n8.xpm"
#include "../img/symbols/n9.xpm"
#include "../img/symbols/score.xpm"
#include "../img/symbols/lives.xpm"
#include "../img/symbols/wave.xpm"

typedef struct {
    uint32_t *colors;
    int width;
    int height;
} Sprite;

Sprite *cursor;
Sprite *airship;
Sprite *p_projectile;
Sprite *alien1;
Sprite *alien2;
Sprite *alien3;
Sprite *alien_explosion;
Sprite *a_projectile;
Sprite *barrier_st0;
Sprite *barrier_st1;
Sprite *barrier_st2;
Sprite *barrier_st3;
Sprite *logo;
Sprite *play;
Sprite *leaderboard;
Sprite *quit;
Sprite *play_highlight;
Sprite *leaderboard_highlight;
Sprite *quit_highlight;
Sprite *heart_life;
Sprite *heart_nolife;
Sprite *number_0;
Sprite *number_1;
Sprite *number_2;
Sprite *number_3;
Sprite *number_4;
Sprite *number_5;
Sprite *number_6;
Sprite *number_7;
Sprite *number_8;
Sprite *number_9;
Sprite *score_text;
Sprite *lives_text;
Sprite *wave_text;

/**
 * Creates a sprite from an XPM map.
 *
 * @param xpm The XPM map to load the sprite from.
 * @return Pointer to the created Sprite, or NULL on failure.
 */
Sprite *create_sprite(xpm_map_t xpm);

/**
 * Draws a sprite at the specified coordinates.
 * 
 * @param sprite Pointer to the Sprite to draw.
 * @param x The x-coordinate where the sprite should be drawn.
 * @param y The y-coordinate where the sprite should be drawn.
 * @return 0 on success, non-zero on failure.
 */
int draw_sprite(Sprite *sprite, uint16_t x, uint16_t y);

/**
 * Frees the memory allocated for a sprite.
 *
 * @param sprite Pointer to the Sprite to destroy.
 */
void destroy_sprite(Sprite *sprite);

/**
 * Retrieves the sprite corresponding to a digit character.
 * 
 * @param digit The digit character.
 * @return Pointer to the corresponding digit Sprite, or NULL if invalid.
 */
Sprite *get_digit_sprite(char digit);

/**
 * Creates all menu-related sprites.
 */
void createMenuSprites();

/**
 * Destroys all menu-related sprites.
 */
void destroyMenuSprites();

/**
 * Creates all game-related sprites.
 */
void createGameSprites();

/**
 * Destroys all game-related sprites.
 */
void destroyGameSprites();

/**
 * Creates all game over screen sprites.
 */
void createGameOverSprites();

/**
 * Destroys all game over screen sprites.
 */
void destroyGameOverSprites();
