#include "hitbox.h"

#include <stddef.h>

Hitbox hitbox_create(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
  Hitbox hitbox;

  hitbox.x = x;
  hitbox.y = y;
  hitbox.width = width;
  hitbox.height = height;

  return hitbox;
}

void hitbox_set_position(Hitbox *hitbox, uint16_t x, uint16_t y) {
  if (hitbox == NULL) return;

  hitbox->x = x;
  hitbox->y = y;
}

bool hitbox_is_empty(const Hitbox *hitbox) {
  if (hitbox == NULL) return true;

  return hitbox->width == 0 || hitbox->height == 0;
}

bool hitbox_is_outside_bounds(const Hitbox *hitbox, uint16_t width, uint16_t height) {
  if (hitbox == NULL) return false;

  return hitbox->x >= width || hitbox->y >= height;
}

bool hitbox_intersects(const Hitbox *a, const Hitbox *b) {
  uint32_t a_right;
  uint32_t a_bottom;
  uint32_t b_right;
  uint32_t b_bottom;

  if (hitbox_is_empty(a) || hitbox_is_empty(b)) return false;

  a_right = (uint32_t)a->x + a->width;
  a_bottom = (uint32_t)a->y + a->height;
  b_right = (uint32_t)b->x + b->width;
  b_bottom = (uint32_t)b->y + b->height;

  return a->x < b_right &&
         a_right > b->x &&
         a->y < b_bottom &&
         a_bottom > b->y;
}

bool hitbox_contains_point(const Hitbox *hitbox, uint16_t x, uint16_t y) {
  uint32_t right;
  uint32_t bottom;

  if (hitbox == NULL) return false;

  right = (uint32_t)hitbox->x + hitbox->width;
  bottom = (uint32_t)hitbox->y + hitbox->height;

  return x >= hitbox->x &&
         x < right &&
         y >= hitbox->y &&
         y < bottom;
}
