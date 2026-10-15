#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#include "../../../src/config.h"
#include "../../../src/game/worm/worm.h"

static void test_worm_creation_and_hitbox(void) {
  Worm *worm = worm_create(20, 40);
  Hitbox hitbox;

  assert(worm != NULL);
  assert(worm_get_x(worm) == 20);
  assert(worm_get_y(worm) == 40);
  assert(worm_get_health(worm) == WORM_INITIAL_HEALTH);
  assert(worm_is_alive(worm) == true);

  hitbox = worm_get_hitbox(worm);
  assert(hitbox.x == 20);
  assert(hitbox.y == 40);
  assert(hitbox.width == WORM_HITBOX_WIDTH);
  assert(hitbox.height == WORM_HITBOX_HEIGHT);

  worm_destroy(worm);
}

static void test_worm_damage_and_death(void) {
  Worm *worm = worm_create(10, 20);
  Hitbox hitbox;

  assert(worm != NULL);

  worm_take_damage(worm, 25);
  assert(worm_get_health(worm) == WORM_INITIAL_HEALTH - 25);
  assert(worm_is_alive(worm) == true);

  worm_take_damage(worm, WORM_INITIAL_HEALTH);
  assert(worm_get_health(worm) == 0);
  assert(worm_is_alive(worm) == false);

  hitbox = worm_get_hitbox(worm);
  assert(hitbox_is_empty(&hitbox) == true);

  worm_destroy(worm);
}

static void test_worm_horizontal_movement(void) {
  Worm *worm = worm_create(20, 40);

  assert(worm != NULL);

  worm_move(worm, 5);
  assert(worm_get_x(worm) == 25);
  assert(worm_get_y(worm) == 40);

  worm_move(worm, -10);
  assert(worm_get_x(worm) == 15);
  assert(worm_get_y(worm) == 40);

  worm_destroy(worm);
}

static void test_dead_worm_does_not_move(void) {
  Worm *worm = worm_create(20, 40);

  assert(worm != NULL);

  worm_take_damage(worm, WORM_INITIAL_HEALTH);
  worm_move(worm, 5);
  assert(worm_get_x(worm) == 20);
  assert(worm_get_y(worm) == 40);

  worm_destroy(worm);
}

static void test_moving_null_worm_is_safe(void) {
  worm_move(NULL, 5);
}

static void test_jump_starts_vertical_motion(void) {
  Worm *worm = worm_create(20, 100);

  assert(worm != NULL);

  worm_jump(worm);
  assert(worm_get_y(worm) == 100);
  assert(worm_get_vy(worm) == -WORM_JUMP_SPEED);
  assert(worm_is_grounded(worm) == false);
  assert(worm_get_state(worm) == WORM_JUMP);

  worm_destroy(worm);
}

static void test_dead_and_null_worm_ignore_jump(void) {
  Worm *worm = worm_create(20, 40);

  assert(worm != NULL);

  worm_take_damage(worm, WORM_INITIAL_HEALTH);
  worm_jump(worm);
  assert(worm_get_y(worm) == 40);
  assert(worm_get_vy(worm) == 0);

  worm_jump(NULL);

  worm_destroy(worm);
}

static void test_worm_damage_hitbox(void) {
  Worm *worm = worm_create(20, 40);
  Hitbox hb;

  assert(worm != NULL);

  hb = worm_get_damage_hitbox(worm);
  assert(hb.x      == 20 + WORM_DAMAGE_HITBOX_OFFSET_X);
  assert(hb.y      == 40 + WORM_DAMAGE_HITBOX_OFFSET_Y);
  assert(hb.width  == WORM_DAMAGE_HITBOX_WIDTH);
  assert(hb.height == WORM_DAMAGE_HITBOX_HEIGHT);

  /* damage box must be strictly inside the physics box */
  assert(hb.x >= 20);
  assert(hb.y >= 40);
  assert(hb.x + hb.width  <= 20 + WORM_HITBOX_WIDTH);
  assert(hb.y + hb.height <= 40 + WORM_HITBOX_HEIGHT);

  /* dead worm → empty hitbox */
  worm_take_damage(worm, WORM_INITIAL_HEALTH);
  hb = worm_get_damage_hitbox(worm);
  assert(hitbox_is_empty(&hb) == true);

  worm_destroy(worm);
}

int main(void) {
  test_worm_creation_and_hitbox();
  test_worm_damage_and_death();
  test_worm_horizontal_movement();
  test_dead_worm_does_not_move();
  test_moving_null_worm_is_safe();
  test_jump_starts_vertical_motion();
  test_dead_and_null_worm_ignore_jump();
  test_worm_damage_hitbox();

  return 0;
}
