#include "projectile.h"

#include <stdlib.h>

#include "../../config.h"

struct Projectile {
  double x;
  double y;
  double vx;
  double vy;
  uint16_t width;
  uint16_t height;
  uint16_t damage;
  bool active;
};

Projectile* projectile_create(double x, double y,
                              double vx, double vy,
                              uint16_t width, uint16_t height,
                              uint16_t damage) {
  Projectile *projectile = (Projectile *) malloc(sizeof(Projectile));
  if (projectile == NULL) return NULL;

  projectile->x = x;
  projectile->y = y;
  projectile->vx = vx;
  projectile->vy = vy;
  projectile->width = width;
  projectile->height = height;
  projectile->damage = damage;
  projectile->active = true;

  return projectile;
}

void projectile_destroy(Projectile *projectile) {
  if (projectile == NULL) return;

  free(projectile);
}

void projectile_update(Projectile *projectile, uint16_t screen_width, uint16_t screen_height) {
  if (projectile == NULL || !projectile->active) return;

  projectile->x += projectile->vx;
  projectile->y += projectile->vy;
  projectile->vy += PROJECTILE_GRAVITY;

  if (projectile->x < 0 ||
      projectile->x >= screen_width ||
      projectile->y >= screen_height) {
    projectile->active = false;
  }
}

Hitbox projectile_get_hitbox(const Projectile *projectile) {
  if (projectile == NULL || !projectile->active) {
    return hitbox_create(0, 0, 0, 0);
  }

  if (projectile->x < 0 || projectile->y < 0 ||
      projectile->x > UINT16_MAX || projectile->y > UINT16_MAX) {
    return hitbox_create(0, 0, 0, 0);
  }

  return hitbox_create((uint16_t) projectile->x,
                       (uint16_t) projectile->y,
                       projectile->width,
                       projectile->height);
}

bool projectile_is_active(const Projectile *projectile) {
  if (projectile == NULL) return false;

  return projectile->active;
}

void projectile_deactivate(Projectile *projectile) {
  if (projectile == NULL) return;

  projectile->active = false;
}

uint16_t projectile_get_damage(const Projectile *projectile) {
  if (projectile == NULL) return 0;

  return projectile->damage;
}

double projectile_get_vx(const Projectile *projectile) {
  if (projectile == NULL) return 0;
  return projectile->vx;
}

double projectile_get_vy(const Projectile *projectile) {
  if (projectile == NULL) return 0;
  return projectile->vy;
}

void projectile_set_vx(Projectile *projectile, double vx) {
  if (projectile == NULL) return;
  projectile->vx = vx;
}

void projectile_set_vy(Projectile *projectile, double vy) {
  if (projectile == NULL) return;
  projectile->vy = vy;
}

double projectile_get_precise_x(const Projectile *projectile) {
  if (projectile == NULL) return 0;
  return projectile->x;
}

double projectile_get_precise_y(const Projectile *projectile) {
  if (projectile == NULL) return 0;
  return projectile->y;
}

void projectile_set_x(Projectile *projectile, double x) {
  if (projectile == NULL) return;
  projectile->x = x;
}

void projectile_set_y(Projectile *projectile, double y) {
  if (projectile == NULL) return;
  projectile->y = y;
}
