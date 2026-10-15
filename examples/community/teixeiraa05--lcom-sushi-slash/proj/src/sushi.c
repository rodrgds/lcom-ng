
#include <stdbool.h>

#include "../include/game/sushi.h"
#include "../include/game/game_rand.h"

#include "../assets/sushi_pieces/Nigiri1.xpm"
#include "../assets/sushi_pieces/Nigiri2.xpm"
#include "../assets/sushi_pieces/Nigiri3.xpm"
#include "../assets/sushi_pieces/Nigiri4.xpm"
#include "../assets/sushi_pieces/Onigiri.xpm"
#include "../assets/sushi_pieces/SushiRoll1.xpm"
#include "../assets/sushi_pieces/SushiRoll2.xpm"
#include "../assets/sushi_pieces/SushiRoll3.xpm"
#include "../assets/sushi_pieces/SushiRoll4.xpm"
#include "../assets/sushi_pieces/SushiRoll5.xpm"
#include "../assets/sushi_pieces/SushiRoll6.xpm"

#include "../assets/sushi_pieces/Nigiri1_left.xpm"
#include "../assets/sushi_pieces/Nigiri2_left.xpm"
#include "../assets/sushi_pieces/Nigiri3_left.xpm"
#include "../assets/sushi_pieces/Nigiri4_left.xpm"
#include "../assets/sushi_pieces/Onigiri_left.xpm"
#include "../assets/sushi_pieces/SushiRoll1_left.xpm"
#include "../assets/sushi_pieces/SushiRoll2_left.xpm"
#include "../assets/sushi_pieces/SushiRoll3_left.xpm"
#include "../assets/sushi_pieces/SushiRoll4_left.xpm"
#include "../assets/sushi_pieces/SushiRoll5_left.xpm"
#include "../assets/sushi_pieces/SushiRoll6_left.xpm"

#include "../assets/sushi_pieces/Nigiri1_right.xpm"
#include "../assets/sushi_pieces/Nigiri2_right.xpm"
#include "../assets/sushi_pieces/Nigiri3_right.xpm"
#include "../assets/sushi_pieces/Nigiri4_right.xpm"
#include "../assets/sushi_pieces/Onigiri_right.xpm"
#include "../assets/sushi_pieces/SushiRoll1_right.xpm"
#include "../assets/sushi_pieces/SushiRoll2_right.xpm"
#include "../assets/sushi_pieces/SushiRoll3_right.xpm"
#include "../assets/sushi_pieces/SushiRoll4_right.xpm"
#include "../assets/sushi_pieces/SushiRoll5_right.xpm"
#include "../assets/sushi_pieces/SushiRoll6_right.xpm"

#include "../assets/sushi_pieces/bomb.xpm"

xpm_map_t sushi_xpms[10] = {
    // Placeholder XPMs for different sushi types
    (xpm_map_t)Nigiri1_xpm,
    (xpm_map_t)Nigiri2_xpm,
    (xpm_map_t)Nigiri3_xpm,
    (xpm_map_t)Nigiri4_xpm,
    (xpm_map_t)Onigiri_xpm,
    (xpm_map_t)SushiRoll1_xpm,
    (xpm_map_t)SushiRoll2_xpm,
    (xpm_map_t)SushiRoll3_xpm,
    (xpm_map_t)SushiRoll5_xpm,
    (xpm_map_t)SushiRoll6_xpm
};

xpm_map_t sushi_left_xpms[10] = {
    // Placeholder XPMs for the left fragments of different sushi types
    (xpm_map_t)Nigiri1_left_xpm,
    (xpm_map_t)Nigiri2_left_xpm,
    (xpm_map_t)Nigiri3_left_xpm,
    (xpm_map_t)Nigiri4_left_xpm,
    (xpm_map_t)Onigiri_left_xpm,
    (xpm_map_t)SushiRoll1_left_xpm,
    (xpm_map_t)SushiRoll2_left_xpm,
    (xpm_map_t)SushiRoll3_left_xpm,
    (xpm_map_t)SushiRoll5_left_xpm,
    (xpm_map_t)SushiRoll6_left_xpm
};

