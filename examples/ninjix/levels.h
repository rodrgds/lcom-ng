/**
 * @file levels.h
 * @brief Level registry -- access and navigation between level definitions.
 * @ingroup group_levels
 *
 * Provides indexed lookup of the precompiled level_t definitions and a
 * has-next query for sequential level progression.
 */

#ifndef PROJ_LEVELS_H
#define PROJ_LEVELS_H

#include <stdbool.h>
#include <stdint.h>

#include "path_map.h"

#define LEVEL_COUNT 5
#define LEVEL_FIRST_ID 1

const level_t *levels_get(uint16_t level_id);
bool levels_has_next(uint16_t level_id);

#endif
