#include "play_state.h"

#include <stdio.h>
#include <string.h>

#include "levels.h"
#include "profiler.h"
#include "targeting.h"
#include "placement.h"
#include "app_constants.h"

#define PLACEMENT_DEBUG 0
#define MULTIPLAYER_START_ELIXIR 25
#define MULTIPLAYER_ELIXIR_CAP 999
#define MULTIPLAYER_WAVE_ELIXIR_BONUS 20

static const uint16_t enemy_elixir_costs[ENEMY_KIND_COUNT] = {
  [ENEMY_KIND_NORMAL]      = 3,
  [ENEMY_KIND_FAST]        = 5,
  [ENEMY_KIND_HEALING]     = 7,
  [ENEMY_KIND_ARMORED]     = 10,
  [ENEMY_KIND_EXPLODING]   = 12,
  [ENEMY_KIND_CASH]        = 14,
  [ENEMY_KIND_TELEPORTING] = 15,
  [ENEMY_KIND_INVISIBLE]   = 16,
};

static const uint16_t enemy_elixir_rewards[ENEMY_KIND_COUNT] = {
  [ENEMY_KIND_NORMAL]      = 4,
  [ENEMY_KIND_FAST]        = 7,
  [ENEMY_KIND_HEALING]     = 9,
  [ENEMY_KIND_ARMORED]     = 12,
  [ENEMY_KIND_EXPLODING]   = 14,
  [ENEMY_KIND_CASH]        = 16,
  [ENEMY_KIND_TELEPORTING] = 18,
  [ENEMY_KIND_INVISIBLE]   = 20,
};

static void play_state_add_elixir(play_state_t *play, uint16_t amount)
{
  if (play == NULL || amount == 0)
    return;

  uint32_t total = (uint32_t)play->elixir + amount;
  play->elixir = (total > MULTIPLAYER_ELIXIR_CAP) ? MULTIPLAYER_ELIXIR_CAP : (uint16_t)total;
}

uint16_t play_state_enemy_elixir_cost(enemy_kind_t kind)
{
  if (kind >= ENEMY_KIND_COUNT)
    return 0;

  return enemy_elixir_costs[kind];
}

static uint16_t play_state_enemy_elixir_reward(enemy_kind_t kind)
{
  if (kind >= ENEMY_KIND_COUNT)
    return 0;

  return enemy_elixir_rewards[kind];
}

static void play_state_on_enemy_killed(play_state_t *play, enemy_t *enemy)
{
  if (play == NULL || enemy == NULL || !enemy->alive)
    return;

  play->coins = (uint16_t)(play->coins + enemy->type->reward);
  play->enemies_killed++;
  enemy->alive = false;
}

static void play_state_effect_apply(play_state_t *play, enemy_handle_t target,
                                     uint8_t effect_type, uint16_t duration, uint8_t strength)
{
  if (play == NULL || effect_type == TOWER_EFFECT_NONE || !enemy_handle_is_valid(target))
    return;

  // Check for existing effect on the same target: strongest refreshes, weaker is ignored!
  for (uint16_t i = 0; i < MAX_ACTIVE_EFFECTS; i++)
  {
    active_effect_t *existing = &play->active_effects[i];
    if (!existing->active || existing->effect_type != effect_type)
      continue;
    if (existing->target.index != target.index || existing->target.generation != target.generation)
      continue;

    if (strength > existing->strength)
    {
      existing->strength = strength;
      existing->ticks_left = (uint16_t)((uint32_t)duration * APP_TIMER_HZ / 1000);
      existing->sub_tick = 0;
    }
    return;
  }

  // No existing effect, find an inactive slot
  for (uint16_t i = 0; i < MAX_ACTIVE_EFFECTS; i++)
  {
    if (!play->active_effects[i].active)
    {
      play->active_effects[i].active = true;
      play->active_effects[i].target = target;
      play->active_effects[i].effect_type = effect_type;
      play->active_effects[i].strength = strength;
      play->active_effects[i].sub_tick = 0;
      play->active_effects[i].ticks_left = (uint16_t)((uint32_t)duration * APP_TIMER_HZ / 1000);
      play->active_effects[i].tick_interval = (effect_type == TOWER_EFFECT_POISON) ? APP_TIMER_HZ : 1;

      return;
    }
  }
}

