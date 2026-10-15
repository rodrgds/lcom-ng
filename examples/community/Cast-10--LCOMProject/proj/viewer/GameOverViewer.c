#include "GameOverViewer.h"

#define RESTART_BUTTON_X 280
#define RESTART_BUTTON_Y 540

#define QUIT_BUTTON_X 525
#define QUIT_BUTTON_Y 540

#define COLOR_WHITE  0xFFFFFF
#define COLOR_RED    0xFF3333
#define COLOR_BLUE   0x3399FF
#define COLOR_GOLD   0xFFD700

GameOverViewer *gameOverViewerCreate() {
    GameOverViewer *viewer = malloc(sizeof(GameOverViewer));
    if (!viewer) return NULL;

    viewer->restartButtonViewer = NULL;
    viewer->quitButtonViewer = NULL;

    viewer->restartButtonViewer = buttonViewerCreate(
        (xpm_map_t) retry_button_xpm,
        (xpm_map_t) retry_button_hover_xpm
    );

    if (!viewer->restartButtonViewer) {
        gameOverViewerDestroy(viewer);
        return NULL;
    }

    viewer->quitButtonViewer = buttonViewerCreate(
        (xpm_map_t)quit_button_xpm, (xpm_map_t)quit_button_hover_xpm);

    if (!viewer->quitButtonViewer) {
        gameOverViewerDestroy(viewer);
        return NULL;
    }

    return viewer;
}

void gameOverViewerDestroy(GameOverViewer *viewer) {
    if (!viewer) return;

    if (viewer->restartButtonViewer) {
        buttonViewerDestroy(viewer->restartButtonViewer);
    }

    if (viewer->quitButtonViewer) {
        buttonViewerDestroy(viewer->quitButtonViewer);
    }

    free(viewer);
}

void gameOverViewerDraw(GameOverViewer *viewer, State *state) {
    if (!viewer || !state || !state->gameOver) return;

   
    buttonViewerDraw(viewer->restartButtonViewer, state->gameOver->restartButton, RESTART_BUTTON_X, RESTART_BUTTON_Y);
    buttonViewerDraw(viewer->quitButtonViewer, state->gameOver->quitButton, QUIT_BUTTON_X, QUIT_BUTTON_Y);

}
