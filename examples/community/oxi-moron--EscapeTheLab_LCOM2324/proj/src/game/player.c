#include "player.h"

void player_construct() {
    player.position = (struct point2D){375, 450};
    player.angle = 90;
}

struct point2D player_get_position() {
    return player.position;
}

double player_get_angle() {
    return player.angle;
}

int player_set_position(struct point2D position) {
    if (position.x < 800 && position.y < 600)
        player.position = position;

    return 0;
}

int player_set_angle(double angle) {
    player.angle = angle;

    return 0;
}

int player_move(enum player_moves move) {
    int new_x = player.position.x, new_y = player.position.y;
    switch(move) {
        case UP:
            new_y -= (int) (PLAYER_SPEED * sin(to_radians(player.angle)));
            new_x += (int) (PLAYER_SPEED * cos(to_radians(player.angle)));
            break;
        case DOWN:
            new_y += (int) (PLAYER_SPEED * sin(to_radians(player.angle)));
            new_x -= (int) (PLAYER_SPEED * cos(to_radians(player.angle)));
            break;
        case LEFT:
            new_y -= (int) (PLAYER_SPEED * cos(to_radians(player.angle)));
            new_x -= (int) (PLAYER_SPEED * sin(to_radians(player.angle)));
            break;
        case RIGHT:
            new_y += (int) (PLAYER_SPEED * cos(to_radians(player.angle)));
            new_x += (int) (PLAYER_SPEED * sin(to_radians(player.angle)));
            break;
        case ROTATE_LEFT:
            player.angle = (int)(player.angle + PLAYER_SPEED) % 360;
            break;
        case ROTATE_RIGHT:
            player.angle = player.angle - PLAYER_SPEED;
            if (player.angle < 0) player.angle += 360;
            break;
    }
    uint8_t cell;
    map_get_grid_pos(new_x * 32 / 800 , new_y * 24 / 600, &cell);
    if (cell != 1 && cell != 8 && cell != 3 && cell != 4) {
      player.position.x = new_x;
      player.position.y = new_y;
    }
    return 0;
}

bool player_crossed_door() {
    uint8_t pos = 0;
    map_get_grid_pos(player.position.x * 32 / 800, player.position.y * 24 / 600 , &pos);
    return pos == 16;
}

void player_reset_position(int n) {
    uint32_t x; uint32_t y; uint32_t angle;
    map_get_start_pos(&x, &y, &angle, n);
    player.position.x = x;
    player.position.y = y;
    player.angle = angle;
}

double (to_radians) (double angle) {
    return 2 * M_PI * angle * 1.0 / 360;
}
