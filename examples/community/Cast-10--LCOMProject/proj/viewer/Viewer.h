#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "../assets/game/arena.h"
#include "../assets/cursor/cursor_xpms.h"
#include "../assets/menu/home.h"
#include "../assets/game_over/p1_lose_xpm.h"
#include "../assets/game_over/p2_lose_xpm.h"
#include "../assets/pause/shop_xpm.h"
#include "GameOverViewer.h"
#include "GameViewer.h"
#include "MenuViewer.h"
#include "ShopViewer.h"
#include "sprite/Sprite.h"
#include "video.h"

/**
 * @brief Top-level renderer that owns all sub-viewers and background sprites.
 */
typedef struct {
    Sprite *cursor;           /**< Mouse cursor sprite. */
    Sprite *bgMenu;           /**< Background for the main menu. */
    Sprite *bgArena;          /**< Background for the in-match arena. */
    Sprite *bgShop;           /**< Background for the shop screen. */
    Sprite *bgGameOverBlue;   /**< Game-over background displayed when blue wins. */
    Sprite *bgGameOverRed;    /**< Game-over background displayed when red wins. */
    GameViewer *gameViewer;       /**< In-match entity renderer. */
    MenuViewer *menuViewer;       /**< Main menu renderer. */
    ShopViewer *shopViewer;       /**< Shop screen renderer. */
    GameOverViewer *gameOverViewer; /**< Game-over screen renderer. */
} Viewer;

/**
 * @brief Allocates and initialises a Viewer, loading all background sprites and sub-viewers.
 * @return Pointer to the new Viewer, or NULL on failure.
 */
Viewer *viewerCreate();

/**
 * @brief Frees a Viewer and all sub-viewers and sprites it owns.
 * @param v Viewer to destroy.
 */
void viewerDestroy(Viewer *v);

/**
 * @brief Clears the back buffer with the appropriate background and draws the current game state.
 * Draws the cursor sprite in all states except STATE_PLAYING.
 * @param v Viewer supplying all renderers and background sprites.
 * @param state Application state selecting which sub-viewer to invoke.
 * @param cursor_x Current cursor X coordinate in pixels.
 * @param cursor_y Current cursor Y coordinate in pixels.
 */
void viewerDraw(Viewer *v, State *state, int cursor_x, int cursor_y);
