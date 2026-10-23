#include "../include/game/sprite.h"

bool sprite_pixel_visible(const sprite_t *sprite, int x, int y) {
    if (sprite == NULL || sprite->pixmap == NULL || x < 0 || y < 0 ||
        x >= sprite->width || y >= sprite->height) return false;
    const uint8_t *pixel = sprite->pixmap + ((size_t)y * sprite->width + x) * 3;
    uint32_t color = pixel[0] | ((uint32_t)pixel[1] << 8) | ((uint32_t)pixel[2] << 16);
    return color != CHROMA_KEY_GREEN_888;
}

static bool clip_axis(double origin, double delta, double low, double high,
                       double *entry, double *exit) {
    if (fabs(delta) < 1e-12) return origin >= low && origin <= high;
    double a = (low - origin) / delta, b = (high - origin) / delta;
    double first = fmin(a, b), last = fmax(a, b);
    if (first > *entry) *entry = first;
    if (last < *exit) *exit = last;
    return *entry <= *exit;
}

bool sprite_segment_intersects(const sprite_t *sprite, point_t start, point_t end) {
    if (sprite == NULL || sprite->pixmap == NULL) return false;
    double dx = end.x - start.x, dy = end.y - start.y;
    if (dx == 0 && dy == 0) return false;
    double entry = 0, exit = 1;
    if (!clip_axis(start.x, dx, 0, sprite->width, &entry, &exit) ||
        !clip_axis(start.y, dy, 0, sprite->height, &entry, &exit)) return false;

    int first_row = (int)fmax(0, ceil(fmin(start.y + entry * dy, start.y + exit * dy)) - 1);
    int last_row = (int)fmin(sprite->height - 1, floor(fmax(start.y + entry * dy, start.y + exit * dy)));
    for (int y = first_row; y <= last_row; y++) {
        double row_entry = entry, row_exit = exit;
        if (!clip_axis(start.y, dy, y, y + 1, &row_entry, &row_exit)) continue;
        double x0 = start.x + row_entry * dx, x1 = start.x + row_exit * dx;
        /* Include both cells at a shared edge so a visible-pixel graze counts. */
        int first_col = (int)fmax(0, ceil(fmin(x0, x1)) - 1);
        int last_col = (int)fmin(sprite->width - 1, floor(fmax(x0, x1)));
        for (int x = first_col; x <= last_col; x++) {
            if (sprite_pixel_visible(sprite, x, y)) return true;
        }
    }
    return false;
}

int sprite_load(sprite_t *sprite, xpm_map_t xpm) {
    if (sprite == NULL) return 1;

    xpm_image_t img;
    uint8_t *pixmap = xpm_load(xpm, XPM_8_8_8, &img);
    if (pixmap == NULL) {
        printf("[sprite] failed to load xpm\n");
        return 1;
    }

    sprite->pixmap = pixmap;
    sprite->width = img.width;
    sprite->height = img.height;

    return 0;
}

void sprite_destroy(sprite_t *sprite) {
    if (sprite != NULL && sprite->pixmap != NULL) {
        free(sprite->pixmap);
        sprite->pixmap = NULL;
    }
}

int anim_sprite_load(anim_sprite_t *anim, sprite_t *sprites[], size_t count,
                     xpm_map_t xpms[], int frame_delay) {
    if (anim == NULL || sprites == NULL || xpms == NULL || count == 0) return 1;

    for (size_t i = 0; i < count; i++) {
        if (sprite_load(sprites[i], xpms[i]) != 0) {
            printf("[anim] failed to load frame %u\n", (unsigned)i);
            for (size_t j = 0; j < i; j++) {
                sprite_destroy(sprites[j]);
            }
            return 1;
        }
    }

    anim->sprites = sprites;
    anim->num_sprites = count;
    anim->cur_frame = 0;
    anim->last_tick = 0;
    anim->frame_delay = frame_delay;

    return 0;
}