xpm_map_t sushi_right_xpms[10] = {
    // Placeholder XPMs for the right fragments of different sushi types
    (xpm_map_t)Nigiri1_right_xpm,
    (xpm_map_t)Nigiri2_right_xpm,
    (xpm_map_t)Nigiri3_right_xpm,
    (xpm_map_t)Nigiri4_right_xpm,
    (xpm_map_t)Onigiri_right_xpm,
    (xpm_map_t)SushiRoll1_right_xpm,
    (xpm_map_t)SushiRoll2_right_xpm,
    (xpm_map_t)SushiRoll3_right_xpm,
    (xpm_map_t)SushiRoll5_right_xpm,
    (xpm_map_t)SushiRoll6_right_xpm
};

sushi_t create_sushi(int x, int y, int vx, int vy, int gravity, sprite_t* xpm, int score_value) {
    sushi_t sushi;
    sushi.x = PHYSICS_FROM_PIXELS(x);
    sushi.y = PHYSICS_FROM_PIXELS(y);
    sushi.vx = PHYSICS_FROM_PIXELS(vx);
    sushi.vy = PHYSICS_FROM_PIXELS(vy);
    sushi.gravity = gravity;
    sushi.xpm = xpm;
    sushi.score_value = score_value;
    sushi.type = 0;
    sushi.kind = SUSHI_REGULAR;
    return sushi;
}



sushi_t create_bomb(int x, int y, int vx, int vy, int gravity, sprite_t* xpm) {
    sushi_t bomb = create_sushi(x, y, vx, vy, gravity, xpm, 0);
    bomb.kind = SUSHI_BOMB;
    return bomb;
}



sushi_t create_sushi_boss(int x, int y, int vx, int vy, int gravity, sprite_t* xpm, int score_value) {
    sushi_t boss = create_sushi(x, y, vx, vy, gravity, xpm, score_value);
    boss.kind = SUSHI_BOSS;
    return boss;
}

static int sushi_horizontal_velocity(int x, const sprite_t *sprite, int speed) {
    int sushi_center = x + sprite->width / 2;
    int screen_center = SCREEN_WIDTH / 2;
    int direction = sushi_center < screen_center ? 1 : -1;
    int velocity = speed / 10;

    if (velocity < 1) velocity = 1;
    return direction * velocity;
}

sushi_t create_random_sushi(int speed, int gravity, uint32_t *prng_seed) {
    sprite_t *sprite = malloc(sizeof(sprite_t));
    if (sprite == NULL) return create_sushi(0, 0, 0, 0, gravity, NULL, 0);

    uint8_t sushi_type = game_rand(prng_seed) % 10;

    if (sprite_load(sprite, sushi_xpms[sushi_type])) {
        free(sprite);
        return create_sushi(0, 0, 0, 0, gravity, NULL, 0);
    }

    int max_x = SCREEN_WIDTH - sprite->width;
    if (max_x < 1) max_x = 1;
    int x = game_rand(prng_seed) % max_x;

    sushi_t sushi = create_sushi(x,
                                 SCREEN_HEIGHT + SUSHI_SPAWN_Y_OFFSET,
                                 sushi_horizontal_velocity(x, sprite, speed),
                                 -speed,
                                 gravity,
                                 sprite,
                                 SUSHI_SCORE_VALUE);
    sushi.type = sushi_type;

    return sushi;
}

