#include <lcom/lcf.h>

#include "pacman.h"

Pacman pacman;

void (set_pacman_position)() {
    pacman.pos.left_x = 500;
    pacman.pos.right_x = 525;
    pacman.pos.top_y = 554;
    pacman.pos.bottom_y = 579;
}
