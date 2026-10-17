#include <math.h>

#include "entity.h"
#include "draw.h"
#include "game_state.h"

uint64_t delta_time;

bool collision_handler(Ball *ball, CollisionEntity entity, CollisionType type) {
    unsigned int ball_critical;
    bool x_alignment, y_alignment;

    switch (type) {
        case PLAYER: {
            const Player *player = entity.player;
            ball_critical = ball->x_pos + ball->radius * (int)(player->number == 2);
            y_alignment = ball->y_pos >= player->y_pos &&
                          ball->y_pos <= player->y_pos + player->height;
            x_alignment = player->number == 1 ?
                ball_critical <= player->x_pos + player->width :
                ball_critical >= player->x_pos;
            
            if (!x_alignment || !y_alignment) return false;

            float relativeY = ball->y_pos - (player->y_pos + player->height / 2.0f);
            float angleScaling = relativeY / (player->height / 2.0f);
            ball->angle = angleScaling * M_PI / 3;
            ball->angle = ball->angle < 0.0f ? 2 * M_PI + ball->angle : ball->angle;
            
            if (ball->angle < M_PI) {
                ball->vX = -fabs(ball->vX);
                ball->vY = fabs(ball->vY);
            } else  {
                ball->vX = fabs(ball->vX);
                ball->vY = -fabs(ball->vY);
            }
            break;
        }
        case WALL: {
            const Wall *wall = entity.wall;
            ball_critical = ball->y_pos + ball->height * (int)(!wall->top);
            y_alignment = wall->top ?
                ball_critical <= wall->y_pos + wall->height :
                ball_critical >= wall->y_pos;

            if (!y_alignment) return false;
            
            ball->angle = ball->angle < 0.0f ? 2 * M_PI + ball->angle : ball->angle;
            ball->angle = M_PI - ball->angle;
            ball->vY *= -1; ball->vX *= -1;

            break;
        }
    }

    return true;

}

Ball *create_ball(xpm_map_t xpm) {
    
    Ball *ball = (Ball *)malloc(sizeof(Ball));
    xpm_image_t image;
    uint8_t *sprite = xpm_load(xpm, XPM_INDEXED, &image);

    *ball = (Ball) {
        .radius = 32,
        .width = image.width,
        .height = image.height,
        .x_pos = x_max / 2,
        .y_pos = y_max / 2,
        .vX = 5,
        .vY = -5,
        .angle = M_PI / 3,
        .sprite = sprite
    };
    
    return ball;
}

bool update_ball(Ball *ball) {
    ball->x_pos += (delta_time / 16.0f) * ball->vX * cos(ball->angle);
    ball->y_pos -= (delta_time / 16.0f) * ball->vY * sin(ball->angle);
    ball->y_pos = fmin(y_max - ball->height - 10, fmax(10, (int)(ball->y_pos - ball->vY * sin(ball->angle))));
    return true;
}


Wall *create_wall(bool top) {
    Wall *wall = (Wall *)malloc(sizeof(Wall));
    *wall = (Wall) {
        .x_pos = 0,
        .y_pos = top ? 0 : y_max - 10,
        .width = x_max,
        .height = 10,
        .top = top
    };

    return wall;
}

Player *create_player(unsigned char number) {
    Player *player = (Player*)malloc(sizeof(Player));
    *player = (Player) {
        .x_pos = number == 1 ? 10 : x_max - 20,
        .y_pos = (y_max * (int)(number == 2) + y_max) / 3 - 100,
        .width = 10,
        .height = 100,
        .number = number
    };

    return player;
}
