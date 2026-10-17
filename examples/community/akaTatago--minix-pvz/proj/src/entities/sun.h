/**
 * @file sun.h
 * @brief Logic and rendering for sun currency.
 */

#ifndef __SUN_H
#define __SUN_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_SUNS 10

/**
 * @brief Structure defining a sun entity.
 */
typedef struct {
    bool active;        /**< @brief Indicates if the sun is active in the game. */
    int16_t x, y;       /**< @brief Current x and y coordinates. */
    int16_t target_y;   /**< @brief The y coordinate where the sun should stop falling. */
    uint16_t lifetime;  /**< @brief Remaining frames before the sun disappears. */
} sun_t;

/**
 * @brief Initializes the suns array and loads sprites.
 */
void suns_init();

/**
 * @brief Spawns a sun at a specific location, falling to a target y-coordinate.
 * * @param x The x-coordinate.
 * @param start_y The initial y-coordinate.
 * @param target_y The final y-coordinate.
 */
void spawn_sun(int16_t x, int16_t start_y, int16_t target_y);

/**
 * @brief Updates the falling logic, lifetime, and periodic spawning of suns.
 */
void suns_update();

/**
 * @brief Renders all active suns.
 */
void suns_render();

/**
 * @brief Checks if an active sun is clicked on.
 * * @param mx The mouse x-coordinate.
 * @param my The mouse y-coordinate.
 * @return true if a sun was clicked and collected, false otherwise.
 */
bool suns_check_click(int16_t mx, int16_t my);

#endif
