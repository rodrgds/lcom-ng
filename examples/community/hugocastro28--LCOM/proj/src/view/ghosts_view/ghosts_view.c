#include <lcom/lcf.h>

#include <stdint.h>

#include "../../devices/graphics/vbe.h"
#include "../../devices/graphics/graphics.h"
#include "../../model/ghosts_model/ghost.h"
#include "../initialize_pixmaps.h"
#include "ghosts_view.h"

extern Ghost ghosts[4];
extern uint8_t* ghost_red_pixmap;
extern uint8_t* ghost_orange_pixmap;
extern uint8_t* ghost_pink_pixmap;
extern uint8_t* ghost_blue_pixmap;
extern xpm_image_t ghost_red_info;
extern xpm_image_t ghost_orange_info;
extern xpm_image_t ghost_pink_info;
extern xpm_image_t ghost_blue_info;

void (draw_ghosts)() {
    draw_xpm(ghost_red_pixmap, ghost_red_info, ghosts[0].pos.left_x, ghosts[0].pos.top_y);
    draw_xpm(ghost_blue_pixmap, ghost_blue_info, ghosts[1].pos.left_x, ghosts[1].pos.top_y);
    draw_xpm(ghost_pink_pixmap, ghost_pink_info, ghosts[2].pos.left_x, ghosts[2].pos.top_y);
    draw_xpm(ghost_orange_pixmap, ghost_orange_info, ghosts[3].pos.left_x, ghosts[3].pos.top_y);
}

void (erase_ghosts)() {
    draw_rectangle(ghosts[0].pos.left_x, ghosts[0].pos.top_y, ghost_red_info.width, ghost_red_info.height, BACKGROUND);
    draw_rectangle(ghosts[1].pos.left_x, ghosts[1].pos.top_y, ghost_blue_info.width, ghost_blue_info.height, BACKGROUND);
    draw_rectangle(ghosts[2].pos.left_x, ghosts[2].pos.top_y, ghost_pink_info.width, ghost_pink_info.height, BACKGROUND);
    draw_rectangle(ghosts[3].pos.left_x, ghosts[3].pos.top_y, ghost_orange_info.width, ghost_orange_info.height, BACKGROUND);
}
