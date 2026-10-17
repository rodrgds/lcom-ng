#ifndef ENTITY_H
#define ENTITY_H

#include <lcom/lcf.h>
#include <stdint.h>

/// @brief Draws a xpm file int the screen with top left corner in position x,y
/// @param xpm XPM to draw
/// @param x position x of top left corner
/// @param y position y of top left corner
/// @return 0 if success, 1 if no success
int draw_xpm(xpm_map_t xpm, int x, int y);

#endif /* ENTITY_H */
