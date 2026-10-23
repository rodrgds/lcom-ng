#include <lcom/lcf.h>
#include <stdint.h>

#include "../../include/ui/ui.h"
#include "../../../lab5/video_gr.h"
#include "../../include/macros/global.h"

int ui_draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
    return vg_draw_pixel(x, y, color);
}

int ui_draw_sprite(const sprite_t *sprite, uint16_t x, uint16_t y) {
    if (sprite == NULL || sprite->pixmap == NULL) {
        printf("[ui] sprite or pixmap is NULL\n");
        return 1;
    }
    return video_draw_sprite(sprite->pixmap, sprite->width, sprite->height, x, y);
}

int ui_draw_sprite_rotated(const sprite_t *sprite, double x, double y, int angle) {
    if (sprite == NULL || sprite->pixmap == NULL) return 1;
    if (angle == 0 && x >= 0 && y >= 0 && x <= SCREEN_WIDTH && y <= SCREEN_HEIGHT &&
        x == floor(x) && y == floor(y)) return ui_draw_sprite(sprite, (uint16_t)x, (uint16_t)y);

    rotation_t rotation = rotation_for_angle(angle);
    double half_width = sprite->width / 2.0, half_height = sprite->height / 2.0;
    double cx = x + half_width, cy = y + half_height;
    double extent_x = fabs(rotation.cosine) * half_width + fabs(rotation.sine) * half_height;
    double extent_y = fabs(rotation.sine) * half_width + fabs(rotation.cosine) * half_height;
    int left = (int)fmax(0, floor(cx - extent_x));
    int right = (int)fmin(SCREEN_WIDTH, ceil(cx + extent_x));
    int top = (int)fmax(0, floor(cy - extent_y));
    int bottom = (int)fmin(SCREEN_HEIGHT, ceil(cy + extent_y));
    for (int sy = top; sy < bottom; sy++) {
        for (int sx = left; sx < right; sx++) {
            point_t local = rotation_inverse(rotation, (point_t){sx + 0.5 - cx, sy + 0.5 - cy});
            int px = (int)floor(local.x + half_width);
            int py = (int)floor(local.y + half_height);
            if (!sprite_pixel_visible(sprite, px, py)) continue;
            const uint8_t *pixel = sprite->pixmap + ((size_t)py * sprite->width + px) * 3;
            uint32_t color = pixel[0] | ((uint32_t)pixel[1] << 8) | ((uint32_t)pixel[2] << 16);
            if (ui_draw_pixel((uint16_t)sx, (uint16_t)sy, color) != 0) return 1;
        }
    }
    return 0;
}

void anim_update(anim_sprite_t *anim, int current_tick) {
    if (current_tick - anim->last_tick >= anim->frame_delay) {
        anim->cur_frame = (anim->cur_frame + 1) % anim->num_sprites;
        anim->last_tick = current_tick;
    }
}

void anim_draw(const anim_sprite_t *anim, uint16_t x, uint16_t y) {
    ui_draw_sprite(anim->sprites[anim->cur_frame], x, y);
}

int ui_draw_background(const sprite_t *background) {
    if (background == NULL || background->pixmap == NULL) {
        printf("[ui] background sprite or pixmap is NULL\n");
        return 1;
    }
    return video_draw_background(background->pixmap);
}

int ui_draw_rectangle(int x, int y, int width, int height, uint32_t color) {
    /* clamp negative origin: shrink dimensions accordingly */
    if (x < 0) { width  += x; x = 0; }
    if (y < 0) { height += y; y = 0; }
    if (width <= 0 || height <= 0) return 0;

    return video_draw_rect((uint16_t)x, (uint16_t)y,
                           (uint16_t)width, (uint16_t)height, color);
}

int ui_draw_button(const button_t *btn) {
    if (btn == NULL) {
        return 1;
    }

    return ui_draw_rectangle(btn->x,
                             btn->y,
                             btn->width,
                             btn->height,
                             btn->hovered ? btn->hover_color : btn->color);
}

bool ui_button_contains(const button_t *btn, int x, int y) {
    if (btn == NULL) {
        return false;
    }

    return x >= btn->x && x < btn->x + btn->width &&
           y >= btn->y && y < btn->y + btn->height;
}

int ui_draw_text(const Font *font, const char *str, uint16_t x, uint16_t y) {
    if (font == NULL || str == NULL || font->loaded_chars < font->num_chars)
        return 1;

    uint32_t cur_x = x;


    for (const char *p = str; *p != '\0'; p++) {
        if (*p == ' ') {
            cur_x += font->char_width;
            continue;
        }

        int idx = font_glyph_index(*p);
        if (idx < 0 || (uint32_t)idx >= font->num_chars)
            return 1;

        if (cur_x > UINT16_MAX)
            return 1;

        if (ui_draw_sprite(&font->glyphs[idx], (uint16_t)cur_x, y) != 0)
            return 1;
        cur_x += font->char_width;
    }
    return 0;
}

void hud_draw(const Font *font, uint32_t score, uint8_t lives, const sprite_t * heart, uint32_t session_seconds) {
    char score_str[20] = "Score ";
    char score_val[20];
    snprintf(score_val, sizeof(score_val), "%u", score);
    
    ui_draw_text(font, score_str, 10, 10);
    ui_draw_text(font, score_val, 10 + strlen(score_str) * font->char_width, 10);

    char lives_str[20] = "Lives ";
    
    ui_draw_text(font, lives_str, SCREEN_WIDTH - 10 - (strlen(lives_str) * font->char_width) - (3 * 16) - 16, 10);
    for (int i = 0; i < lives; i++) {
        ui_draw_sprite(heart, SCREEN_WIDTH - 10 - ((4-i) * 16) + (i * 8), 10);
    }

    uint32_t minutes = session_seconds / 60;
    uint32_t seconds = session_seconds % 60;
    char timer_text[8];
    snprintf(timer_text, sizeof(timer_text), "%02u:%02u", (unsigned) minutes, (unsigned) seconds);

    ui_draw_text(font, timer_text, CENTER_X(font_text_width(font, timer_text)), 10);
}
