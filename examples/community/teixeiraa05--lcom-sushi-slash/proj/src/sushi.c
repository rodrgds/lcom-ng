
#include <stdbool.h>

#include "../include/game/sushi.h"
#include "../include/game/game_rand.h"
#include "../include/game/cut_profile.h"

#include "../assets/sushi_pieces/Nigiri1.xpm"
#include "../assets/sushi_pieces/Nigiri2.xpm"
#include "../assets/sushi_pieces/Nigiri3.xpm"
#include "../assets/sushi_pieces/Nigiri4.xpm"
#include "../assets/sushi_pieces/Onigiri.xpm"
#include "../assets/sushi_pieces/SushiRoll1.xpm"
#include "../assets/sushi_pieces/SushiRoll2.xpm"
#include "../assets/sushi_pieces/SushiRoll3.xpm"
#include "../assets/sushi_pieces/SushiRoll5.xpm"
#include "../assets/sushi_pieces/SushiRoll6.xpm"

#include "../assets/sushi_pieces/bomb.xpm"

static xpm_map_t sushi_xpms[] = {
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

sushi_t create_sushi(int x, int y, int vx, int vy, int gravity, sprite_t* xpm, int score_value) {
    sushi_t sushi;
    sushi.x = PHYSICS_FROM_PIXELS(x);
    sushi.y = PHYSICS_FROM_PIXELS(y);
    sushi.vx = PHYSICS_FROM_PIXELS(vx);
    sushi.vy = PHYSICS_FROM_PIXELS(vy);
    sushi.gravity = gravity;
    sushi.angle = 0;
    sushi.angular_velocity = 0;
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

    uint8_t sushi_type = game_rand(prng_seed) % (sizeof(sushi_xpms) / sizeof(sushi_xpms[0]));

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
    sushi.angle = game_rand(prng_seed) % 360;
    sushi.angular_velocity = 1 + game_rand(prng_seed) % 3;
    if (game_rand(prng_seed) & 1) sushi.angular_velocity = -sushi.angular_velocity;

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
    sushi->angle = ((sushi->angle + sushi->angular_velocity) % 360 + 360) % 360;

    if (sushi->xpm != NULL) {
        int margin = sushi->kind == SUSHI_REGULAR ? sushi->xpm->height : 0;
        if (sushi_screen_x(sushi) + sushi->xpm->width + margin < 0 ||
            sushi_screen_x(sushi) - margin > SCREEN_WIDTH) {
            sushi_destroy(sushi);
        }
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

    if (sushi->kind == SUSHI_REGULAR) {
        rotation_t rotation = rotation_for_angle(sushi->angle);
        double half_width = sushi->xpm->width / 2.0, half_height = sushi->xpm->height / 2.0;
        double cx = sushi->x / (double)PHYSICS_SCALE + half_width;
        double cy = sushi->y / (double)PHYSICS_SCALE + half_height;
        point_t start = rotation_inverse(rotation, (point_t){cut_x0 - cx, cut_y0 - cy});
        point_t end = rotation_inverse(rotation, (point_t){cut_x1 - cx, cut_y1 - cy});
        start.x += half_width; start.y += half_height;
        end.x += half_width; end.y += half_height;
        return sprite_segment_intersects(sushi->xpm, start, end);
    }
    return segment_intersects_sushi_bounds(sushi, cut_x0, cut_y0, cut_x1, cut_y1, radius);
}

int cut_sushi(sushi_t* sushi, int cut_x0, int cut_y0, int cut_x1, int cut_y1,
               sushi_fragment_t* left_fragment, sushi_fragment_t* right_fragment) {
    if (sushi == NULL || left_fragment == NULL || right_fragment == NULL ||
        left_fragment == right_fragment || sushi->kind != SUSHI_REGULAR || sushi->xpm == NULL) return 1;
    if (!sushi_segment_intersects(sushi, cut_x0, cut_y0, cut_x1, cut_y1, 0)) return 1;

    rotation_t rotation = rotation_for_angle(sushi->angle);
    double half_width = sushi->xpm->width / 2.0, half_height = sushi->xpm->height / 2.0;
    double cx = sushi->x / (double)PHYSICS_SCALE + half_width;
    double cy = sushi->y / (double)PHYSICS_SCALE + half_height;
    point_t start = rotation_inverse(rotation, (point_t){cut_x0 - cx, cut_y0 - cy});
    point_t end = rotation_inverse(rotation, (point_t){cut_x1 - cx, cut_y1 - cy});
    const cut_profile_t *profile = cut_profile_select(sushi->type, sushi->xpm, start, end);
    if (profile == NULL) return 1;
    cut_piece_t pieces[2];
    if (cut_profile_split(sushi->xpm, profile, pieces) != 0) return 1;
    point_t normal = rotation_apply(rotation, (point_t){profile->nx, profile->ny});
    sushi_fragment_t *fragments[2] = {left_fragment, right_fragment};
    for (int side = 0; side < 2; side++) {
        sprite_t *sprite = pieces[side].sprite;
        point_t center = rotation_apply(rotation,
            (point_t){pieces[side].x + sprite->width / 2.0 - half_width,
                      pieces[side].y + sprite->height / 2.0 - half_height});
        int direction = side == 0 ? -1 : 1;
        *fragments[side] = (sushi_fragment_t){
            .x = sushi->x + (int)lround((half_width + center.x - sprite->width / 2.0) * PHYSICS_SCALE),
            .y = sushi->y + (int)lround((half_height + center.y - sprite->height / 2.0) * PHYSICS_SCALE),
            .vx = sushi->vx + (int)lround(direction * normal.x * PHYSICS_FROM_PIXELS(2)),
            .vy = sushi->vy + (int)lround(direction * normal.y * PHYSICS_FROM_PIXELS(2)),
            .gravity = sushi->gravity,
            .angle = sushi->angle,
            .angular_velocity = sushi->angular_velocity,
            .xpm = sprite
        };
    }
    sushi_destroy(sushi);

    return 0;
}

void update_sushi_fragment(sushi_fragment_t* fragment) {
    if (fragment == NULL) return;

    fragment->x += fragment->vx;
    fragment->y += fragment->vy;
    fragment->vy += fragment->gravity;
    fragment->angle = ((fragment->angle + fragment->angular_velocity) % 360 + 360) % 360;

    // Deactivate if it goes off-screen
    if (fragment->xpm != NULL &&
        (sushi_fragment_screen_x(fragment) + fragment->xpm->width + fragment->xpm->height < 0 ||
         sushi_fragment_screen_x(fragment) - fragment->xpm->height > SCREEN_WIDTH ||
         sushi_fragment_screen_y(fragment) - fragment->xpm->width > SCREEN_HEIGHT)) {
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
