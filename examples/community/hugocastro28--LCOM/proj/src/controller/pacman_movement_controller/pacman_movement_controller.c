#include <lcom/lcf.h>

#include "pacman_movement_controller.h"
#include "../../model/pacman_model/pacman.h"
#include "../game_controller.h"
#include "../collisions_controller/collisions-controller.h"
#include "../timer_controller/timer_controller.h"
#include "../../view/menu_view/menu_view.h"

extern Pacman pacman;
extern bool game_is_on;
extern unsigned int score;

extern uint8_t* pacman_left_pixmap;
extern uint8_t* pacman_right_pixmap;
extern uint8_t* pacman_up_pixmap;
extern uint8_t* pacman_down_pixmap;
extern uint8_t* pacman_pixmap;

extern xpm_image_t pacman_left_info;
extern xpm_image_t pacman_right_info;
extern xpm_image_t pacman_up_info;
extern xpm_image_t pacman_down_info;
extern xpm_image_t pacman_info;

void (pacmanMoveLeft)() {
    Position aux = pacman.pos;
    pacman.pos.left_x -= PACMAN_STEP;
    pacman.pos.right_x -= PACMAN_STEP;
    pacman_pixmap = pacman_left_pixmap;
    pacman_info = pacman_left_info;
    pacmanCollisionHandler(aux);
}

void (pacmanMoveRight)() {
    Position aux = pacman.pos;
    pacman.pos.left_x += PACMAN_STEP;
    pacman.pos.right_x += PACMAN_STEP;
    pacman_pixmap = pacman_right_pixmap;
    pacman_info = pacman_right_info;
    pacmanCollisionHandler(aux);
}

void (pacmanMoveUp)() {
    Position aux = pacman.pos;
    pacman.pos.top_y -= PACMAN_STEP;
    pacman.pos.bottom_y -= PACMAN_STEP;
    pacman_pixmap = pacman_up_pixmap;
    pacman_info = pacman_up_info;
    pacmanCollisionHandler(aux);
}

void (pacmanMoveDown)() {
    Position aux = pacman.pos;
    pacman.pos.top_y += PACMAN_STEP;
    pacman.pos.bottom_y += PACMAN_STEP;
    pacman_pixmap = pacman_down_pixmap;
    pacman_info = pacman_down_info;
    pacmanCollisionHandler(aux);
}

void (pacmanCollisionHandler)(Position aux) {
    if (maze_collision(pacman.pos))
        pacman.pos = aux;
    else if (pacman_ghost_collision()) {
        pacman.pos = aux;
        game_is_on = false;
    }
    else if (pacman_small_coin_collision())
        score += SCORE;
    else if (pacman_big_coin_collision())
        increase_time();
}
