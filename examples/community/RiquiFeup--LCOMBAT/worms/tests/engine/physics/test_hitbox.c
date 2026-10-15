#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#include "../../../src/engine/physics/hitbox.h"

static void test_hitbox_intersects(void) {
  Hitbox a = hitbox_create(10, 10, 20, 20);
  Hitbox b = hitbox_create(25, 25, 20, 20);
  Hitbox c = hitbox_create(30, 10, 20, 20);
  Hitbox empty = hitbox_create(15, 15, 0, 10);

  assert(hitbox_intersects(&a, &b) == true);

  /* Edge-touching rectangles are intentionally not considered intersecting. */
  assert(hitbox_intersects(&a, &c) == false);
  assert(hitbox_intersects(&a, &empty) == false);
}

static void test_hitbox_contains_point(void) {
  Hitbox h = hitbox_create(10, 10, 20, 20);

  assert(hitbox_contains_point(&h, 10, 10) == true);
  assert(hitbox_contains_point(&h, 29, 29) == true);

  assert(hitbox_contains_point(&h, 30, 30) == false);
  assert(hitbox_contains_point(&h, 9, 10) == false);
}

static void test_hitbox_helpers(void) {
  Hitbox h = hitbox_create(10, 10, 20, 20);
  Hitbox empty = hitbox_create(0, 0, 0, 0);

  hitbox_set_position(&h, 50, 60);
  assert(h.x == 50);
  assert(h.y == 60);
  assert(h.width == 20);
  assert(h.height == 20);

  assert(hitbox_is_empty(&h) == false);
  assert(hitbox_is_empty(&empty) == true);
  assert(hitbox_is_empty(NULL) == true);

  assert(hitbox_is_outside_bounds(&h, 800, 600) == false);
  hitbox_set_position(&h, 800, 60);
  assert(hitbox_is_outside_bounds(&h, 800, 600) == true);
  assert(hitbox_is_outside_bounds(NULL, 800, 600) == false);
}

int main(void) {
  test_hitbox_intersects();
  test_hitbox_contains_point();
  test_hitbox_helpers();

  return 0;
}