static uint16_t play_state_compute_override_speed(play_state_t *play, uint16_t enemy_slot)
{
  uint16_t override_speed = UINT16_MAX;

  for (uint16_t i = 0; i < MAX_ACTIVE_EFFECTS; i++)
  {
    active_effect_t *effect = &play->active_effects[i];
    if (!effect->active || effect->effect_type != TOWER_EFFECT_SLOW)
      continue;
    if (effect->target.index != enemy_slot)
      continue;

    enemy_t *enemy = enemy_pool_get(&play->wave_ctrl.enemies, effect->target);
    if (enemy == NULL || !enemy->alive)
      continue;

    uint16_t reduced = (uint16_t)(enemy->type->speed * (100 - effect->strength) / 100);
    if (reduced < override_speed)
      override_speed = reduced;
  }

  return override_speed;
}

static void play_state_step_effects(play_state_t *play)
{
  if (play == NULL)
    return;

  for (uint16_t i = 0; i < MAX_ACTIVE_EFFECTS; i++)
  {
    active_effect_t *effect = &play->active_effects[i];
    if (!effect->active)
      continue;

    enemy_t *enemy = enemy_pool_get(&play->wave_ctrl.enemies, effect->target);
    if (enemy == NULL || !enemy->alive)
    {
      effect->active = false;
      continue;
    }

    if (effect->effect_type == TOWER_EFFECT_POISON)
    {
      effect->sub_tick++;
      if (effect->sub_tick >= effect->tick_interval)
      {
        effect->sub_tick = 0;
        enemy->health = (int16_t)(enemy->health - (int16_t)effect->strength);
        if (enemy->health <= 0)
          play_state_on_enemy_killed(play, enemy);
      }
    }

    if (effect->ticks_left > 0)
      effect->ticks_left--;
    if (effect->ticks_left == 0)
      effect->active = false;
  }
}

static void play_state_on_enemy_leaked(play_state_t *play, enemy_t *enemy)
{
  if (play == NULL || enemy == NULL || enemy->type == NULL || !enemy->reached_goal)
    return;

  if (play->manual_spawning)
    play_state_add_elixir(play, play_state_enemy_elixir_reward(enemy->type->kind));

  if (enemy->type->damage_to_base >= play->base_hp)
    play->base_hp = 0;
  else
    play->base_hp = (uint16_t)(play->base_hp - enemy->type->damage_to_base);
}

static void play_state_step_tower_attacks(play_state_t *play)
{
  if (enemy_pool_empty(&play->wave_ctrl.enemies))
    return;

  for (uint16_t i = 0; i < play->tower_field.tower_count; i++)
  {
    tower_t *tower = &play->tower_field.towers[i];
    if (tower->type == NULL || tower->cooldown_left > 0 || tower->anim == TOWER_ANIM_ATTACK)
      continue;

    // The tower is ready to attack, find a target
    enemy_handle_t target = targeting_find_enemy(&play->wave_ctrl.enemies, tower);
    enemy_t *enemy = enemy_pool_get(&play->wave_ctrl.enemies, target);
    if (enemy == NULL)
      continue;

    tower_face_point(tower, enemy->x, enemy->y);
    tower->cooldown_left = tower->type->cooldown_ticks;
    tower->anim = TOWER_ANIM_ATTACK;
    tower->anim_tick = 0;
    tower->pending_target = target;
  }
}

static void play_state_step_tower_projectile_releases(play_state_t *play)
{
  for (uint16_t i = 0; i < play->tower_field.tower_count; i++)
  {
    tower_t *tower = &play->tower_field.towers[i];
    if (tower->type == NULL || tower->anim != TOWER_ANIM_ATTACK)
      continue;
    if (!enemy_handle_is_valid(tower->pending_target))
      continue;
    if (tower->anim_tick < tower->type->projectile_release_ticks)
      continue;

    // When the projectile finally releases
    enemy_t *enemy = enemy_pool_get(&play->wave_ctrl.enemies, tower->pending_target);
    if (enemy != NULL)
    {
      tower_face_point(tower, enemy->x, enemy->y);
      projectile_pool_spawn(&play->projectiles,
                            tower->type->projectile_kind,
                            tower->pending_target,
                            (int16_t)(tower->x + tower->type->frame_width / 2),
                            (int16_t)(tower->y + tower->type->frame_height / 2),
                            8, tower->type->damage,
                            tower->type->effect_type,
                            tower->type->effect_duration,
                            tower->type->effect_strength);
    }

    tower->pending_target = enemy_handle_invalid();
  }
}

