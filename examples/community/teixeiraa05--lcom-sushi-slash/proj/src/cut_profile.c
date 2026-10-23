#include "../include/game/cut_profile.h"
#include "../assets/sushi_pieces/cut_profiles.h"

#include <float.h>
#include <stdlib.h>
#include <string.h>

const cut_profile_t *cut_profile_select(uint8_t type, const sprite_t *sprite,
                                       point_t start, point_t end) {
    if (sprite == NULL || sprite->pixmap == NULL ||
        type >= sizeof(sushi_cut_profiles) / sizeof(sushi_cut_profiles[0])) return NULL;
    const cut_profile_set_t *set = &sushi_cut_profiles[type];
    if (sprite->width != set->width || sprite->height != set->height) return NULL;

    double dx = end.x - start.x, dy = end.y - start.y;
    double length_squared = dx * dx + dy * dy;
    if (length_squared == 0) return NULL;

    /* shortcut: eight angles and three offsets approximate the slash; add profiles for finer cuts. */
    int direction = 0;
    double best_angle = DBL_MAX;
    for (int i = 0; i < CUT_DIRECTION_COUNT; i++) {
        const cut_profile_t *profile = &set->profiles[i * CUT_OFFSET_COUNT];
        double error = fabs(profile->nx * dx + profile->ny * dy);
        if (error < best_angle) {
            direction = i;
            best_angle = error;
        }
    }

    /* Project the slash line onto the chosen normal; reversing it keeps the cut. */
    double t = -(start.x * dx + start.y * dy) / length_squared;
    point_t closest = {start.x + t * dx, start.y + t * dy};
    const cut_profile_t *best = NULL;
    double best_offset = DBL_MAX;
    for (int i = 0; i < CUT_OFFSET_COUNT; i++) {
        const cut_profile_t *profile = &set->profiles[direction * CUT_OFFSET_COUNT + i];
        double error = fabs(profile->nx * closest.x + profile->ny * closest.y - profile->offset);
        if (error < best_offset) {
            best = profile;
            best_offset = error;
        }
    }
    return best;
}

static int pixel_side(const sprite_t *source, const cut_profile_t *profile, int x, int y) {
    double distance = profile->nx * (x + 0.5 - source->width / 2.0) +
                      profile->ny * (y + 0.5 - source->height / 2.0);
    return distance < profile->offset ? 0 : 1;
}

int cut_profile_split(const sprite_t *source, const cut_profile_t *profile,
                      cut_piece_t pieces[2]) {
    if (source == NULL || source->pixmap == NULL || profile == NULL || pieces == NULL) return 1;
    int min_x[2] = {source->width, source->width};
    int min_y[2] = {source->height, source->height};
    int max_x[2] = {-1, -1}, max_y[2] = {-1, -1};
    for (int y = 0; y < source->height; y++) {
        for (int x = 0; x < source->width; x++) {
            if (!sprite_pixel_visible(source, x, y)) continue;
            int side = pixel_side(source, profile, x, y);
            if (x < min_x[side]) min_x[side] = x;
            if (x > max_x[side]) max_x[side] = x;
            if (y < min_y[side]) min_y[side] = y;
            if (y > max_y[side]) max_y[side] = y;
        }
    }
    cut_piece_t result[2] = {{0}, {0}};
    for (int side = 0; side < 2; side++) {
        if (max_x[side] < 0) goto fail;
        result[side].x = min_x[side];
        result[side].y = min_y[side];
        result[side].sprite = calloc(1, sizeof(sprite_t));
        sprite_t *sprite = result[side].sprite;
        if (sprite == NULL) goto fail;
        sprite->width = max_x[side] - min_x[side] + 1;
        sprite->height = max_y[side] - min_y[side] + 1;
        sprite->pixmap = malloc((size_t)sprite->width * sprite->height * 3);
        if (sprite->pixmap == NULL) goto fail;
        for (int y = 0; y < sprite->height; y++) {
            for (int x = 0; x < sprite->width; x++) {
                int sx = x + min_x[side], sy = y + min_y[side];
                uint8_t *dst = sprite->pixmap + ((size_t)y * sprite->width + x) * 3;
                if (pixel_side(source, profile, sx, sy) == side) {
                    memcpy(dst, source->pixmap + ((size_t)sy * source->width + sx) * 3, 3);
                } else {
                    dst[0] = CHROMA_KEY_GREEN_888 & 255;
                    dst[1] = (CHROMA_KEY_GREEN_888 >> 8) & 255;
                    dst[2] = (CHROMA_KEY_GREEN_888 >> 16) & 255;
                }
            }
        }
    }
    pieces[0] = result[0];
    pieces[1] = result[1];
    return 0;

fail:
    for (int side = 0; side < 2; side++) {
        sprite_destroy(result[side].sprite);
        free(result[side].sprite);
    }
    return 1;
}
