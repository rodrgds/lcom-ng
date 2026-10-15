#include <lcom/lcf.h>
#include "sprite.h"
#include "video.h"

Sprite *(sprite_load)(xpm_map_t xpm) {
    Sprite *s = malloc(sizeof(Sprite));
    if (s == NULL) return NULL;

    s->colors = xpm_load(xpm, XPM_8_8_8, &s->img);
    s->has_alpha = false;
    if (s->colors == NULL) {
        free(s);
        return NULL;
    }

    return s;
}

Sprite *(sprite_load_alpha)(xpm_map_t xpm) {
    Sprite *s = malloc(sizeof(Sprite));
    if (s == NULL) return NULL;

    s->colors = xpm_load(xpm, XPM_8_8_8_8, &s->img);
    s->has_alpha = true;
    if (s->colors == NULL) {
        free(s);
        return NULL;
    }

    return s;
}

void (sprite_draw)(Sprite *s, uint16_t x, uint16_t y) {
    if (s == NULL) return;

    uint8_t *px = s->colors;
    if (!s->has_alpha) {
        for (int i = 0; i < s->img.height; i++) {
            for (int j = 0; j < s->img.width; j++, px += 3) {
                uint32_t color = ((uint32_t)px[2] << 16) | ((uint32_t)px[1] << 8) | px[0];
                if (color != 0xFF00FF && color != 0xFFFFFF) video_draw_pixel(x + j, y + i, color);
            }
        }
        return;
    }

    px = s->colors;
    for (int i = 0; i < s->img.height; i++) {
        for (int j = 0; j < s->img.width; j++, px += 4) {
            if (px[3] == 0) continue;
            uint32_t color = ((uint32_t)px[2] << 16) | ((uint32_t)px[1] << 8) | px[0];
            video_draw_pixel(x + j, y + i, color);
        }
    }
}

void (sprite_draw_colored)(Sprite *s, uint16_t x, uint16_t y, uint32_t fg, uint32_t bg, bool has_bg) {
    if (s == NULL || !s->has_alpha) return;

    uint8_t *px = s->colors;
    for (int i = 0; i < s->img.height; i++) {
        for (int j = 0; j < s->img.width; j++, px += 4) {
            if (px[3] == 0) {
                video_draw_pixel(x + j, y + i, fg);
                continue;
            }
            if (has_bg) {
                video_draw_pixel(x + j, y + i, bg);
            }
        }
    }
}

void (sprite_destroy)(Sprite *s) {
    if (s == NULL) return;
    free(s->colors);
    free(s);
}
