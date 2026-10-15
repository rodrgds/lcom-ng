#include "physics_controller.h"

#include <stddef.h>

#include "../../config.h"
#include "../terrain/terrain.h"

static bool hitbox_at_collides(int32_t x, int32_t y) {
  Hitbox hitbox;

  if (x < 0 || y < 0) return true;
  if (x + WORM_HITBOX_WIDTH > SCREEN_W) return true;
  if (y + WORM_HITBOX_HEIGHT > SCREEN_H) return true;

  hitbox = hitbox_create((uint16_t) x,
                         (uint16_t) y,
                         WORM_HITBOX_WIDTH,
                         WORM_HITBOX_HEIGHT);
  return terrain_collides_hitbox(hitbox);
}

static void update_grounded_state(Worm *worm) {
  bool grounded = hitbox_at_collides(worm_get_x(worm), worm_get_y(worm) + 1);
  worm_set_grounded(worm, grounded);
}

static void apply_horizontal_motion(Worm *worm) {
  double vx = worm_get_vx(worm);
  double start_x = worm_get_precise_x(worm);
  double target_x = start_x + vx;
  int32_t current_x = worm_get_x(worm);
  int32_t target_pixel_x = (int32_t) target_x;
  int32_t step;

  if (vx == 0) return;

  step = (vx > 0) ? 1 : -1;

  while (current_x != target_pixel_x) {
    int32_t next_x = current_x + step;
    int32_t y = worm_get_y(worm);

    if (!hitbox_at_collides(next_x, y)) {
      worm_set_position(worm, next_x, worm_get_precise_y(worm));
      current_x = next_x;
      continue;
    }

    bool stepped = false;
    for (int32_t up = 1; up <= WORM_MAX_STEP_HEIGHT; up++) {
      if (!hitbox_at_collides(next_x, y - up)) {
        worm_set_position(worm, next_x, worm_get_precise_y(worm) - up);
        current_x = next_x;
        stepped = true;
        break;
      }
    }

    if (!stepped) {
      worm_set_vx(worm, 0);
      return;
    }
  }

  if (!hitbox_at_collides(target_pixel_x, worm_get_y(worm))) {
    worm_set_position(worm, target_x, worm_get_precise_y(worm));
  }
}

static void apply_vertical_motion(Worm *worm) {
  double vy = worm_get_vy(worm);
  double start_y = worm_get_precise_y(worm);
  double target_y = start_y + vy;
  int32_t current_y = worm_get_y(worm);
  int32_t target_pixel_y = (int32_t) target_y;
  int32_t step;

  if (vy == 0) {
    update_grounded_state(worm);
    return;
  }

  step = (vy > 0) ? 1 : -1;
  worm_set_grounded(worm, false);

  while (current_y != target_pixel_y) {
    int32_t x = worm_get_x(worm);
    int32_t next_y = current_y + step;

    if (hitbox_at_collides(x, next_y)) {
      worm_set_vy(worm, 0);
      if (step > 0) worm_set_grounded(worm, true);
      return;
    }

    worm_set_position(worm, worm_get_precise_x(worm), next_y);
    current_y = next_y;
  }

  if (!hitbox_at_collides(worm_get_x(worm), target_pixel_y)) {
    worm_set_position(worm, worm_get_precise_x(worm), target_y);
  }
}

static void update_worm_state(Worm *worm) {
  WormState current = worm_get_state(worm);
  if (current == WORM_WEAPON_PULL || current == WORM_WEAPON_HOLD || current == WORM_WEAPON_RETRACT) return;

  if (!worm_is_grounded(worm) && worm_get_vy(worm) < 0) {
    worm_set_state(worm, WORM_JUMP);
  } else if (!worm_is_grounded(worm) && worm_get_vy(worm) > 1.5) {
    worm_set_state(worm, WORM_FALL);
  } else if (worm_get_vx(worm) != 0) {
    worm_set_state(worm, WORM_WALK);
  } else {
    worm_set_state(worm, WORM_IDLE);
  }
}

void physics_update_worm(Worm *worm) {
  if (worm == NULL || !worm_is_alive(worm)) return;

  apply_horizontal_motion(worm);
  apply_vertical_motion(worm);
  update_grounded_state(worm);

  if (!worm_is_grounded(worm)) {
    worm_set_vy(worm, worm_get_vy(worm) + WORM_GRAVITY);
  }
  else if (worm_get_vy(worm) > 0) {
    worm_set_vy(worm, 0);
  }

  update_worm_state(worm);
}