static void play_state_step_projectiles(play_state_t *play)
{
  uint16_t cursor = 0;
  while (cursor < play->projectiles.active_count)
  {
    uint16_t i = play->projectiles.active_indices[cursor];
    projectile_t *projectile = &play->projectiles.projectiles[i];
    if (!projectile->active)
    {
      // if the current projectile is no longer active, remove it from the pool and continue with the next one
      projectile_pool_deactivate(&play->projectiles, i);
      continue;
    }

    enemy_t *enemy = enemy_pool_get(&play->wave_ctrl.enemies, projectile->target);
    if (enemy == NULL || !enemy->alive)
    {
      // if the enemy died in the meantime, or reached the goal and is now invalid, deactivate the projectile
      projectile_pool_deactivate(&play->projectiles, i);
      continue;
    }

    int16_t dx = (int16_t)(enemy->x - projectile->x);
    int16_t dy = (int16_t)(enemy->y - projectile->y);
    int16_t abs_dx = (dx >= 0) ? dx : (int16_t)-dx;
    int16_t abs_dy = (dy >= 0) ? dy : (int16_t)-dy;

    if (abs_dx <= (int16_t)projectile->speed && abs_dy <= (int16_t)projectile->speed)
    {
      enemy->health = (int16_t)(enemy->health - (int16_t)projectile->damage);

      if (projectile->effect_type == TOWER_EFFECT_SLOW &&
          (enemy->type->abilities & ENEMY_ABILITY_INVISIBLE))
        enemy->revealed = true;

      play_state_effect_apply(play, projectile->target,
                              projectile->effect_type,
                              projectile->effect_duration,
                              projectile->effect_strength);
      projectile_pool_deactivate(&play->projectiles, i);

      if (enemy->health <= 0)
        play_state_on_enemy_killed(play, enemy);
      continue;
    }

    int16_t step_x = (abs_dx < (int16_t)projectile->speed) ? abs_dx : (int16_t)projectile->speed;
    int16_t step_y = (abs_dy < (int16_t)projectile->speed) ? abs_dy : (int16_t)projectile->speed;

    if (dx > 0)
      projectile->x = (int16_t)(projectile->x + step_x);
    else if (dx < 0)
      projectile->x = (int16_t)(projectile->x - step_x);

    if (dy > 0)
      projectile->y = (int16_t)(projectile->y + step_y);
    else if (dy < 0)
      projectile->y = (int16_t)(projectile->y - step_y);

    cursor++;
  }
}

void play_state_begin_placement(play_state_t *play, tower_kind_t kind)
{
  if (play == NULL || kind >= TOWER_KIND_COUNT)
    return;

  play->placement_active = true;
  play->placement_kind = kind;
  play->placement_hover_valid = false;
}

void play_state_cancel_placement(play_state_t *play)
{
  if (play == NULL)
    return;

  play->placement_active = false;
  play->placement_hover_valid = false;
}

void play_state_update_placement_hover(play_state_t *play, int16_t pixel_x, int16_t pixel_y)
{
  uint16_t col;
  uint16_t row;

  if (play == NULL || !play->placement_active)
    return;

  if (!pixels_to_tile_coordinates(&play->path_map, pixel_x, pixel_y, &col, &row))
  {
    play->placement_hover_valid = false;
    return;
  }

  play->placement_hover_valid = true;
  play->placement_hover_col = (int16_t)col;
  play->placement_hover_row = (int16_t)row;
  play->placement_hover_result = validate_tile_placement(&play->path_map,
                                                           &play->tower_field,
                                                           play->placement_hover_col,
                                                           play->placement_hover_row);
}

