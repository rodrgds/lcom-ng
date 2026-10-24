#include "bullet.h"
#include "../config.h"
#include "../graphics/graphics.h"
#include <stddef.h>

#define BULLET_DRAW_WIDTH 8
#define BULLET_DRAW_HEIGHT 16
#define ENEMY_BULLET_DRAW_WIDTH 12
#define ENEMY_BULLET_DRAW_HEIGHT 16

static int bullet_abs(int value) {
  return value < 0 ? -value : value;
}

static int bullet_sign(int value) {
  if (value < 0) {
    return -1;
  }

  if (value > 0) {
    return 1;
  }

  return 0;
}

void bullet_spawn(Game *game) {
  int count = game->weapon_multi_shot_active ? 3 : 1;
  int vxs[] = {0, -4, 4};
  int spawned = 0;

  for (int i = 0; i < MAX_BULLETS && spawned < count; i++) {
    if (!game->bullets[i].base.active) {
      game->bullets[i].base.active = true;
      game->bullets[i].base.width = BULLET_DRAW_WIDTH;
      game->bullets[i].base.height = BULLET_DRAW_HEIGHT;
      game->bullets[i].base.x = game->ship.base.x + game->ship.base.width / 2 - game->bullets[i].base.width / 2;
      game->bullets[i].base.y = game->ship.base.y - game->bullets[i].base.height;
      game->bullets[i].base.vx = vxs[spawned];
      game->bullets[i].base.vy = -BULLET_SPEED;
      spawned++;
    }
  }
}

void enemy_bullet_spawn(Game *game, int x, int y, int target_x, int target_y) {
  for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
    if (!game->enemy_bullets[i].base.active) {
      int dx = target_x - x;
      int dy = target_y - y;
      int abs_dx = bullet_abs(dx);
      int abs_dy = bullet_abs(dy);
      int major = abs_dx > abs_dy ? abs_dx : abs_dy;
      int vx = 0;
      int vy = ENEMY_BULLET_SPEED;

      if (major > 0) {
        vx = (dx * ENEMY_BULLET_SPEED) / major;
        vy = (dy * ENEMY_BULLET_SPEED) / major;
      }

      if (vx == 0 && dx != 0) {
        vx = bullet_sign(dx);
      }

      if (vy == 0) {
        vy = dy < 0 ? -1 : 1;
      }

      game->enemy_bullets[i].base.active = true;
      game->enemy_bullets[i].base.width = ENEMY_BULLET_DRAW_WIDTH;
      game->enemy_bullets[i].base.height = ENEMY_BULLET_DRAW_HEIGHT;
      game->enemy_bullets[i].base.x = x - ENEMY_BULLET_DRAW_WIDTH / 2;
      game->enemy_bullets[i].base.y = y - ENEMY_BULLET_DRAW_HEIGHT / 2;
      game->enemy_bullets[i].base.vx = vx;
      game->enemy_bullets[i].base.vy = vy;
      return;
    }
  }
}

void bullet_update_all(Game *game) {
  for (int i = 0; i < MAX_BULLETS; i++) {
    if (!game->bullets[i].base.active) {
      continue;
    }
    game->bullets[i].base.x += game->bullets[i].base.vx;
    game->bullets[i].base.y += game->bullets[i].base.vy;
    if (game->bullets[i].base.y + game->bullets[i].base.height < 0) {
      game->bullets[i].base.active = false;
    }
  }

  for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
    if (!game->enemy_bullets[i].base.active) {
      continue;
    }

    game->enemy_bullets[i].base.x += game->enemy_bullets[i].base.vx;
    game->enemy_bullets[i].base.y += game->enemy_bullets[i].base.vy;

    if (game->enemy_bullets[i].base.y > SCREEN_HEIGHT ||
        game->enemy_bullets[i].base.x + game->enemy_bullets[i].base.width < 0 ||
        game->enemy_bullets[i].base.x > SCREEN_WIDTH) {
      game->enemy_bullets[i].base.active = false;
    }
  }
}

void bullet_draw_all(const Game *game, const Sprite *sprite) {
  for (int i = 0; i < MAX_BULLETS; i++) {
    if (!game->bullets[i].base.active) {
      continue;
    }
    if (sprite != NULL && sprite->loaded) {
      sprite_draw_scaled(sprite, game->bullets[i].base.x, game->bullets[i].base.y, 1);
    } else {
      graphics_draw_rectangle_clipped(game->bullets[i].base.x,
                                      game->bullets[i].base.y,
                                      game->bullets[i].base.width,
                                      game->bullets[i].base.height,
                                      COLOR_WHITE);
    }
  }
}

void enemy_bullet_draw_all(const Game *game, const Sprite *sprite) {
  for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
    if (!game->enemy_bullets[i].base.active) {
      continue;
    }

    if (sprite != NULL && sprite->loaded) {
      sprite_draw(sprite, game->enemy_bullets[i].base.x, game->enemy_bullets[i].base.y);
    }
    else {
      graphics_draw_rectangle_clipped(game->enemy_bullets[i].base.x,
                                      game->enemy_bullets[i].base.y,
                                      game->enemy_bullets[i].base.width,
                                      game->enemy_bullets[i].base.height,
                                      COLOR_RED);
    }
  }
}
