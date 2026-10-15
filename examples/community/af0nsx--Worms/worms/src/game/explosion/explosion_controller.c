#include "explosion_controller.h"
#include "../../config.h"
#include "../terrain/terrain.h"
#include <stddef.h>

// Returns squared distance between two points.
static int32_t dist_sq(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
  int32_t dx = x2 - x1;
  int32_t dy = y2 - y1;
  return dx * dx + dy * dy;
}

static int32_t clamp_int32(int32_t value, int32_t min, int32_t max) {
  if (value < min) return min;
  if (value > max) return max;
  return value;
}

static bool explosion_touches_hitbox(int32_t x, int32_t y, int radius, const Hitbox *hitbox) {
  if (hitbox == NULL || hitbox_is_empty(hitbox)) return false;

  int32_t nearest_x = clamp_int32(x, hitbox->x, hitbox->x + hitbox->width);
  int32_t nearest_y = clamp_int32(y, hitbox->y, hitbox->y + hitbox->height);
  return dist_sq(x, y, nearest_x, nearest_y) <= radius * radius;
}

Explosion* explosion_controller_spawn(GameContext *ctx, int32_t x, int32_t y, int radius, int max_damage) {
  if (ctx == NULL) return NULL;

  terrain_explode(x, y, radius);

  // Worm damage
  for (int i = 0; i < MAX_WORMS; i++) {
    Worm *worm = game_context_get_worm(ctx, i);
    if (worm != NULL && worm_is_alive(worm)) {
      Hitbox hb = worm_get_damage_hitbox(worm);
      
      if (explosion_touches_hitbox(x, y, radius, &hb)) {
        worm_take_damage(worm, max_damage);
      }
    }
  }

  Explosion *e = explosion_create(x, y, EXPLOSION_DURATION_TICKS);
  if (e != NULL) {
    game_context_add_explosion(ctx, e);
  }
  return e;
}

void explosion_controller_tick_all(GameContext *ctx) {
  if (ctx == NULL) return;
  for (int i = 0; i < MAX_EXPLOSIONS; i++) {
    Explosion *e = game_context_get_explosion(ctx, i);
    if (e != NULL && explosion_is_active(e)) {
      explosion_advance_tick(e);
    }
  }
}
