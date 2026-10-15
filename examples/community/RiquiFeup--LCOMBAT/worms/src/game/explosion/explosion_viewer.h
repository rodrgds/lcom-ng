#ifndef EXPLOSION_VIEWER_H
#define EXPLOSION_VIEWER_H

#include "explosion.h"

/**
 * @brief Draws the explosion on the screen.
 * 
 * Maps the explosion's current tick to the corresponding animation frame 
 * from the assets and renders it at the explosion's coordinates.
 * 
 * @param e Pointer to the active explosion model.
 */
void explosion_viewer_draw(const Explosion *e);

#endif /* EXPLOSION_VIEWER_H */
