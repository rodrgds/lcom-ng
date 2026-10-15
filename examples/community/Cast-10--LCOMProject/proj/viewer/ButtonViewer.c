#include "ButtonViewer.h"

ButtonViewer *buttonViewerCreate(xpm_map_t normal_xpm, xpm_map_t hover_xpm) {
    ButtonViewer *viewer = malloc(sizeof(ButtonViewer));
    if (!viewer) return NULL;

    viewer->normal = spriteCreate(normal_xpm);
    viewer->hover = spriteCreate(hover_xpm);

    if (!viewer->normal || !viewer->hover) {
        buttonViewerDestroy(viewer);
        return NULL;
    }

    return viewer;
}

void buttonViewerDestroy(ButtonViewer *viewer) {
    if (!viewer) return;

    if (viewer->normal) spriteDestroy(viewer->normal);
    if (viewer->hover) spriteDestroy(viewer->hover);

    free(viewer);
}

void buttonViewerDraw(ButtonViewer *viewer, ButtonState *button, int x, int y) {
    if (!viewer || !button) return;

    button->x = x;
    button->y = y;
    button->width = getWidth(viewer->normal);
    button->height = getHeight(viewer->normal);

    if (button->hovered) {
        spriteDraw(viewer->hover, button->x, button->y);
    } else {
        spriteDraw(viewer->normal, button->x, button->y);
    }
}

