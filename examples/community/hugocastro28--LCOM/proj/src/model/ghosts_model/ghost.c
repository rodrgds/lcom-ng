#include <lcom/lcf.h>

#include "ghost.h"

Ghost ghosts[4];
// red - blue - pink - orange

void (set_blue_position)() {
    ghosts[1].pos.left_x = 440;
    ghosts[1].pos.right_x = 465;
    ghosts[1].pos.top_y = 360;
    ghosts[1].pos.bottom_y = 335;
}

void (set_orange_position)() {
    ghosts[3].pos.left_x = 560;
    ghosts[3].pos.right_x = 585;
    ghosts[3].pos.top_y = 360;
    ghosts[3].pos.bottom_y = 335;
}

void (set_pink_position)() {
    ghosts[2].pos.left_x = 500;
    ghosts[2].pos.right_x = 525;
    ghosts[2].pos.top_y = 360;
    ghosts[2].pos.bottom_y = 335;
}

void (set_red_position)() {
    ghosts[0].pos.left_x = 500;
    ghosts[0].pos.right_x = 525;
    ghosts[0].pos.top_y = 310;
    ghosts[0].pos.bottom_y = 330;
}

void (set_ghosts_positions)() {
    set_red_position();
    set_orange_position();
    set_pink_position();
    set_blue_position();
}
