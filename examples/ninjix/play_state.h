/**
 * @file play_state.h
 * @brief Gameplay state -- simulation tick, tower placement, and session data.
 * @ingroup group_app
 *
 * Owns the active session: towers, enemies, wave progress, placement mode,
 * projectiles, coins, base HP, and active effects.  The scene module reads
 * this state for rendering; the app module drives it via play_state_step().
 */

#ifndef PROJ_PLAY_STATE_H
#define PROJ_PLAY_STATE_H

#include <stdbool.h>
#include <stdint.h>

#include "enemy_path.h"
#include "path_map.h"
#include "enemy_pool.h"
#include "projectile_pool.h"
#include "resources.h"
#include "tower_field.h"
#include "placement.h"
#include "wave.h"

#define MAX_ACTIVE_EFFECTS 64

typedef struct {
  bool active;
  enemy_handle_t target;
  uint8_t effect_type;
  uint16_t ticks_left;
  uint8_t strength;
  uint8_t tick_interval;
  uint8_t sub_tick;
} active_effect_t;

typedef enum {
  PLAY_RESULT_RUNNING,
  PLAY_RESULT_GAME_OVER,
  PLAY_RESULT_VICTORY
} play_result_t;

typedef struct {
  uint16_t level_id;
  path_map_t path_map;
  int16_t path_waypoints_x[MAX_WAYPOINTS];
  int16_t path_waypoints_y[MAX_WAYPOINTS];
  uint16_t path_waypoint_count;
  tower_field_t tower_field;
  wave_controller_t wave_ctrl;
  projectile_pool_t projectiles;
  active_effect_t active_effects[MAX_ACTIVE_EFFECTS];
  uint16_t base_hp;
  uint16_t base_max_hp;
  uint16_t coins;
  uint16_t elixir;
  uint16_t enemies_killed;
  uint8_t speed_multiplier;
  bool manual_spawning;
  uint16_t multiplayer_wave_remaining[ENEMY_KIND_COUNT];
  uint16_t multiplayer_wave_limit;
  uint16_t multiplayer_wave_sent;
  int16_t selected_tower;
  uint16_t prev_wave;
  uint16_t wave_bonus_base;
  uint16_t wave_bonus_step;
  bool wave_running;
  bool placement_active;
  tower_kind_t placement_kind;
  bool placement_hover_valid;
  int16_t placement_hover_col;
  int16_t placement_hover_row;
  tile_placement_result_t placement_hover_result;
} play_state_t;

/**
 * @brief Initialise a play session for the given level.
 * @param play          Play state to initialise (must be heap/stack allocated).
 * @param resources     Loaded resource handles (sprites, enemy types).
 * @param level_id      Zero-based level index (selects map + wave schedule).
 * @return 0 on success, non-zero on failure (caller must call play_state_shutdown).
 */
int play_state_init(play_state_t *play, const resources_t *resources,
                    uint16_t level_id);

/**
 * @brief Release all resources held by a play session.
 * @param play  Play state to tear down.  Safe to call on a zeroed struct.
 */
void play_state_shutdown(play_state_t *play);

/**
 * @brief Advance the gameplay simulation by one tick (60 Hz).
 *
 * Steps the wave controller, tower targeting, projectile movement,
 * effect application, enemy movement, tower animation, and result detection.
 *
 * @param play  Active play state.
 */
void play_state_step(play_state_t *play);

/**
 * @brief Query whether the session has ended.
 * @param play  Active play state.
 * @return PLAY_RESULT_RUNNING, PLAY_RESULT_GAME_OVER, or PLAY_RESULT_VICTORY.
 */
play_result_t play_state_get_result(const play_state_t *play);

/**
 * @brief Enter tower-placement mode for the given tower kind.
 * @param play  Active play state.
 * @param kind  Tower kind selected from the shop.
 */
void play_state_begin_placement(play_state_t *play, tower_kind_t kind);

/**
 * @brief Cancel the current placement mode without placing a tower.
 * @param play  Active play state.
 */
void play_state_cancel_placement(play_state_t *play);

