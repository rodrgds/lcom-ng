#include "explosion.h"
#include "../config.h"
#include "../graphics/graphics.h"
#include <stddef.h>

#define EXPLOSION_DRAW_SIZE 64

void explosion_spawn(Game *game, int center_x, int center_y) {
  for (int i = 0; i < MAX_EXPLOSIONS; i++) {
    if (!game->explosions[i].base.active) {
      game->explosions[i].base.active = true;
      game->explosions[i].base.x = center_x;
      game->explosions[i].base.y = center_y;
      game->explosions[i].age = 0;
      return;
    }
  }
}

void explosion_update_all(Game *game) {
  for (int i = 0; i < MAX_EXPLOSIONS; i++) {
    if (!game->explosions[i].base.active) {
      continue;
    }
    game->explosions[i].age++;
    if (game->explosions[i].age >= EXPLOSION_DURATION) {
      game->explosions[i].base.active = false;
    }
  }
}

void explosion_draw_all(const Game *game, const Sprite *sprite) {
  for (int i = 0; i < MAX_EXPLOSIONS; i++) {
    if (!game->explosions[i].base.active) {
      continue;
    }
    if (sprite != NULL && sprite->loaded) {
      sprite_draw_scaled_centered(sprite, game->explosions[i].base.x, game->explosions[i].base.y, 1);
    } else {
      graphics_draw_rectangle_clipped(game->explosions[i].base.x - EXPLOSION_DRAW_SIZE / 2,
                                      game->explosions[i].base.y - EXPLOSION_DRAW_SIZE / 2,
                                      EXPLOSION_DRAW_SIZE,
                                      EXPLOSION_DRAW_SIZE,
                                      COLOR_GREEN);
    }
  }
}
