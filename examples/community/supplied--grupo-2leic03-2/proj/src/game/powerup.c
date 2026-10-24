#include "powerup.h"
#include "../config.h"
#include "../graphics/graphics.h"
#include <stdlib.h>

bool powerup_spawn(Game *game, PowerupType type) {
  for (int i = 0; i < MAX_POWERUPS; i++) {
    if (!game->powerups[i].base.active) {
      game->powerups[i].base.active = true;
      game->powerups[i].type = type;
      game->powerups[i].base.width = 32; // Sprite size * 2
      game->powerups[i].base.height = 32;
      game->powerups[i].base.x = rand() % (SCREEN_WIDTH - game->powerups[i].base.width);
      game->powerups[i].base.y = -game->powerups[i].base.height;
      game->powerups[i].base.vy = 2; // Fixed speed downward
      return true;
    }
  }

  return false;
}

void powerup_update_all(Game *game) {
  for (int i = 0; i < MAX_POWERUPS; i++) {
    if (!game->powerups[i].base.active) {
      continue;
    }

    game->powerups[i].base.y += game->powerups[i].base.vy;
    if (game->powerups[i].base.y > SCREEN_HEIGHT) {
      game->powerups[i].base.active = false;
    }
  }
}

void powerup_draw_all(const Game *game, const Sprite *health_sprite, const Sprite *shield_sprite, const Sprite *fire_rate_sprite, const Sprite *multi_shot_sprite) {
  for (int i = 0; i < MAX_POWERUPS; i++) {
    if (!game->powerups[i].base.active) {
      continue;
    }

    const Sprite *spr = NULL;
    uint32_t fallback_color = COLOR_WHITE;
    if (game->powerups[i].type == POWERUP_HEALTH) {
      spr = health_sprite;
      fallback_color = COLOR_RED;
    } else if (game->powerups[i].type == POWERUP_SHIELD) {
      spr = shield_sprite;
      fallback_color = COLOR_CYAN;
    } else if (game->powerups[i].type == POWERUP_FIRE_RATE) {
      spr = fire_rate_sprite;
      fallback_color = COLOR_YELLOW;
    } else if (game->powerups[i].type == POWERUP_MULTI_SHOT) {
      spr = multi_shot_sprite;
      fallback_color = COLOR_ORANGE;
    }

    if (spr != NULL && spr->loaded) {
      sprite_draw_scaled(spr, game->powerups[i].base.x, game->powerups[i].base.y, 2);
    } else {
      graphics_draw_rectangle_clipped(game->powerups[i].base.x,
                                      game->powerups[i].base.y,
                                      game->powerups[i].base.width,
                                      game->powerups[i].base.height,
                                      fallback_color);
    }
  }
}
