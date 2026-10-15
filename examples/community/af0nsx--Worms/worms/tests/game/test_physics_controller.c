#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#include "../../src/config.h"
#include "../../src/game/physics/physics_controller.h"

static int ground_y = SCREEN_H + 1;
static int wall_x = SCREEN_W + 1;

bool terrain_collides_hitbox(Hitbox hitbox) {
  if (hitbox.y + hitbox.height > ground_y) return true;
  if (hitbox.x + hitbox.width > wall_x) return true;
  return false;
}

static void test_worm_lands_on_ground(void) {
  Worm *worm = worm_create(20, 100);

  assert(worm != NULL);

  ground_y = 200;
  wall_x = SCREEN_W + 1;
  worm_set_grounded(worm, false);

  for (int i = 0; i < 40 && !worm_is_grounded(worm); i++) {
    physics_update_worm(worm);
  }

  assert(worm_is_grounded(worm));
  assert(worm_get_y(worm) == ground_y - WORM_HITBOX_HEIGHT);
  assert(worm_get_vy(worm) == 0);

  worm_destroy(worm);
}

static void test_worm_stops_before_wall(void) {
  Worm *worm = worm_create(20, 200 - WORM_HITBOX_HEIGHT);

  assert(worm != NULL);

  ground_y = 200;
  wall_x = 55;
  worm_set_grounded(worm, true);
  worm_set_vx(worm, WORM_MOVE_SPEED);

  physics_update_worm(worm);

  assert(worm_get_x(worm) + WORM_HITBOX_WIDTH <= wall_x);
  assert(worm_get_vx(worm) == 0);

  worm_destroy(worm);
}

static void test_jump_requires_ground(void) {
  Worm *worm = worm_create(20, 100);

  assert(worm != NULL);

  worm_set_grounded(worm, false);
  worm_jump(worm);
  assert(worm_get_vy(worm) == 0);

  worm_set_grounded(worm, true);
  worm_jump(worm);
  assert(worm_get_vy(worm) == -WORM_JUMP_SPEED);

  worm_destroy(worm);
}

int main(void) {
  test_worm_lands_on_ground();
  test_worm_stops_before_wall();
  test_jump_requires_ground();

  return 0;
}
