#ifndef BAZOOKA_VIEWER_H
#define BAZOOKA_VIEWER_H

#include "bazooka.h"

/**
 * @brief Draws the bazooka projectile on the screen.
 * 
 * Computes the correct sprite rotation based on velocity and aligns it with the hitbox.
 *
 * @param bazooka Pointer to the Bazooka to draw.
 */
void bazooka_viewer_draw(const Bazooka *bazooka);

#endif /* BAZOOKA_VIEWER_H */