int play_state_try_place_tower(play_state_t *play, int16_t pixel_x, int16_t pixel_y)
{
  uint16_t col;
  uint16_t row;
  int16_t tower_x;
  int16_t tower_y;
  tile_placement_result_t result;
  const tower_type_t *type;

  if (play == NULL || !play->placement_active)
    return 1;

  if (!pixels_to_tile_coordinates(&play->path_map, pixel_x, pixel_y, &col, &row))
    return 1;

  result = validate_tile_placement(&play->path_map, &play->tower_field,
                                   (int16_t)col, (int16_t)row);
  if (result != TILE_PLACEMENT_OK)
    return 1;

  type = &play->tower_field.tower_types[0][play->placement_kind];

  if (play->coins < type->cost)
    return 1;

  tower_x = (int16_t)(PLACEMENT_MAP_ORIGIN_X + col * PLACEMENT_TILE_SIZE +
                      (PLACEMENT_TILE_SIZE - type->frame_width) / 2);
  tower_y = (int16_t)(PLACEMENT_MAP_ORIGIN_Y + row * PLACEMENT_TILE_SIZE +
                      (PLACEMENT_TILE_SIZE - type->frame_height) / 2);

  if (tower_field_add(&play->tower_field, play->placement_kind, tower_x, tower_y) != 0)
    return 1;

  play->coins = (uint16_t)(play->coins - type->cost);
  play_state_cancel_placement(play);
  return 0;
}

static void play_state_clear_multiplayer_wave_budget(play_state_t *play)
{
  if (play == NULL)
    return;

  memset(play->multiplayer_wave_remaining, 0, sizeof(play->multiplayer_wave_remaining));
  play->multiplayer_wave_limit = 0;
  play->multiplayer_wave_sent = 0;
}

static void play_state_open_multiplayer_wave(play_state_t *play)
{
  if (play == NULL || play->wave_ctrl.current_wave >= play->wave_ctrl.wave_count)
    return;

  play_state_clear_multiplayer_wave_budget(play);

  const wave_t *wave = &play->wave_ctrl.waves[play->wave_ctrl.current_wave];
  for (uint8_t i = 0; i < wave->entry_count; i++)
  {
    const wave_entry_t *entry = &wave->entries[i];
    if (entry->kind >= ENEMY_KIND_COUNT)
      continue;

    play->multiplayer_wave_remaining[entry->kind] =
        (uint16_t)(play->multiplayer_wave_remaining[entry->kind] + entry->count);
    play->multiplayer_wave_limit = (uint16_t)(play->multiplayer_wave_limit + entry->count);
  }

  play->wave_ctrl.wave_active = true;
  play->wave_ctrl.timer = 0;
  play_state_add_elixir(play, MULTIPLAYER_WAVE_ELIXIR_BONUS);
}

uint16_t play_state_multiplayer_enemy_remaining(const play_state_t *play, enemy_kind_t kind)
{
  if (play == NULL || kind >= ENEMY_KIND_COUNT)
    return 0;

  return play->multiplayer_wave_remaining[kind];
}

uint16_t play_state_multiplayer_wave_remaining_total(const play_state_t *play)
{
  uint16_t total = 0;

  if (play == NULL)
    return 0;

  for (uint8_t i = 0; i < ENEMY_KIND_COUNT; i++)
    total = (uint16_t)(total + play->multiplayer_wave_remaining[i]);

  return total;
}

static bool play_state_multiplayer_has_affordable_enemy(const play_state_t *play)
{
  if (play == NULL)
    return false;

  for (uint8_t i = 0; i < ENEMY_KIND_COUNT; i++)
  {
    if (play->multiplayer_wave_remaining[i] > 0 &&
        play->elixir >= play_state_enemy_elixir_cost((enemy_kind_t)i))
      return true;
  }

  return false;
}

void play_state_set_manual_spawning(play_state_t *play, bool manual_spawning)
{
  if (play == NULL)
    return;

  play->manual_spawning = manual_spawning;
  if (manual_spawning)
  {
    play->tower_field.tower_count = 0;
    enemy_pool_init(&play->wave_ctrl.enemies);
    projectile_pool_init(&play->projectiles);
    memset(play->active_effects, 0, sizeof(play->active_effects));
    play->elixir = MULTIPLAYER_START_ELIXIR;
    play_state_clear_multiplayer_wave_budget(play);
    play->wave_ctrl.current_wave = 0;
    play->wave_ctrl.timer = (play->wave_ctrl.wave_count > 0) ?
                            play->wave_ctrl.waves[0].pre_wave_delay : 0;
    play->wave_ctrl.wave_active = false;
    play->wave_ctrl.level_complete = false;
  }
}

int play_state_place_tower_direct(play_state_t *play, tower_kind_t kind, int16_t x, int16_t y)
{
  if (play == NULL || kind >= TOWER_KIND_COUNT)
    return 1;

  return tower_field_add(&play->tower_field, kind, x, y);
}

