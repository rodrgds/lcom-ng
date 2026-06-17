/**
 * @file enemy_handle.h
 * @brief Generation-based handle for safe enemy references.
 * @ingroup group_enemies
 *
 * A handle pairs a pool index with a generation counter. Before use, the
 * generation is compared against the current slot generation to ensure the
 * handle is still valid (same entity, not reused by a later spawn).
 */

#ifndef PROJ_ENEMY_HANDLE_H
#define PROJ_ENEMY_HANDLE_H

#include <stdbool.h>
#include <stdint.h>

#define ENEMY_HANDLE_INVALID_INDEX UINT16_MAX

typedef struct {
  uint16_t index;
  uint16_t generation;
} enemy_handle_t;

static inline enemy_handle_t enemy_handle_invalid(void)
{
  enemy_handle_t handle = {
    .index = ENEMY_HANDLE_INVALID_INDEX,
    .generation = 0
  };
  return handle;
}

static inline bool enemy_handle_is_valid(enemy_handle_t handle)
{
  return handle.index != ENEMY_HANDLE_INVALID_INDEX;
}

#endif
