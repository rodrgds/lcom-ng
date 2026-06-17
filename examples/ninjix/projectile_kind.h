/**
 * @file projectile_kind.h
 * @brief Projectile type enumeration -- shuriken, bullet, smoke bomb, poison dart.
 * @ingroup group_projectiles
 *
 * Each tower kind maps to a specific projectile kind through tower_type_t.
 */

#ifndef PROJ_PROJECTILE_KIND_H
#define PROJ_PROJECTILE_KIND_H

typedef enum {
  PROJECTILE_SHURIKEN = 0,
  PROJECTILE_BULLET,
  PROJECTILE_SMOKE_BOMB,
  PROJECTILE_POISON_DART,
  PROJECTILE_KIND_COUNT,
} projectile_kind_t;

#endif
