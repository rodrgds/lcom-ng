#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Opaque type representing an explosion model.
 */
typedef struct Explosion Explosion;

/**
 * @brief Creates a new explosion model.
 * @param x X coordinate of the explosion center.
 * @param y Y coordinate of the explosion center.
 * @param duration_ticks Total number of ticks this explosion will remain active.
 * @return Pointer to the new explosion, or NULL on failure.
 */
Explosion* explosion_create(int32_t x, int32_t y, uint8_t duration_ticks);

/**
 * @brief Destroys an explosion model.
 * @param e Pointer to the explosion to destroy. NULL is accepted.
 */
void explosion_destroy(Explosion *e);

/**
 * @brief Gets the X coordinate of the explosion.
 */
int32_t explosion_get_x(const Explosion *e);

/**
 * @brief Gets the Y coordinate of the explosion.
 */
int32_t explosion_get_y(const Explosion *e);

/**
 * @brief Gets the current tick count of the explosion.
 */
uint8_t explosion_get_current_tick(const Explosion *e);

/**
 * @brief Gets the total duration of the explosion in ticks.
 */
uint8_t explosion_get_total_ticks(const Explosion *e);

/**
 * @brief Checks if the explosion is still active.
 * @return true if current_tick < duration_ticks, false otherwise.
 */
bool explosion_is_active(const Explosion *e);

/**
 * @brief Advances the explosion internal timer by one tick.
 */
void explosion_advance_tick(Explosion *e);

#endif /* EXPLOSION_H */
