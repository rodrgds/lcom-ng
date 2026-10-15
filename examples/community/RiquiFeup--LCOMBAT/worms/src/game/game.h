#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdint.h>

#include "../engine/event/event_queue.h"
#include "cursor/cursor.h"
#include "worm/worm.h"
#include "projectile/projectile.h"
#include "weapons/bazooka/bazooka.h"
#include "explosion/explosion.h"
#include "weapons/grenade/grenade.h"
#include "water/water.h"

#define MAX_EXPLOSIONS 5

typedef struct GameContext GameContext;

/**
 * @brief Creates a new game context with worms, cursor, queue, and water.
 * @return Context pointer, or NULL on allocation failure.
 */
GameContext *game_context_create(void);

/**
 * @brief Destroys a game context and all owned game objects.
 * @param context Context to destroy. NULL is accepted.
 */
void game_context_destroy(GameContext *context);

/**
 * @brief Gets an explosion slot by index.
 * @param context Game context.
 * @param index Explosion slot index.
 * @return Explosion pointer, or NULL for invalid input or empty slot.
 */
Explosion *game_context_get_explosion(const GameContext *context, int index);

/**
 * @brief Adds an explosion to the first available slot.
 * @param context Game context.
 * @param e Explosion to take ownership of. NULL is ignored.
 */
void game_context_add_explosion(GameContext *context, Explosion *e);

/**
 * @brief Gets the water model.
 * @param context Game context.
 * @return Water pointer, or NULL for invalid context.
 */
const Water *game_context_get_water(const GameContext *context);

/**
 * @brief Gets the shared event queue.
 * @param context Game context.
 * @return Event queue pointer, or NULL for invalid context.
 */
EventQueue *game_context_get_queue(GameContext *context);

/**
 * @brief Gets the cursor model.
 * @param context Game context.
 * @return Cursor pointer, or NULL for invalid context.
 */
Cursor *game_context_get_cursor(const GameContext *context);

/**
 * @brief Gets a worm by global index.
 * @param context Game context.
 * @param index Global worm index.
 * @return Worm pointer, or NULL for invalid input.
 */
Worm *game_context_get_worm(const GameContext *context, int index);

/**
 * @brief Gets the team whose turn is currently active.
 * @param context Game context.
 * @return Current team index, or 0 for invalid context.
 */
int game_context_get_current_team(const GameContext *context);

/**
 * @brief Gets the global worm index selected by the active team.
 * @param context Game context.
 * @return Active worm index, or fallback index for invalid state.
 */
int game_context_get_active_worm_index(const GameContext *context);

/**
 * @brief Gets the worm currently controlled by the active team.
 * @param context Game context.
 * @return Active worm pointer, or NULL for invalid context.
 */
Worm *game_context_get_active_worm(const GameContext *context);

/**
 * @brief Gets the team that owns a global worm index.
 * @param worm_index Global worm index.
 * @return Team index, or -1 for invalid index.
 */
int game_context_get_worm_team(int worm_index);

/**
 * @brief Checks whether a team has at least one living worm.
 * @param context Game context.
 * @param team Team index.
 * @return true if the team has a living worm, false otherwise.
 */
bool game_context_is_team_alive(const GameContext *context, int team);

/**
 * @brief Selects a living worm from the current team.
 * @param context Game context.
 * @param worm_index Global worm index.
 * @return 0 on success, 1 on invalid selection.
 */
int game_context_select_worm(GameContext *context, int worm_index);

/**
 * @brief Selects a living current-team worm containing a screen position.
 *
 * @param context Game context. NULL is rejected.
 * @param x Screen X coordinate. Negative values are rejected.
 * @param y Screen Y coordinate. Negative values are rejected.
 * @return 0 if a worm was selected, 1 otherwise.
 */
int game_context_select_worm_at_position(GameContext *context, int16_t x, int16_t y);

/**
 * @brief Advances to the next living team's turn and resets the timer.
 * @param context Game context.
 */
void game_context_next_team_turn(GameContext *context);

/**
 * @brief Gets the generic active projectile.
 * @param context Game context.
 * @return Projectile pointer, or NULL if none.
 */
Projectile *game_context_get_projectile(const GameContext *context);

/**
 * @brief Sets the generic active projectile and destroys the previous one.
 * @param context Game context.
 * @param projectile Projectile to take ownership of.
 */
void game_context_set_projectile(GameContext *context, Projectile *projectile);

/**
 * @brief Gets the active bazooka shot.
 * @param context Game context.
 * @return Bazooka pointer, or NULL if none.
 */
Bazooka *game_context_get_bazooka(const GameContext *context);

/**
 * @brief Sets the active bazooka shot and destroys the previous one.
 * @param context Game context.
 * @param bazooka Bazooka to take ownership of.
 */
void game_context_set_bazooka(GameContext *context, Bazooka *bazooka);

/**
 * @brief Retrieves the active grenade from the game context.
 * @param context Pointer to the GameContext.
 * @return Pointer to the Grenade.
 */
Grenade *game_context_get_grenade(const GameContext *context);

/**
 * @brief Sets the active grenade in the game context.
 * @param context Pointer to the GameContext.
 * @param grenade Pointer to the Grenade.
 */
void game_context_set_grenade(GameContext *context, Grenade *grenade);

/**
 * @brief Gets remaining ticks in the current turn.
 * @param context Game context.
 * @return Remaining ticks, or 0 for invalid context.
 */
uint32_t game_context_ticks_left(const GameContext *context);

/**
 * @brief Advances the turn timer by one tick.
 * @param context Game context.
 */
void game_context_tick(GameContext *context);

/**
 * @brief Resets the current turn timer to the configured duration.
 * @param context Game context.
 */
void game_context_reset_turn(GameContext *context);

/**
 * @brief Resets game objects for a new match.
 * @param context Game context.
 */
void game_context_reset(GameContext *context);

#endif
