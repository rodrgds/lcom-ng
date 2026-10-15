#ifndef WATER_H
#define WATER_H

#include <stdint.h>

typedef struct Water Water;

/**
 * @brief Creates a water model at the specified Y level.
 * @param initial_level The Y coordinate where the water begins.
 * @return Pointer to the Water, or NULL on allocation failure.
 */
Water* water_create(int32_t initial_level);

/**
 * @brief Destroys the water model.
 * @param w Pointer to the water to destroy.
 */
void water_destroy(Water *w);

/**
 * @brief Gets the current Y level of the water.
 * @param w Pointer to the water.
 * @return The Y coordinate of the water surface.
 */
int32_t water_get_level(const Water *w);

#endif /* WATER_H */