sushi_t create_random_bomb(int speed, int gravity, uint32_t *prng_seed) {
    sprite_t *sprite = malloc(sizeof(sprite_t));
    if (sprite == NULL) return create_bomb(0, 0, 0, 0, gravity, NULL);

    if (sprite_load(sprite, (xpm_map_t)bomb_xpm)) {
        free(sprite);
        return create_bomb(0, 0, 0, 0, gravity, NULL);
    }

    int max_x = SCREEN_WIDTH - sprite->width;
    if (max_x < 1) max_x = 1;
    int x = game_rand(prng_seed) % max_x;

    return create_bomb(x,
                       SCREEN_HEIGHT + SUSHI_SPAWN_Y_OFFSET,
                       sushi_horizontal_velocity(x, sprite, speed),
                       -speed,
                       gravity,
                       sprite);
}

void update_sushi(sushi_t* sushi) {
    if (sushi == NULL) return;

    sushi->x += sushi->vx;
    sushi->y += sushi->vy;
    sushi->vy += sushi->gravity;

    // Deactivate if it goes off-screen
    if (sushi->xpm != NULL &&
        (sushi_screen_x(sushi) + sushi->xpm->width < 0 || sushi_screen_x(sushi) > SCREEN_WIDTH)) {
          sushi_destroy(sushi);
    }
}

int sushi_screen_x(const sushi_t* sushi) {
    return sushi == NULL ? 0 : PHYSICS_TO_PIXELS(sushi->x);
}

int sushi_screen_y(const sushi_t* sushi) {
    return sushi == NULL ? 0 : PHYSICS_TO_PIXELS(sushi->y);
}

void sushi_destroy(sushi_t* sushi) {
    if (sushi != NULL) {
        if (sushi->xpm != NULL) {
            sprite_destroy(sushi->xpm);
            free(sushi->xpm);
            sushi->xpm = NULL;
        }
    }
}

static bool point_inside_sushi_bounds(const sushi_t* sushi, int x, int y, int padding) {
    int sushi_x = sushi_screen_x(sushi);
    int sushi_y = sushi_screen_y(sushi);

    return x >= sushi_x - padding &&
           x <= sushi_x + sushi->xpm->width + padding &&
           y >= sushi_y - padding &&
           y <= sushi_y + sushi->xpm->height + padding;
}

static int orientation(int ax, int ay, int bx, int by, int cx, int cy) {
    long value = (long)(by - ay) * (cx - bx) - (long)(bx - ax) * (cy - by);

    if (value == 0) return 0;
    return value > 0 ? 1 : 2;
}

static bool point_on_segment(int ax, int ay, int bx, int by, int px, int py) {
    return px >= (ax < bx ? ax : bx) && px <= (ax > bx ? ax : bx) &&
           py >= (ay < by ? ay : by) && py <= (ay > by ? ay : by);
}

static bool segments_intersect(int ax, int ay, int bx, int by, int cx, int cy, int dx, int dy) {
    int o1 = orientation(ax, ay, bx, by, cx, cy);
    int o2 = orientation(ax, ay, bx, by, dx, dy);
    int o3 = orientation(cx, cy, dx, dy, ax, ay);
    int o4 = orientation(cx, cy, dx, dy, bx, by);

    if (o1 != o2 && o3 != o4) return true;

    if (o1 == 0 && point_on_segment(ax, ay, bx, by, cx, cy)) return true;
    if (o2 == 0 && point_on_segment(ax, ay, bx, by, dx, dy)) return true;
    if (o3 == 0 && point_on_segment(cx, cy, dx, dy, ax, ay)) return true;
    if (o4 == 0 && point_on_segment(cx, cy, dx, dy, bx, by)) return true;

    return false;
}

static bool segment_intersects_sushi_bounds(const sushi_t* sushi, int x0, int y0, int x1, int y1, int padding) {
    int sushi_x = sushi_screen_x(sushi);
    int sushi_y = sushi_screen_y(sushi);
    int left = sushi_x - padding;
    int right = sushi_x + sushi->xpm->width + padding;
    int top = sushi_y - padding;
    int bottom = sushi_y + sushi->xpm->height + padding;

    if (point_inside_sushi_bounds(sushi, x0, y0, padding) ||
        point_inside_sushi_bounds(sushi, x1, y1, padding)) {
        return true;
    }

    return segments_intersect(x0, y0, x1, y1, left, top, right, top) ||
           segments_intersect(x0, y0, x1, y1, right, top, right, bottom) ||
           segments_intersect(x0, y0, x1, y1, right, bottom, left, bottom) ||
           segments_intersect(x0, y0, x1, y1, left, bottom, left, top);
}

