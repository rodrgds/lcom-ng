/**
 * @file health_bar.c
 * @brief Implementation of health bar drawing.
 *
 * This file contains the implementation of the function declared in health_bar.h.
 */
#include "health_bar.h"

int draw_health_bar(unsigned int health, sprite_t *h_bar) {
  const uint16_t spacing = 30;
  position_t *pos = get_sprite_pos(h_bar);
  const int16_t px = pos->x;

  while (health > 0) {
    if (draw_sprite(h_bar)) {
      return 1;
    }
    pos->x += spacing;
    health--;
  }

  pos->x = px;
  return 0;
}
