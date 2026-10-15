#include <assert.h>
#include <stddef.h>

#include "../../../src/config.h"
#include "../../../src/game/worm/worm_controller.h"

static void test_movement_commands(void) {
  assert(worm_controller_command_from_key(KEY_LEFT) == WORM_COMMAND_MOVE_LEFT);
  assert(worm_controller_command_from_key(KEY_RIGHT) == WORM_COMMAND_MOVE_RIGHT);
  assert(worm_controller_command_from_key(KEY_SPACE) == WORM_COMMAND_JUMP);
}

static void test_unrelated_keys_have_no_command(void) {
  assert(worm_controller_command_from_key(KEY_NONE) == WORM_COMMAND_NONE);
  assert(worm_controller_command_from_key(KEY_ESC) == WORM_COMMAND_NONE);
  assert(worm_controller_command_from_key(KEY_ENTER) == WORM_COMMAND_NONE);
}

static void test_apply_jump_starts_vertical_motion(void) {
  Worm *worm = worm_create(20, 100);

  assert(worm != NULL);

  worm_controller_apply_command(worm, WORM_COMMAND_JUMP);
  assert(worm_get_y(worm) == 100);
  assert(worm_get_vy(worm) == -WORM_JUMP_SPEED);
  assert(worm_is_grounded(worm) == false);

  worm_destroy(worm);
}

static void test_horizontal_command_works_after_jump_starts(void) {
  Worm *worm = worm_create(20, 100);

  assert(worm != NULL);

  worm_controller_apply_command(worm, WORM_COMMAND_JUMP);
  worm_controller_apply_command(worm, WORM_COMMAND_MOVE_RIGHT);
  assert(worm_get_x(worm) == 20);
  assert(worm_get_y(worm) == 100);
  assert(worm_get_vx(worm) == WORM_MOVE_SPEED);
  assert(worm_get_vy(worm) == -WORM_JUMP_SPEED);

  worm_destroy(worm);
}

int main(void) {
  test_movement_commands();
  test_unrelated_keys_have_no_command();
  test_apply_jump_starts_vertical_motion();
  test_horizontal_command_works_after_jump_starts();

  return 0;
}
