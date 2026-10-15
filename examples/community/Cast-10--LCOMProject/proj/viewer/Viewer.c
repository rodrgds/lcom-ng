#include "Viewer.h"

Viewer *viewerCreate() {
    Viewer *v = malloc(sizeof(Viewer));
    if (!v) return NULL;

    v->cursor = NULL;
    v->bgMenu = NULL;
    v->bgArena = NULL;
    v->bgShop = NULL;
    v->bgGameOverBlue = NULL;
    v->bgGameOverRed = NULL;
    v->gameViewer = NULL;
    v->menuViewer = NULL;
    v->shopViewer = NULL;
    v->gameOverViewer = NULL;

    v->cursor = spriteCreate((xpm_map_t) cursor_xpm);
    if (!v->cursor) { viewerDestroy(v); return NULL; }

    v->bgMenu = spriteCreate((xpm_map_t) home_xpm);
    if (!v->bgMenu) { viewerDestroy(v); return NULL; }

    v->bgArena = spriteCreate((xpm_map_t) background_xpm);
    if (!v->bgArena) { viewerDestroy(v); return NULL; }

    v->bgShop = spriteCreate((xpm_map_t) shop_xpm);
    if (!v->bgShop) { viewerDestroy(v); return NULL; }


    v->bgGameOverBlue = spriteCreate((xpm_map_t) p1_lose_xpm);
    if (!v->bgGameOverBlue) { viewerDestroy(v); return NULL; }

    v->bgGameOverRed = spriteCreate((xpm_map_t) p2_lose_xpm);
    if (!v->bgGameOverRed) { viewerDestroy(v); return NULL; }

    v->gameViewer = gameViewerCreate();
    if (!v->gameViewer) { viewerDestroy(v); return NULL; }

    v->menuViewer = menuViewerCreate();
    if (!v->menuViewer) { viewerDestroy(v); return NULL; }

    v->shopViewer = shopViewerCreate();
    if (!v->shopViewer) { viewerDestroy(v); return NULL; }

    v->gameOverViewer = gameOverViewerCreate();
    if (!v->gameOverViewer) { viewerDestroy(v); return NULL; }

    return v;
}

void viewerDestroy(Viewer *v) {
    if (!v) return;

    if (v->cursor) spriteDestroy(v->cursor);
    if (v->bgMenu) spriteDestroy(v->bgMenu);
    if (v->bgArena) spriteDestroy(v->bgArena);
    if (v->bgShop) spriteDestroy(v->bgShop);
    if (v->bgGameOverBlue) spriteDestroy(v->bgGameOverBlue);
    if (v->bgGameOverRed) spriteDestroy(v->bgGameOverRed);
    if (v->gameViewer) gameViewerDestroy(v->gameViewer);
    if (v->menuViewer) menuViewerDestroy(v->menuViewer);
    if (v->shopViewer) shopViewerDestroy(v->shopViewer);
    if (v->gameOverViewer) gameOverViewerDestroy(v->gameOverViewer);

    free(v);
}

/**
 * @brief Returns the raw pixel data of the background sprite for the current game state.
 * @param v Viewer owning the background sprites.
 * @param state Application state selecting which background to use.
 * @return Pointer to the background pixel buffer, or NULL if v or state is NULL.
 */
static const uint8_t *viewerGetBackground(Viewer *v, State *state) {
    if (!v || !state) return NULL;

    switch (state->state) {
        case STATE_MENU:
            return spriteGetMap(v->bgMenu);
        case STATE_PLAYING:
            return spriteGetMap(v->bgArena);
        case STATE_PAUSED:
            return spriteGetMap(v->bgShop);
        case STATE_GAME_OVER:
            if (state->winner == TEAM_BLUE)     return spriteGetMap(v->bgGameOverRed);
            else if (state->winner == TEAM_RED) return spriteGetMap(v->bgGameOverBlue);
            return NULL;
        default:
            return NULL;
    }
}

void viewerDraw(Viewer *v, State *state, int cursor_x, int cursor_y) {
    if (!v || !state) return;

    video_clear_back_buffer(viewerGetBackground(v, state));

    switch (state->state) {
        case STATE_MENU:
            if (state->menu) {
                menuViewerDraw(v->menuViewer, state->menu);
            }
            break;

        case STATE_PLAYING:
            if (state->game) {
                gameViewerDraw(
                    v->gameViewer,
                    state->game->p1,
                    state->game->p2,
                    &state->game->minions,
                    &state->game->towers
                );
            }
            break;

        case STATE_PAUSED:

            if (state->shop) {
                shopViewerDraw(v->shopViewer, state->shop,
                               championCurrentPoints(state->game->p1),
                               championCurrentPoints(state->game->p2));
            }

            break;

        case STATE_GAME_OVER:
            gameOverViewerDraw(v->gameOverViewer, state);
            break;
    }

    if (state->state != STATE_PLAYING) {
        spriteDraw(v->cursor, cursor_x, cursor_y);
    }

    video_swap_buffers();
}