bool sushi_segment_intersects(const sushi_t* sushi, int cut_x0, int cut_y0, int cut_x1, int cut_y1, int radius) {
    if (sushi == NULL || sushi->xpm == NULL) return false;

    return segment_intersects_sushi_bounds(sushi, cut_x0, cut_y0, cut_x1, cut_y1, radius);
}

int cut_sushi(sushi_t* sushi, sushi_fragment_t* left_fragment, sushi_fragment_t* right_fragment) {
    if (sushi == NULL || left_fragment == NULL || right_fragment == NULL ||
        sushi->kind != SUSHI_REGULAR || sushi->type >= 11) return 1;

    sprite_t *left_sprite = malloc(sizeof(sprite_t));
    sprite_t *right_sprite = malloc(sizeof(sprite_t));

    if (left_sprite == NULL || right_sprite == NULL) {
        free(left_sprite);
        free(right_sprite);
        return 1;
    }

    if (sprite_load(left_sprite, sushi_left_xpms[sushi->type])) {
        free(left_sprite);
        free(right_sprite);
        return 1;
    }

    if (sprite_load(right_sprite, sushi_right_xpms[sushi->type])) {
        sprite_destroy(left_sprite);
        free(left_sprite);
        free(right_sprite);
        return 1;
    }

    // Create left fragment
    *left_fragment = (sushi_fragment_t){
        .x = sushi->x - PHYSICS_FROM_PIXELS(2), // Move left
        .y = sushi->y ,
        .vx = sushi->vx - PHYSICS_FROM_PIXELS(2), // Move left
        .vy = sushi->vy - PHYSICS_FROM_PIXELS(2),
        .gravity = sushi->gravity,
        .xpm = left_sprite
    };

    // Create right fragment
    *right_fragment = (sushi_fragment_t){
        .x = sushi->x + PHYSICS_FROM_PIXELS(2), // Move right
        .y = sushi->y,
        .vx = sushi->vx + PHYSICS_FROM_PIXELS(2), // Move right
        .vy = sushi->vy - PHYSICS_FROM_PIXELS(2),
        .gravity = sushi->gravity,
        .xpm = right_sprite
    };

    // Destroy the original sushi
    sushi_destroy(sushi);

    return 0;
}

void update_sushi_fragment(sushi_fragment_t* fragment) {
    if (fragment == NULL) return;

    fragment->x += fragment->vx;
    fragment->y += fragment->vy;
    fragment->vy += fragment->gravity;

    // Deactivate if it goes off-screen
    if (fragment->xpm != NULL &&
        (sushi_fragment_screen_x(fragment) + fragment->xpm->width < 0 ||
         sushi_fragment_screen_x(fragment) > SCREEN_WIDTH ||
         sushi_fragment_screen_y(fragment) > SCREEN_HEIGHT)) {
          sushi_fragment_destroy(fragment);
    }
}

int sushi_fragment_screen_x(const sushi_fragment_t* fragment) {
    return fragment == NULL ? 0 : PHYSICS_TO_PIXELS(fragment->x);
}

int sushi_fragment_screen_y(const sushi_fragment_t* fragment) {
    return fragment == NULL ? 0 : PHYSICS_TO_PIXELS(fragment->y);
}

void sushi_fragment_destroy(sushi_fragment_t* fragment) {
    if (fragment != NULL) {
        if (fragment->xpm != NULL) {
            sprite_destroy(fragment->xpm);
            free(fragment->xpm);
            fragment->xpm = NULL;
        }
    }
}
