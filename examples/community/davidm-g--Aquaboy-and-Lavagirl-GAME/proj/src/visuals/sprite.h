
#ifndef SPRITE_H
#define SPRITE_H
#include <lcom/lcf.h>
#include <stdarg.h> // va_* macros are defined here

typedef struct {
  uint16_t x, y;
  uint16_t width, height;
  int16_t xspeed, yspeed, xaccell;
  bool isOnGround;
  uint32_t *map;
} Sprite;

/**
 * @brief Creates a new sprite.
 *
 * @param map The XPM map of the sprite.
 * @param x The initial X-coordinate of the sprite.
 * @param y The initial Y-coordinate of the sprite.
 * @param xspeed The initial X-speed of the sprite.
 * @param yspeed The initial Y-speed of the sprite.
 * @return A pointer to the created sprite.
 */
Sprite *create_sprite(xpm_map_t map, uint16_t x, uint16_t y, int16_t xspeed, int16_t yspeed);

/**
 * @brief Destroys a sprite.
 *
 * @param sp The sprite to be destroyed.
 */
void destroy_sprite(Sprite *sp);

/**
 * @brief Gets the X-coordinate of a sprite.
 *
 * @param sp The sprite.
 * @return The X-coordinate of the sprite.
 */
uint16_t get_posx(Sprite *sp);

/**
 * @brief Gets the Y-coordinate of a sprite.
 *
 * @param sp The sprite.
 * @return The Y-coordinate of the sprite.
 */
uint16_t get_posy(Sprite *sp);

/**
 * @brief Sets the X-coordinate of a sprite.
 *
 * @param sp The sprite.
 * @param x The new X-coordinate of the sprite.
 */
void set_posx(Sprite *sp, uint16_t x);

/**
 * @brief Sets the Y-coordinate of a sprite.
 *
 * @param sp The sprite.
 * @param y The new Y-coordinate of the sprite.
 */
void set_posy(Sprite *sp, uint16_t y);
#endif