/**
 * @brief Update the placement hover position as the mouse moves.
 *
 * Recalculates tile coordinates and validates whether the hovered tile
 * can accept a tower, storing the result in play->placement_hover_*.
 *
 * @param play    Active play state.
 * @param pixel_x Mouse X in screen pixels.
 * @param pixel_y Mouse Y in screen pixels.
 */
void play_state_update_placement_hover(play_state_t *play, int16_t pixel_x, int16_t pixel_y);

/**
 * @brief Attempt to place the selected tower at the given pixel position.
 *
 * Validates the tile, checks coin balance, deducts the cost, and adds the
 * tower to the tower field.  Returns 0 on success.
 *
 * @param play    Active play state.
 * @param pixel_x Mouse X in screen pixels.
 * @param pixel_y Mouse Y in screen pixels.
 * @return 0 if the tower was placed, non-zero otherwise.
 */
int play_state_try_place_tower(play_state_t *play, int16_t pixel_x, int16_t pixel_y);
/**
 * @brief Enable or disable manual enemy spawning mode.
 *
 * When enabled, waves are opened manually and enemies are spawned using
 * play_state_spawn_enemy(). The session is reset for multiplayer-style
 * gameplay, including elixir economy and wave budgets.
 *
 * @param play             Active play state.
 * @param manual_spawning  true to enable manual spawning mode, false to
 *                         restore normal wave-controller behaviour.
 */
void play_state_set_manual_spawning(play_state_t *play, bool manual_spawning);
/**
 * @brief Place a tower directly at world coordinates.
 *
 * Bypasses placement-mode interaction and immediately attempts to add a
 * tower to the tower field. Intended for editor, setup, or multiplayer
 * synchronisation use cases.
 *
 * @param play  Active play state.
 * @param kind  Tower type to place.
 * @param x     Tower X position in pixels.
 * @param y     Tower Y position in pixels.
 * @return 0 on success, non-zero if placement failed.
 */
int play_state_place_tower_direct(play_state_t *play, tower_kind_t kind, int16_t x, int16_t y);
/**
 * @brief Spawn an enemy of the specified type.
 *
 * Creates an enemy at the map spawn location. In manual-spawning mode,
 * elixir cost, wave budget, and wave state are validated before spawning.
 *
 * @param play  Active play state.
 * @param kind  Enemy type to spawn.
 * @return 0 on success, non-zero if spawning was not permitted or no slot
 *         was available.
 */
int play_state_spawn_enemy(play_state_t *play, enemy_kind_t kind);
/**
 * @brief Spawn an enemy for a specific wave.
 *
 * In manual-spawning mode, automatically opens the requested wave if it is
 * the current pending wave, then attempts to spawn the enemy.
 *
 * @param play        Active play state.
 * @param kind        Enemy type to spawn.
 * @param wave_index  Wave index associated with the spawn request.
 * @return 0 on success, non-zero on failure.
 */
int play_state_spawn_enemy_in_wave(play_state_t *play, enemy_kind_t kind, uint16_t wave_index);
/**
 * @brief Get the elixir cost of spawning an enemy type.
 *
 * @param kind  Enemy type.
 * @return Elixir cost for the specified enemy, or 0 if the type is invalid.
 */
uint16_t play_state_enemy_elixir_cost(enemy_kind_t kind);
/**
 * @brief Query remaining wave budget for a specific enemy type.
 *
 * In manual-spawning mode, each wave has a limited number of enemies that
 * may be spawned. This function reports the remaining count for a single
 * enemy type.
 *
 * @param play  Active play state.
 * @param kind  Enemy type.
 * @return Remaining number of enemies of that type available this wave.
 */
uint16_t play_state_multiplayer_enemy_remaining(const play_state_t *play, enemy_kind_t kind);
/**
 * @brief Query total remaining enemy budget for the current wave.
 *
 * Sums all remaining spawn counts across enemy types in manual-spawning mode.
 *
 * @param play  Active play state.
 * @return Total number of enemies still available to spawn in the current wave.
 */
uint16_t play_state_multiplayer_wave_remaining_total(const play_state_t *play);
#endif
