#include "background.h"
#include "v_card.h"
#include "game_state.h"
#include <stdio.h>
#include "sprites/world/grass_grid.xpm"

static xpm_image_t grass_grid_img;
static uint8_t *grass_grid_pixmap = NULL;

void background_init() {
    grass_grid_pixmap = xpm_load((xpm_map_t) grass_grid_xpm, XPM_8_8_8_8, &grass_grid_img);
    if (grass_grid_pixmap == NULL) {
        printf("Error loading background xpm.\n");
    }
}

void background_render() {
    if (grass_grid_pixmap != NULL) {
        vg_draw_xpm(grass_grid_img, grass_grid_pixmap, 0, 100);
    }
}