int play_state_spawn_enemy(play_state_t *play, enemy_kind_t kind)
{
  uint16_t cost = 0;

  if (play == NULL || kind >= ENEMY_KIND_COUNT)
    return 1;

  if (play->manual_spawning)
  {
    cost = play_state_enemy_elixir_cost(kind);
    if (!play->wave_ctrl.wave_active ||
        play->wave_ctrl.level_complete ||
        play->multiplayer_wave_remaining[kind] == 0 ||
        play->elixir < cost)
      return 1;
  }

  enemy_handle_t handle = enemy_pool_spawn(&play->wave_ctrl.enemies,
                                           play->wave_ctrl.enemy_types[kind],
                                           play->wave_ctrl.spawn_x,
                                           play->wave_ctrl.spawn_y);
  if (!enemy_handle_is_valid(handle))
    return 1;

  if (play->manual_spawning)
  {
    play->elixir = (uint16_t)(play->elixir - cost);
    play->multiplayer_wave_remaining[kind]--;
    play->multiplayer_wave_sent++;
  }

  return 0;
}

int play_state_spawn_enemy_in_wave(play_state_t *play, enemy_kind_t kind, uint16_t wave_index)
{
  if (play == NULL || kind >= ENEMY_KIND_COUNT)
    return 1;

  if (play->manual_spawning &&
      !play->wave_ctrl.level_complete &&
      !play->wave_ctrl.wave_active &&
      play->wave_ctrl.current_wave == wave_index)
  {
    play->wave_ctrl.timer = 0;
    play_state_open_multiplayer_wave(play);
  }

  return play_state_spawn_enemy(play, kind);
}

// Start of placement validation logic debug
#if PLACEMENT_DEBUG
static const char *placement_result_name(tile_placement_result_t result)
{
  switch (result)
  {
  case TILE_PLACEMENT_OK:
    return "OK";
  case TILE_PLACEMENT_OUT_OF_BOUNDS:
    return "OUT_OF_BOUNDS";
  case TILE_PLACEMENT_ON_PATH:
    return "ON_PATH";
  case TILE_PLACEMENT_OCCUPIED:
    return "OCCUPIED";
  default:
    return "?";
  }
}

static void play_state_test_placement(const play_state_t *play)
{
  const path_map_t *map;
  const tower_field_t *field;
  tile_placement_result_t result;
  uint16_t col;
  uint16_t row;

  if (play == NULL)
    return;

  map = &play->path_map;
  field = &play->tower_field;

  printf("[placement] --- tests start ---\n");

  result = validate_tile_placement(map, field, -1, 0);
  printf("[placement] (-1,0) -> %s\n", placement_result_name(result));

  result = validate_tile_placement(map, field, 18, 0);
  printf("[placement] (18,0) -> %s\n", placement_result_name(result));

  result = validate_tile_placement(map, field, 0, 0);
  printf("[placement] (0,0) grass -> %s\n", placement_result_name(result));

  result = validate_tile_placement(map, field, 0, 2);
  printf("[placement] (0,2) spawn -> %s\n", placement_result_name(result));

  result = validate_tile_placement(map, field, 3, 2);
  printf("[placement] (3,2) path # -> %s\n", placement_result_name(result));

  result = validate_tile_placement(map, field, 3, 2);
  printf("[placement] (3,2) with debug tower at (130,90) -> %s (expect ON_PATH, tile is #)\n",
         placement_result_name(result));

  result = validate_tile_placement(map, field, 6, 2);
  printf("[placement] (6,2) grass -> %s\n", placement_result_name(result));

  if (pixels_to_tile_coordinates(map, 130, 90, &col, &row))
    printf("[placement] pixels (130,90) -> col=%u row=%u\n",
           (unsigned)col, (unsigned)row);
  else
    printf("[placement] pixels (130,90) -> conversion failed\n");

  if (pixels_to_tile_coordinates(map, 5, 5, &col, &row))
    printf("[placement] pixels (5,5) -> col=%u row=%u (inside map, expected)\n", (unsigned)col, (unsigned)row);

  printf("[placement] --- per-tower validate ---\n");
  for (uint16_t i = 0; i < field->tower_count; i++)
  {
    const tower_t *tower = &field->towers[i];

    if (!pixels_to_tile_coordinates(map, tower->x, tower->y, &col, &row))
      continue;

    result = validate_tile_placement(map, field, (int16_t)col, (int16_t)row);
    printf("[placement] tower[%u] at pixel (%d,%d) -> cell (%u,%u) -> %s\n",
           (unsigned)i, (int)tower->x, (int)tower->y,
           (unsigned)col, (unsigned)row, placement_result_name(result));
  }

  printf("[placement] --- tests end ---\n");
}
#endif
// End of placement validation logic debug

