#include "tower_field.h"

#include <stddef.h>

typedef struct {
  uint16_t range, damage, cooldown, cost;
  int16_t range_pct, dmg_pct, cd_pct;
  uint32_t highlight_color;
  uint8_t effect_type;
  uint16_t effect_dur;
  uint16_t effect_dur_pct;
  uint8_t effect_str;
  int16_t effect_str_pct;
  projectile_kind_t proj_kind;
  uint16_t proj_release;
} tower_base_stats_t;


static uint16_t scale_stat(uint16_t base, uint8_t lvl, int16_t num)
{
  int32_t scaled = (int32_t)base * (100 + num * (int32_t)lvl) / 100;
  if (scaled < 0)
    scaled = 0;
  return (uint16_t)scaled;
}

static int tower_field_sheet_fits(const vg_sprite_t *sheet, uint16_t frame_width,
                                    uint16_t frame_height, uint8_t frame_columns)
{
  if (sheet == NULL || sheet->pixels == NULL)
    return 0;

  return (uint32_t)frame_width * frame_columns <= sheet->width &&
        (uint32_t)frame_height * TOWER_DIRECTION_ROWS <= sheet->height;
}

int tower_field_add(tower_field_t *field, tower_kind_t kind, int16_t x, int16_t y)
{
  if (field == NULL || kind >= TOWER_KIND_COUNT)
    return 1;

  if (field->tower_count >= TOWER_MAX_COUNT)
    return 1;

  tower_t *tower = &field->towers[field->tower_count];
  tower_init(tower, &field->tower_types[0][kind], x, y);
  field->tower_count++;
  return 0;
}

