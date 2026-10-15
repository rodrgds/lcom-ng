#include "worm.h"

#include <stdlib.h>

#include "../../config.h"

struct Worm {
  double x;
  double y;
  double vx;
  double vy;
  bool is_grounded;
  uint16_t health;
  WormState state;
  WormDirection direction;
  WeaponType current_weapon;
};

Worm *worm_create(int32_t x, int32_t y) {
  Worm *worm = (Worm *) malloc(sizeof(Worm));
  if (worm == NULL) return NULL;

  worm->x = x;
  worm->y = y;
  worm->vx = 0;
  worm->vy = 0;
  worm->is_grounded = true;
  worm->health = WORM_INITIAL_HEALTH;
  worm->state = WORM_IDLE;
  worm->direction = WORM_DIR_RIGHT;
  worm->current_weapon = WEAPON_NONE;

  return worm;
}

void worm_destroy(Worm *worm) {
  if (worm == NULL) return;

  free(worm);
}

void worm_take_damage(Worm *worm, uint16_t damage) {
  if (worm == NULL || !worm_is_alive(worm)) return;

  if (damage >= worm->health) {
    worm->health = 0;
    worm->state = WORM_DEAD;
    return;
  }

  worm->health -= damage;
}

Hitbox worm_get_hitbox(const Worm *worm) {
  if (worm == NULL || !worm_is_alive(worm) ||
      worm->x < 0 || worm->y < 0 ||
      worm->x > UINT16_MAX || worm->y > UINT16_MAX) {
    return hitbox_create(0, 0, 0, 0);
  }

  return hitbox_create((uint16_t) worm_get_x(worm),
                       (uint16_t) worm_get_y(worm),
                       WORM_HITBOX_WIDTH,
                       WORM_HITBOX_HEIGHT);
}

Hitbox worm_get_damage_hitbox(const Worm *worm) {
  if (worm == NULL || !worm_is_alive(worm) ||
      worm->x < 0 || worm->y < 0 ||
      worm->x > UINT16_MAX || worm->y > UINT16_MAX) {
    return hitbox_create(0, 0, 0, 0);
  }

  return hitbox_create(
    (uint16_t)(worm_get_x(worm) + WORM_DAMAGE_HITBOX_OFFSET_X),
    (uint16_t)(worm_get_y(worm) + WORM_DAMAGE_HITBOX_OFFSET_Y),
    WORM_DAMAGE_HITBOX_WIDTH,
    WORM_DAMAGE_HITBOX_HEIGHT
  );
}

int32_t worm_get_x(const Worm *worm) {
  if (worm == NULL) return 0;
  return (int32_t) worm->x;
}

int32_t worm_get_y(const Worm *worm) {
  if (worm == NULL) return 0;
  return (int32_t) worm->y;
}

double worm_get_vx(const Worm *worm) {
  if (worm == NULL) return 0;
  return worm->vx;
}

double worm_get_vy(const Worm *worm) {
  if (worm == NULL) return 0;
  return worm->vy;
}

double worm_get_precise_x(const Worm *worm) {
  if (worm == NULL) return 0;
  return worm->x;
}

double worm_get_precise_y(const Worm *worm) {
  if (worm == NULL) return 0;
  return worm->y;
}

uint16_t worm_get_health(const Worm *worm) {
  if (worm == NULL) return 0;
  return worm->health;
}

bool worm_is_alive(const Worm *worm) {
  if (worm == NULL) return false;
  return worm->state != WORM_DEAD && worm->state != WORM_DROWNED && worm->health > 0;
}

bool worm_is_grounded(const Worm *worm) {
  if (worm == NULL || !worm_is_alive(worm)) return false;
  return worm->is_grounded;
}

WormState worm_get_state(const Worm *worm) {
  if (worm == NULL) return WORM_DEAD;
  return worm->state;
}

WormDirection worm_get_direction(const Worm *worm) {
  if (worm == NULL) return WORM_DIR_RIGHT;
  return worm->direction;
}

void worm_set_state(Worm *worm, WormState state) {
  if (worm == NULL) return;
  worm->state = state;
}

void worm_set_direction(Worm *worm, WormDirection dir) {
  if (worm == NULL) return;
  worm->direction = dir;
}

void worm_set_position(Worm *worm, double x, double y) {
  if (worm == NULL || !worm_is_alive(worm)) return;
  worm->x = x;
  worm->y = y;
}

void worm_set_vx(Worm *worm, double vx) {
  if (worm == NULL || !worm_is_alive(worm)) return;
  worm->vx = vx;
  if (vx > 0) worm->direction = WORM_DIR_RIGHT;
  else if (vx < 0) worm->direction = WORM_DIR_LEFT;
}

void worm_set_vy(Worm *worm, double vy) {
  if (worm == NULL || !worm_is_alive(worm)) return;
  worm->vy = vy;
}

void worm_set_grounded(Worm *worm, bool grounded) {
  if (worm == NULL || !worm_is_alive(worm)) return;
  worm->is_grounded = grounded;
}

void worm_move(Worm *worm, double dx) {
  if (worm == NULL || !worm_is_alive(worm)) return;
  if (worm->state != WORM_IDLE && worm->state != WORM_WALK && worm->state != WORM_WEAPON_HOLD) return;

  worm->x += dx;
  // update direction based on movement
  if (dx > 0) worm->direction = WORM_DIR_RIGHT;
  else if (dx < 0) worm->direction = WORM_DIR_LEFT;
}

void worm_jump(Worm *worm) {
  if (worm == NULL || !worm_is_alive(worm)) return;
  if (!worm->is_grounded) return;

  worm->vy = -WORM_JUMP_SPEED;
  worm->is_grounded = false;
  worm->state = WORM_JUMP;
}

WeaponType worm_get_weapon(const Worm *worm) {
  if (worm == NULL) return WEAPON_NONE;
  return worm->current_weapon;
}

void worm_set_weapon(Worm *worm, WeaponType weapon) {
  if (worm == NULL) return;
  worm->current_weapon = weapon;
}