int play_state_init(play_state_t *play, const resources_t *resources,
                    uint16_t level_id)
{
  if (play == NULL || resources == NULL)
    return 1;

  const level_t *level = levels_get(level_id);
  if (level == NULL || level->waves == NULL || level->wave_count == 0)
  {
    printf("play_state_init(): invalid level %u\n", (unsigned)level_id);
    return 1;
  }

  memset(play, 0, sizeof(*play));
  play->level_id = level_id;
  play->base_max_hp = 100;
  play->base_hp = play->base_max_hp;
  play->coins = level->start_coins;
  play->speed_multiplier = 1;
  play->selected_tower = -1;
  play->wave_bonus_base = level->wave_bonus_base;
  play->wave_bonus_step = level->wave_bonus_step;

  if (path_map_load_level(&play->path_map, level) != 0)
  {
    printf("play_state_init(): path_map_load_level() failed\n");
    play_state_shutdown(play);
    return 1;
  }

  tower_sprite_bank_t tower_sprites = {
    .basic_sheets = { &resources->tower_basic_sprites[0], &resources->tower_basic_sprites[1],
                      &resources->tower_basic_sprites[2], &resources->tower_basic_sprites[3],
                      &resources->tower_basic_sprites[4] },
    .gun_sheets = { &resources->tower_gun_sprites[0], &resources->tower_gun_sprites[1],
                    &resources->tower_gun_sprites[2], &resources->tower_gun_sprites[3],
                    &resources->tower_gun_sprites[4] },
    .smoke_sheets = { &resources->tower_smoke_sprites[0], &resources->tower_smoke_sprites[1],
                      &resources->tower_smoke_sprites[2], &resources->tower_smoke_sprites[3],
                      &resources->tower_smoke_sprites[4] },
    .poison_sheets = { &resources->tower_poison_sprites[0], &resources->tower_poison_sprites[1],
                       &resources->tower_poison_sprites[2], &resources->tower_poison_sprites[3],
                       &resources->tower_poison_sprites[4] }
  };

  if (tower_field_init(&play->tower_field, &tower_sprites) != 0)
  {
    printf("play_state_init(): tower_field_init() failed\n");
    play_state_shutdown(play);
    return 1;
  }

  if (!enemy_path_compute(&play->path_map,
                          play->path_waypoints_x,
                          play->path_waypoints_y,
                          &play->path_waypoint_count,
                          MAX_WAYPOINTS))
  {
    printf("play_state_init(): enemy_path_compute() failed\n");
    play_state_shutdown(play);
    return 1;
  }

  wave_ctrl_init(&play->wave_ctrl,
                 level->waves, level->wave_count,
                 resources->enemy_types,
                 (int16_t)(ENEMY_PATH_ORIGIN_X + play->path_map.spawn_x * ENEMY_PATH_TILE_SIZE + ENEMY_PATH_TILE_SIZE / 2),
                 (int16_t)(ENEMY_PATH_ORIGIN_Y + play->path_map.spawn_y * ENEMY_PATH_TILE_SIZE + ENEMY_PATH_TILE_SIZE / 2),
                 play->path_waypoints_x,
                 play->path_waypoints_y,
                 play->path_waypoint_count);
  projectile_pool_init(&play->projectiles);

  return 0;
}

void play_state_shutdown(play_state_t *play)
{
  if (play == NULL)
    return;

  wave_ctrl_destroy(&play->wave_ctrl);
  path_map_destroy(&play->path_map);
  memset(&play->tower_field, 0, sizeof(play->tower_field));
}

