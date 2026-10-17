#ifndef __SPRITE_H_
#define __SPRITE_H_

#include <lcom/lcf.h>
#include "video/video.h"

/**
 * @brief Structure representing a sprite
 */
typedef struct Sprite {
  uint32_t x, y;           /**< Current position */
  uint32_t width, height;  /**< Dimensions */
  int32_t xspeed, yspeed; /**< Current speed */
  uint32_t *map;               /**< The pixmap */
} Sprite;

/**
 * @brief Structure representing a sprite lookup table entry
 */
typedef struct {
    const char *name; /**< Name of the sprite */
    Sprite **sprite; /**< Pointer to the sprite */
} SpriteLookup;

// Declaration of sprite variables
Sprite *menu_background;
Sprite *create_player_background;
Sprite *choose_player_background;
Sprite *game_background;
Sprite *deal_deck;
Sprite *close_button;
Sprite *coin_sprite;
Sprite *coin_shadow_sprite;
Sprite *mouse_cursor;
Sprite *mouse_cursor_click;
Sprite *card_back;
Sprite *clubs_2;
Sprite *clubs_3;
Sprite *clubs_4;
Sprite *clubs_5;
Sprite *clubs_6;
Sprite *clubs_7;
Sprite *clubs_8;
Sprite *clubs_9;
Sprite *clubs_10;
Sprite *clubs_J;
Sprite *clubs_Q;
Sprite *clubs_K;
Sprite *clubs_A;
Sprite *diamonds_2;
Sprite *diamonds_3;
Sprite *diamonds_4;
Sprite *diamonds_5;
Sprite *diamonds_6;
Sprite *diamonds_7;
Sprite *diamonds_8;
Sprite *diamonds_9;
Sprite *diamonds_10;
Sprite *diamonds_J;
Sprite *diamonds_Q;
Sprite *diamonds_K;
Sprite *diamonds_A;
Sprite *hearts_2;
Sprite *hearts_3;
Sprite *hearts_4;
Sprite *hearts_5;
Sprite *hearts_6;
Sprite *hearts_7;
Sprite *hearts_8;
Sprite *hearts_9;
Sprite *hearts_10;
Sprite *hearts_J;
Sprite *hearts_Q;
Sprite *hearts_K;
Sprite *hearts_A;
Sprite *spades_2;
Sprite *spades_3;
Sprite *spades_4;
Sprite *spades_5;
Sprite *spades_6;
Sprite *spades_7;
Sprite *spades_8;
Sprite *spades_9;
Sprite *spades_10;
Sprite *spades_J;
Sprite *spades_Q;
Sprite *spades_K;
Sprite *spades_A;
Sprite *a;
Sprite *b;
Sprite *c;
Sprite *d;
Sprite *e;
Sprite *f;
Sprite *g;
Sprite *h;
Sprite *i;
Sprite *j;
Sprite *k;
Sprite *l;
Sprite *m;
Sprite *n;
Sprite *o;
Sprite *p;
Sprite *q;
Sprite *r;
Sprite *s;
Sprite *t;
Sprite *u;
Sprite *v;
Sprite *w;
Sprite *x;
Sprite *y;
Sprite *z;
Sprite *zero;
Sprite *one;
Sprite *two;
Sprite *three;
Sprite *four;
Sprite *five;
Sprite *six;
Sprite *seven;
Sprite *eight;
Sprite *nine;
Sprite *space;
Sprite *hyphen;
Sprite *colon;

/**
 * @brief Creates a sprite
 * @param pic Array of strings representing the pixmap
 * @param x Initial x coordinate
 * @param y Initial y coordinate
 * @param xspeed Initial horizontal speed
 * @param yspeed Initial vertical speed
 * @return Pointer to the created sprite
 */
Sprite* (create_sprite)(const char *pic[], int x, int y, int xspeed, int yspeed);

/**
 * @brief Destroys a sprite
 * @param sp Pointer to the sprite to destroy
 */
void (destroy_sprite)(Sprite *sp);

/**
 * @brief Moves a sprite to a new position
 * @param sp Pointer to the sprite
 * @param destx Destination x coordinate
 * @param desty Destination y coordinate
 */
void (move_sprite)(Sprite *sp, uint32_t destx, uint32_t desty);

/**
 * @brief Sets the position of a sprite
 * @param sp Pointer to the sprite
 * @param x New x coordinate
 * @param y New y coordinate
 */
void (set_sprite_position)(Sprite *sp, uint32_t x, uint32_t y);

/**
 * @brief Draws a sprite
 * @param sp Pointer to the sprite
 */
void (draw_sprite)(Sprite *sp);

/**
 * @brief Draws a sprite to a buffer
 * @param sp Pointer to the sprite
 * @param buffer Pointer to the buffer
 */
void (draw_sprite_to_buffer)(Sprite *sp, uint8_t *buffer);

/**
 * @brief Draws a scaled sprite
 * @param sp Pointer to the sprite
 * @param xScale Scale factor for the x direction
 * @param yScale Scale factor for the y direction
 */
void (draw_scaled_sprite)(Sprite *sp, float xScale, float yScale);

/**
 * @brief Initializes global sprite variables
 */
void (create_sprites)();

/**
 * @brief Destroys global sprite variables
 */
void (destroy_sprites)();

/**
 * @brief Finds a sprite by name in the sprite lookup table
 * @param name Name of the sprite to find
 * @return Pointer to the sprite if found, NULL otherwise
 */
Sprite* (find_sprite)(const char *name);

#endif /* __SPRITE_H_ */
