/**
 * @file bullet.c
 * @brief Implementation of bullet entity management.
 *
 * This file contains the implementation of the functions declared in bullet.h.
 */
#include "bullet.h"
#include <stdlib.h> 
#include <math.h>   

#define BULLET_SPRITE_WIDTH 5
#define BULLET_SPRITE_HEIGHT 5


bullet_t* bullet_init(int16_t start_x, int16_t start_y, int16_t target_x, int16_t target_y, bool is_from_pl) {    int16_t initial_sprite_x = start_x - (BULLET_SPRITE_WIDTH / 2);
    int16_t initial_sprite_y = start_y - (BULLET_SPRITE_HEIGHT / 2);

    xpm_map_t bullets[1] = {bullet_xpm};
    sprite_t* sp = create_sprite(bullets, 1, initial_sprite_x, initial_sprite_y);
    if (sp == NULL) {
        return NULL;
    }

    bullet_t *bullet = (bullet_t*) malloc(sizeof(bullet_t));
    if (bullet == NULL) {
        destroy_sprite(sp);
        return NULL;
    }

    bullet->sprite = sp;
    bullet->active = true;
    bullet->is_from_player = is_from_pl;

    float dx = (float)(target_x - start_x);
    float dy = (float)(target_y - start_y);

    float magnitude = sqrt(dx*dx + dy*dy);

    int16_t speed=BULLET_SPEED_ENEMY;

    if (is_from_pl){ speed=BULLET_SPEED_PLAYER;}


    if (magnitude == 0) { 
        bullet->vx = 0;
        bullet->vy = -1 * speed; 
    } else {
        bullet->vx = (int16_t)((dx / magnitude) * speed);
        bullet->vy = (int16_t)((dy / magnitude) * speed);
    }
    
    if (bullet->vx == 0 && bullet->vy == 0 && magnitude != 0) {
        if (fabs(dx) > fabs(dy)) {
            bullet->vx = (dx > 0) ? 1 : -1;
        } else {
            bullet->vy = (dy > 0) ? 1 : -1;
        }
    }
    
    return bullet;
}

void destroy_bullet(bullet_t *bullet) {
    if (bullet != NULL) {
        if (bullet->sprite != NULL) {
            destroy_sprite(bullet->sprite);
        }
        free(bullet);
        bullet = NULL;
    }
}

void bullet_move(bullet_t *bullet) {
    if (bullet != NULL && bullet->active && bullet->sprite != NULL) {
        position_t *pos = get_sprite_pos(bullet->sprite);
        pos->x += bullet->vx;
        pos->y += bullet->vy;
    }
}

int bullet_draw(const bullet_t *bullet) {
    if (bullet != NULL && bullet->active && bullet->sprite != NULL) {
        return draw_sprite(bullet->sprite); 
    }
    return 1; 
}

void bullet_check_bounds_deactivate(bullet_t *bullet, unsigned int screen_width, unsigned int screen_height) {
    if (bullet != NULL && bullet->active && bullet->sprite != NULL) {
        position_t *pos = get_sprite_pos(bullet->sprite);
        uint16_t width = get_sprite_width(bullet->sprite);
        uint16_t height = get_sprite_height(bullet->sprite);

        if (pos->x + width < 0 || pos->x > (int16_t)screen_width ||
            pos->y + height < 0 || pos->y > (int16_t)screen_height) {
            bullet->active = false;
        }
    }
}
