#include "MenuViewer.h"
#define PLAY_BUTTON_X 320
#define PLAY_BUTTON_Y 470

MenuViewer *menuViewerCreate() {
    MenuViewer *viewer = malloc(sizeof(MenuViewer));
    if (!viewer) return NULL;

    viewer->playButtonViewer = NULL;

    viewer->playButtonViewer = buttonViewerCreate(
        (xpm_map_t) play_button_xpm,
        (xpm_map_t) play_button_hover_xpm
    );

    if (!viewer->playButtonViewer) {
        menuViewerDestroy(viewer);
        return NULL;
    }

    return viewer;
}

void menuViewerDestroy(MenuViewer *viewer) {
    if (!viewer) return;

    if (viewer->playButtonViewer) {
        buttonViewerDestroy(viewer->playButtonViewer);
    }

    free(viewer);
}

void menuViewerDraw(MenuViewer *viewer, MenuState *menu) {
    if (!viewer || !menu) return;

    if (menu->playButton) {
        buttonViewerDraw(viewer->playButtonViewer, menu->playButton, PLAY_BUTTON_X, PLAY_BUTTON_Y);
    }
}
