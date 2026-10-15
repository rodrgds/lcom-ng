#include "shopBarViewer.h"

ShopBarViewer *shopBarViewerCreate() {
    ShopBarViewer *viewer = malloc(sizeof(ShopBarViewer));
    if (!viewer) return NULL;

    viewer->border_color = 0xCCCCCC;
    viewer->empty_color = 0x000000;
    viewer->fill_color = 0x00AA00;
    viewer->buy_hover_color = 0xFFAA00;
    viewer->buy_disabled_color = 0x888888;

    viewer->buttonViewer = buttonViewerCreate(buy_button_xpm, buy_button_hover_xpm);

    return viewer;
}

void shopBarViewerDestroy(ShopBarViewer *viewer) {
    if (!viewer) return;

    free(viewer);
}

void shopBarViewerDraw(ShopBarViewer *viewer, ShopBarState *bar) {
    if (!viewer || !bar || !bar->buyButton) return;

    int segmentWidth = bar->width / SHOP_BAR_MAX_LEVEL;

    vg_draw_rectangle(
        bar->x - 3,
        bar->y - 3,
        bar->width + 6,
        bar->height + 6,
        viewer->border_color
    );

    vg_draw_rectangle(
        bar->x,
        bar->y,
        bar->width,
        bar->height,
        viewer->empty_color
    );

    for (int i = 0; i < bar->level; i++) {
        vg_draw_rectangle(
            bar->x + i * segmentWidth + 2,
            bar->y + 2,
            segmentWidth - 4,
            bar->height - 4,
            viewer->fill_color
        );
    }

    int btn_h = getHeight(viewer->buttonViewer->normal);
    int btn_y = bar->y + (bar->height - btn_h) / 2;
    buttonViewerDraw(viewer->buttonViewer, bar->buyButton, bar->x + bar->width + 10, btn_y);

}

