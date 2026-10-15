#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#include "../../../src/config.h"
#include "../../../src/game/projectile/projectile.h"

static void test_projectile_movement(void) {
  Projectile *p = projectile_create(10.0, 20.0, 3.0, 4.0, 5, 5, 10);
  Hitbox h;

  assert(p != NULL);

  projectile_update(p, 800, 600);

  h = projectile_get_hitbox(p);
  assert(h.x == 13);
  assert(h.y == 24);
  assert(h.width == 5);
  assert(h.height == 5);

  projectile_destroy(p);
}

static void test_projectile_gravity_accelerates_downward(void) {
  Projectile *p = projectile_create(10.0, 100.0, 2.0, -10.0, 5, 5, 10);
  Hitbox h;

  assert(p != NULL);

  projectile_update(p, 800, 600);
  h = projectile_get_hitbox(p);
  assert(h.x == 12);
  assert(h.y == 90);

  projectile_update(p, 800, 600);
  h = projectile_get_hitbox(p);
  assert(h.x == 14);
  assert(h.y == 80 + PROJECTILE_GRAVITY);

  projectile_destroy(p);
}

static void test_inactive_projectile_does_not_move(void) {
  Projectile *p = projectile_create(10.0, 100.0, 2.0, 3.0, 5, 5, 10);
  Hitbox h;

  assert(p != NULL);

  projectile_deactivate(p);
  projectile_update(p, 800, 600);
  h = projectile_get_hitbox(p);
  assert(projectile_is_active(p) == false);
  assert(h.width == 0);
  assert(h.height == 0);

  projectile_destroy(p);
}

static void test_projectile_deactivates_out_of_bounds(void) {
  Projectile *p = projectile_create(798.0, 10.0, 3.0, 0.0, 5, 5, 10);
  Hitbox h;

  assert(p != NULL);

  projectile_update(p, 800, 600);

  assert(projectile_is_active(p) == false);

  h = projectile_get_hitbox(p);
  assert(h.width == 0);
  assert(h.height == 0);

  projectile_destroy(p);
}

static void test_projectile_above_screen_has_empty_hitbox(void) {
  Projectile *p = projectile_create(100.0, 0.0, 0.0, -10.0, 5, 5, 10);
  Hitbox h;

  assert(p != NULL);

  projectile_update(p, 800, 600);

  assert(projectile_is_active(p) == true);

  h = projectile_get_hitbox(p);
  assert(h.width == 0);
  assert(h.height == 0);

  projectile_destroy(p);
}

int main(void) {
  test_projectile_movement();
  test_projectile_gravity_accelerates_downward();
  test_inactive_projectile_does_not_move();
  test_projectile_deactivates_out_of_bounds();
  test_projectile_above_screen_has_empty_hitbox();

  return 0;
}