int tower_field_init(tower_field_t *field, const tower_sprite_bank_t *sprites)
{
  if (field == NULL || sprites == NULL)
    return 1;

  // Validate all spritesheets fit their expected frame sizes
  for (uint8_t lvl = 0; lvl < TOWER_MAX_LEVEL; lvl++)
  {
    if (!tower_field_sheet_fits(sprites->basic_sheets[lvl], TOWER_FRAME_WIDTH, TOWER_FRAME_HEIGHT, TOWER_FRAME_COLUMNS) ||
        !tower_field_sheet_fits(sprites->gun_sheets[lvl], TOWER_GUN_FRAME_WIDTH, TOWER_GUN_FRAME_HEIGHT, TOWER_GUN_FRAME_COLUMNS) ||
        !tower_field_sheet_fits(sprites->smoke_sheets[lvl], TOWER_FRAME_WIDTH, TOWER_FRAME_HEIGHT, TOWER_FRAME_COLUMNS) ||
        !tower_field_sheet_fits(sprites->poison_sheets[lvl], TOWER_FRAME_WIDTH, TOWER_FRAME_HEIGHT, TOWER_POISON_FRAME_COLUMNS))
      return 1;
  }

  static const tower_anim_desc_t idle_desc = {
    .start_col = 0,
    .end_col = 1,
    .loop = 1,
    .release_col = 0
  };

  static const tower_anim_desc_t attack_desc_basic = {
    .start_col = 2,
    .end_col = 12,
    .loop = 0,
    .release_col = 8
  };

  static const tower_anim_desc_t attack_desc_gun = {
    .start_col = 2,
    .end_col = 8,
    .loop = 0,
    .release_col = 6
  };

  static const tower_anim_desc_t attack_desc_smoke = {
    .start_col = 2,
    .end_col = 12,
    .loop = 0,
    .release_col = 8
  };

  static const tower_anim_desc_t attack_desc_poison = {
    .start_col = 2,
    .end_col = 11,
    .loop = 0,
    .release_col = 7
  };

  static const tower_base_stats_t base_stats[TOWER_KIND_COUNT] = {
    [TOWER_KIND_BASIC]  = { 56, 16, 40, 90, 7, 60, -10, 0x000000, TOWER_EFFECT_NONE, 0, 0, 0, 0, PROJECTILE_SHURIKEN,    4 * TOWER_ATTACK_FRAME_TICKS },
    [TOWER_KIND_GUN]    = { 96, 68, 90, 155, 8, 60, -7, 0xDC143C, TOWER_EFFECT_NONE, 0, 0, 0, 0, PROJECTILE_BULLET,      2 * TOWER_ATTACK_FRAME_TICKS },
    [TOWER_KIND_SMOKE]  = { 60, 6, 72, 125, 7, 70, -6, 0x585858, TOWER_EFFECT_SLOW, 3000, 16, 30, 17, PROJECTILE_SMOKE_BOMB, 3 * TOWER_ATTACK_FRAME_TICKS },
    [TOWER_KIND_POISON] = { 64, 8, 64, 170, 7, 100, -7, 0xC71585, TOWER_EFFECT_POISON, 4000, 10, 8, 70, PROJECTILE_POISON_DART, 7 * TOWER_ATTACK_FRAME_TICKS },
  };

  static const tower_anim_desc_t *attack_descs[TOWER_KIND_COUNT] = {
    [TOWER_KIND_BASIC]  = &attack_desc_basic,
    [TOWER_KIND_GUN]    = &attack_desc_gun,
    [TOWER_KIND_SMOKE]  = &attack_desc_smoke,
    [TOWER_KIND_POISON] = &attack_desc_poison,
  };

  static const uint16_t frame_widths[TOWER_KIND_COUNT] = {
    [TOWER_KIND_BASIC]  = TOWER_FRAME_WIDTH,
    [TOWER_KIND_GUN]    = TOWER_GUN_FRAME_WIDTH,
    [TOWER_KIND_SMOKE]  = TOWER_FRAME_WIDTH,
    [TOWER_KIND_POISON] = TOWER_FRAME_WIDTH,
  };

  static const uint16_t frame_heights[TOWER_KIND_COUNT] = {
    [TOWER_KIND_BASIC]  = TOWER_FRAME_HEIGHT,
    [TOWER_KIND_GUN]    = TOWER_GUN_FRAME_HEIGHT,
    [TOWER_KIND_SMOKE]  = TOWER_FRAME_HEIGHT,
    [TOWER_KIND_POISON] = TOWER_FRAME_HEIGHT,
  };

  static const uint8_t frame_columns[TOWER_KIND_COUNT] = {
    [TOWER_KIND_BASIC]  = TOWER_FRAME_COLUMNS,
    [TOWER_KIND_GUN]    = TOWER_GUN_FRAME_COLUMNS,
    [TOWER_KIND_SMOKE]  = TOWER_FRAME_COLUMNS,
    [TOWER_KIND_POISON] = TOWER_POISON_FRAME_COLUMNS,
  };

  const vg_sprite_t * const *sheet_arrays[TOWER_KIND_COUNT] = {
    [TOWER_KIND_BASIC]  = sprites->basic_sheets,
    [TOWER_KIND_GUN]    = sprites->gun_sheets,
    [TOWER_KIND_SMOKE]  = sprites->smoke_sheets,
    [TOWER_KIND_POISON] = sprites->poison_sheets,
  };

  field->tower_count = 0;

  for (uint8_t lvl = 0; lvl < TOWER_MAX_LEVEL; lvl++)
  {
    for (tower_kind_t kind = 0; kind < TOWER_KIND_COUNT; kind++)
    {
      const tower_base_stats_t *base = &base_stats[kind];

      uint16_t range    = scale_stat(base->range, lvl, base->range_pct);
      uint16_t damage   = scale_stat(base->damage, lvl, base->dmg_pct);
      uint16_t cooldown = scale_stat(base->cooldown, lvl, base->cd_pct);

      uint16_t cost;
      if (lvl == 0)
        cost = base->cost;
      else
        cost = (uint16_t)(base->cost * (lvl * lvl + 3) / 5);

      uint8_t effect_str = (uint8_t)scale_stat(base->effect_str, lvl, base->effect_str_pct);
      // Cap slow effect at 50%
      if (base->effect_type == TOWER_EFFECT_SLOW && effect_str > 50)
        effect_str = 50;

      uint16_t effect_dur = scale_stat(base->effect_dur, lvl, base->effect_dur_pct);

      tower_type_init(&field->tower_types[lvl][kind],
                      kind,
                      sheet_arrays[kind][lvl],
                      frame_widths[kind],
                      frame_heights[kind],
                      frame_columns[kind],
                      TOWER_DIRECTION_ROWS,
                      range, damage, cooldown, cost,
                      base->highlight_color,
                      base->proj_kind,
                      base->proj_release,
                      base->effect_type,
                      effect_dur,
                      effect_str,
                      &idle_desc,
                      attack_descs[kind]);
    }
  }

  return 0;
}

int tower_field_remove(tower_field_t *field, uint16_t index)
{
  if (field == NULL || index >= field->tower_count)
    return 1;

  for (uint16_t i = index; i + 1 < field->tower_count; i++)
    field->towers[i] = field->towers[i + 1];

  field->tower_count--;
  return 0;
}

void tower_field_step(tower_field_t *field)
{
  if (field == NULL)
    return;

  for (uint16_t index = 0; index < field->tower_count; index++)
    tower_step(&field->towers[index]);
}
