/**
 * @file levels.c
 * @brief Level registry -- lookup and progression for precompiled level definitions.
 * @ingroup group_levels
 */

#include "levels.h"

#include "levels/level_1.h"
#include "levels/level_2.h"
#include "levels/level_3.h"
#include "levels/level_4.h"
#include "levels/level_5.h"

static const level_t *const level_table[LEVEL_COUNT] = {
  &level_1, &level_2, &level_3, &level_4, &level_5,
};

const level_t *levels_get(uint16_t level_id)
{
  if (level_id < LEVEL_FIRST_ID || level_id > LEVEL_COUNT)
    return NULL;
  return level_table[level_id - LEVEL_FIRST_ID];
}

bool levels_has_next(uint16_t level_id)
{
  return level_id >= LEVEL_FIRST_ID && level_id < LEVEL_COUNT;
}
