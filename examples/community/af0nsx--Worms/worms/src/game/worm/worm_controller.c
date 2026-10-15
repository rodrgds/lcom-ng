#include "worm_controller.h"
#include "../../engine/input/input.h"
#include "../../config.h"

WormCommand worm_controller_command_from_key(InputKey key) {
  switch (key) {
    case KEY_LEFT:
      return WORM_COMMAND_MOVE_LEFT;
    case KEY_RIGHT:
      return WORM_COMMAND_MOVE_RIGHT;
    case KEY_SPACE:
      return WORM_COMMAND_JUMP;
    case KEY_1:
      return WORM_COMMAND_EQUIP_BAZOOKA;
    case KEY_2:
      return WORM_COMMAND_EQUIP_GRENADE;
    default:
      return WORM_COMMAND_NONE;
  }
}

void worm_controller_apply_command(Worm *worm, WormCommand command) {
  switch (command) {
    case WORM_COMMAND_MOVE_LEFT:
      worm_set_vx(worm, -WORM_MOVE_SPEED);
      break;

    case WORM_COMMAND_MOVE_RIGHT:
      worm_set_vx(worm, WORM_MOVE_SPEED);
      break;

    case WORM_COMMAND_JUMP:
      worm_jump(worm);
      break;
    case WORM_COMMAND_EQUIP_BAZOOKA:
      if (worm_get_state(worm) == WORM_IDLE || worm_get_state(worm) == WORM_WALK || worm_get_state(worm) == WORM_WEAPON_PULL || worm_get_state(worm) == WORM_WEAPON_HOLD) {
        if ((worm_get_state(worm) == WORM_WEAPON_PULL || worm_get_state(worm) == WORM_WEAPON_HOLD) && worm_get_weapon(worm) == WEAPON_BAZOOKA) {
          worm_set_state(worm, WORM_IDLE);
        } else {
          worm_set_weapon(worm, WEAPON_BAZOOKA);
          worm_set_state(worm, WORM_WEAPON_PULL);
          worm_set_vx(worm, 0);
        }
      }
      break;
    case WORM_COMMAND_EQUIP_GRENADE:
      if (worm_get_state(worm) == WORM_IDLE || worm_get_state(worm) == WORM_WALK || worm_get_state(worm) == WORM_WEAPON_PULL || worm_get_state(worm) == WORM_WEAPON_HOLD) {
        if ((worm_get_state(worm) == WORM_WEAPON_PULL || worm_get_state(worm) == WORM_WEAPON_HOLD) && worm_get_weapon(worm) == WEAPON_GRENADE) {
          worm_set_state(worm, WORM_IDLE);
        } else {
          worm_set_weapon(worm, WEAPON_GRENADE);
          worm_set_state(worm, WORM_WEAPON_PULL);
          worm_set_vx(worm, 0);
        }
      }
      break;
    case WORM_COMMAND_NONE:
      worm_set_vx(worm, 0);
      break;
  }
}

void worm_controller_apply_keys(Worm *worm) {
  if (input_is_key_pressed(KEY_LEFT)) {
    worm_controller_apply_command(worm, WORM_COMMAND_MOVE_LEFT);
  }
  else if (input_is_key_pressed(KEY_RIGHT)) {
    worm_controller_apply_command(worm, WORM_COMMAND_MOVE_RIGHT);
  }
  else {
    worm_controller_apply_command(worm, WORM_COMMAND_NONE);
  }

  if (input_is_key_pressed(KEY_UP)) {
    worm_controller_apply_command(worm, WORM_COMMAND_JUMP);
  }
}
