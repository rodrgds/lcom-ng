#ifndef WORM_VIEWER_H
#define WORM_VIEWER_H

#include "worm.h"

/**
 * @brief Opaque visual representation of a Worm.
 *
 * Manages all AnimatedSprites for one worm (idle variations, walk,
 * jump, die) and selects the correct animation based on the Worm
 * model's state and direction.  Follows MVC: the Viewer reads the
 * Model via const getters and delegates drawing to the renderer.
 */
typedef struct WormViewer WormViewer;

/**
 * @brief Creates a WormViewer, building AnimatedSprites from the
 *        globally loaded assets.
 *
 * Must be called AFTER assets_init().
 *
 * @return Pointer to the new WormViewer, or NULL on allocation failure.
 */
WormViewer *worm_viewer_create(void);

/**
 * @brief Frees the WormViewer and all its AnimatedSprite instances.
 *        The underlying Sprite pixel data (owned by assets) is NOT freed.
 * @param wv WormViewer to destroy.  NULL is accepted (no-op).
 */
void worm_viewer_destroy(WormViewer *wv);

/**
 * @brief Advances the animation by one tick and draws the correct
 *        frame at the worm's current position.
 *
 * Reads state, direction and position from the Worm model (const).
 * When an idle animation finishes, a new random idle is selected.
 *
 * @param wv   WormViewer to update and draw.  NULL is accepted (no-op).
 * @param worm Worm model to read from.  NULL is accepted (no-op).
 */
void worm_viewer_draw(WormViewer *wv, const Worm *worm);

#endif /* WORM_VIEWER_H */