static void play_state_step_manual_waves(play_state_t *play)
{
  if (play == NULL || !play->manual_spawning || play->wave_ctrl.level_complete)
    return;

  if (play->wave_ctrl.timer > 0)
  {
    play->wave_ctrl.timer--;
    return;
  }

  if (!play->wave_ctrl.wave_active)
  {
    play_state_open_multiplayer_wave(play);
    return;
  }

  if (play_state_multiplayer_wave_remaining_total(play) == 0 &&
      enemy_pool_empty(&play->wave_ctrl.enemies))
  {
    play->wave_ctrl.wave_active = false;
    play->wave_ctrl.current_wave++;
    play_state_clear_multiplayer_wave_budget(play);

    if (play->wave_ctrl.current_wave >= play->wave_ctrl.wave_count)
      play->wave_ctrl.level_complete = true;
    else
      play->wave_ctrl.timer = play->wave_ctrl.waves[play->wave_ctrl.current_wave].pre_wave_delay;
    return;
  }

  if (enemy_pool_empty(&play->wave_ctrl.enemies) &&
      !play_state_multiplayer_has_affordable_enemy(play))
  {
    memset(play->multiplayer_wave_remaining, 0, sizeof(play->multiplayer_wave_remaining));
  }
}

void play_state_step(play_state_t *play)
{
  if (play == NULL)
    return;

  PROFILE_BEGIN("wave");
  if (play->manual_spawning)
    play_state_step_manual_waves(play);
  else if (!wave_ctrl_waiting_for_clear(&play->wave_ctrl) ||
           projectile_pool_empty(&play->projectiles))
    wave_ctrl_step(&play->wave_ctrl);

  // Award coins when a wave completes (current_wave advanced)
  if (play->wave_ctrl.current_wave > play->prev_wave)
  {
    uint16_t reward = (uint16_t)((play->wave_bonus_base + play->wave_bonus_step * play->prev_wave) * 2);
    play->coins = (uint16_t)(play->coins + reward);
    play->prev_wave = play->wave_ctrl.current_wave;
  }
  PROFILE_END();

  PROFILE_BEGIN("tower_targeting");
  play_state_step_tower_attacks(play);
  PROFILE_END();

  PROFILE_BEGIN("tower_projectile_release");
  play_state_step_tower_projectile_releases(play);
  PROFILE_END();

  PROFILE_BEGIN("projectiles");
  play_state_step_projectiles(play);
  PROFILE_END();

  PROFILE_BEGIN("effects");
  play_state_step_effects(play);
  PROFILE_END();

  PROFILE_BEGIN("ability_heal");
  for (uint16_t active_i = 0; active_i < play->wave_ctrl.enemies.active_count; active_i++)
  {
    uint16_t i = play->wave_ctrl.enemies.active_indices[active_i];
    enemy_slot_t *slot = &play->wave_ctrl.enemies.slots[i];
    if (!slot->active || !slot->enemy.alive)
      continue;
    if (!(slot->enemy.type->abilities & ENEMY_ABILITY_HEALING))
      continue;
    if (slot->enemy.anim_tick % 30 == 0 && slot->enemy.health < slot->enemy.type->health)
      slot->enemy.health = (int16_t)(slot->enemy.health + 2);
  }
  PROFILE_END();

  PROFILE_BEGIN("enemies");
  for (uint16_t active_i = 0; active_i < play->wave_ctrl.enemies.active_count; active_i++)
  {
    uint16_t i = play->wave_ctrl.enemies.active_indices[active_i];
    enemy_slot_t *slot = &play->wave_ctrl.enemies.slots[i];
    if (slot->active && slot->enemy.alive)
    {
      uint16_t override_speed = play_state_compute_override_speed(play, i);
      enemy_step(&slot->enemy, play->path_waypoints_x, play->path_waypoints_y, play->path_waypoint_count, override_speed);

      if (slot->enemy.reached_goal)
        play_state_on_enemy_leaked(play, &slot->enemy);
    }
  }
  PROFILE_END();

  PROFILE_BEGIN("cleanup_enemies");
  wave_ctrl_deactivate_inactive_enemies(&play->wave_ctrl);
  PROFILE_END();

  PROFILE_BEGIN("tower_anim");
  tower_field_step(&play->tower_field);
  PROFILE_END();
}

play_result_t play_state_get_result(const play_state_t *play)
{
  if (play == NULL)
    return PLAY_RESULT_RUNNING;

  if (play->base_hp <= 0)
    return PLAY_RESULT_GAME_OVER;
    
  if (play->wave_ctrl.level_complete && enemy_pool_empty(&play->wave_ctrl.enemies))
    return PLAY_RESULT_VICTORY;

  return PLAY_RESULT_RUNNING;
}
