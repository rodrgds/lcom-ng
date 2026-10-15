#ifndef GAME_VIEWER_H
#define GAME_VIEWER_H

#include "game.h"

typedef struct GameViewer GameViewer;

/**
 * @brief Creates all viewer state needed to render the game.
 * @return Viewer pointer, or NULL on allocation failure.
 */
GameViewer *game_viewer_create(void);

/**
 * @brief Destroys a game viewer.
 * @param gv Viewer to destroy. NULL is accepted.
 */
void game_viewer_destroy(GameViewer *gv);

/**
 * @brief Draws only the world scene without transient overlays.
 * @param gv Viewer state.
 * @param context Game context to render.
 */
void game_viewer_draw_scene(GameViewer *gv, const GameContext *context);

/**
 * @brief Draws a complete normal game frame.
 * @param gv Viewer state.
 * @param context Game context to render.
 */
void game_viewer_draw(GameViewer *gv, const GameContext *context);

/**
 * @brief Draws a game frame with the active weapon aim overlay.
 * @param gv Viewer state.
 * @param context Game context to render.
 * @param aim_angle Current aim angle in radians.
 * @param aim_power Current aim power percentage.
 */
void game_viewer_draw_aimed(GameViewer *gv, const GameContext *context, double aim_angle, int aim_power);

#endif /* GAME_VIEWER_H */
