#include "bullet.h"

/**
 * @brief Represents a single bullet instance.
 *
 * The Bullet struct is opaque.
 */
struct Bullet {
    Sprite *sprite; /**< Underlying sprite */
    bool active;    /**< Active flag */
};

/* Create and destroy section */

Bullet *create_bullet(int x, int y, double angle) {
    Bullet *bullet = malloc(sizeof(Bullet));
    if (bullet == NULL) {
        printf("create_bullet: NULL pointer provided\n");
        return NULL;
    }

    bullet->sprite = create_sprite((xpm_map_t) bullet_xpm, x, y, (int16_t) round(cos(angle) * BULLET_SPEED), (int16_t) round(sin(angle) * BULLET_SPEED));
    
    if (bullet->sprite == NULL) {
        printf("create_bullet: create_sprite failed\n");
        free(bullet);
        return NULL;
    }
    bullet->active = true;

    return bullet;
}

int destroy_bullet(Bullet *bullet) {
    if (bullet == NULL) {
        printf("destroy_bullet: NULL pointer provided\n");
        return 1;
    }
    
    if (destroy_sprite(bullet->sprite)) {
        printf("destroy_bullet: destroy_sprite failed\n");
        return 1;
    }
    free(bullet);
    return 0;
}

/* Getter and setter section */

Sprite *bullet_get_sprite(Bullet *bullet) {
    if (bullet == NULL) {
        printf("bullet_get_sprite: NULL pointer provided\n");
        return NULL;
    }

    return bullet->sprite;
}

int16_t bullet_get_x(Bullet *bullet) {
    if (bullet == NULL) {
        printf("bullet_get_x: NULL pointer provided\n");
        return 0;
    }

    return bullet->sprite->x;
}

void bullet_set_x(Bullet *bullet, int16_t x) {
    if (bullet == NULL) {
        printf("bullet_set_x: NULL pointer provided\n");
        return;
    }

    bullet->sprite->x = x;
}

int16_t bullet_get_y(Bullet *bullet) {
    if (bullet == NULL) {
        printf("bullet_get_y: NULL pointer provided\n");
        return 0;
    }

    return bullet->sprite->y;
}

void bullet_set_y(Bullet *bullet, int16_t y) {
    if (bullet == NULL) {
        printf("bullet_set_y: NULL pointer provided\n");
        return;
    }

    bullet->sprite->y = y;
}

double bullet_get_xspeed(Bullet *bullet) {
    if (bullet == NULL) {
        printf("bullet_get_xspeed: NULL pointer provided\n");
        return 0;
    }

    return bullet->sprite->xspeed;
}

double bullet_get_yspeed(Bullet *bullet) {
    if (bullet == NULL) {
        printf("bullet_get_yspeed: NULL pointer provided\n");
        return 0;
    }

    return bullet->sprite->yspeed;
}

bool bullet_get_active(Bullet *bullet) {
    if (bullet == NULL) {
        printf("bullet_get_active: NULL pointer provided\n");
        return false;
    }

    return bullet->active;
}

void bullet_set_active(Bullet *bullet, bool active) {
    if (bullet == NULL) {
        printf("bullet_set_active: NULL pointer provided\n");
        return;
    }

    bullet->active = active;
}

/* Statics section */

/* Others section */

/* Draw section */

int draw_bullet(Bullet *bullet, uint8_t *frame_buffer) {
    if (bullet == NULL) {
        printf("draw_bullet: NULL pointer provided\n");
        return 1;
    }

    if (draw_transparent_sprite(bullet->sprite, frame_buffer)) {
        printf("draw_bullet: draw_transparent_sprite failed\n");
        return 1;
    }

    return 0;
}
