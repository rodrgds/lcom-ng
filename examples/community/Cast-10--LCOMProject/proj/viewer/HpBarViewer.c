#include "HpBarViewer.h"

HpBarViewer *createHpBarViewer(void) {
    HpBarViewer *viewer = malloc(sizeof(HpBarViewer));
    if (!viewer) return NULL;

    viewer->border_color = 0x000000;
    viewer->empty_color = 0x444444;
    viewer->high_color = 0x00CC00;
    viewer->mid_color = 0xFFAA00;
    viewer->low_color = 0xCC0000;

    return viewer;
}

void destroyHpBarViewer(HpBarViewer *viewer) {
    free(viewer);
}

void hpBarViewerDraw(HpBarViewer *viewer, int x, int y, int width, int current_hp, int max_hp) {
    if (!viewer || width <= 0 || max_hp <= 0) return;

    int bar_y = y - HP_BAR_HEIGHT - HP_BAR_OFFSET;
    if (bar_y < 0 || x < 0) return;

    int inner_w = width - 2;
    int inner_h = HP_BAR_HEIGHT - 2;

    int hp = current_hp < 0 ? 0 : (current_hp > max_hp ? max_hp : current_hp);
    int filled = inner_w > 0 ? (hp * inner_w) / max_hp : 0;

    int pct = (hp * 100) / max_hp;
    uint32_t fill_color = pct > 60 ? viewer->high_color
                        : pct > 25 ? viewer->mid_color
                        : viewer->low_color;

    vg_draw_rectangle(x, bar_y, width, HP_BAR_HEIGHT, viewer->border_color);
    vg_draw_rectangle(x + 1, bar_y + 1, inner_w, inner_h, viewer->empty_color);
    if (filled > 0)
        vg_draw_rectangle(x + 1, bar_y + 1, filled, inner_h, fill_color);
}
