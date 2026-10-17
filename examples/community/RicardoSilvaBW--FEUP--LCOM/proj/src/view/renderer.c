#include "renderer.h"

#include "../controller/video/graphics.h"
#include "../config.h"
#include "font.h"

#include <stdio.h>
#include <stdint.h>

int (draw_note)(note *n) {
    if (n == NULL || n->sp == NULL)
        return 1;
    
    if (!n->active) return 0;
    
    if (n->sp->colors != NULL) return draw_xpm_scaled(n->sp->img, n->sp->colors, n->sp->x, n->sp->y, NOTE_SCALE);
    
    uint32_t lane_colors[] = {
        0xB01050,
        0x1050B0,
        0x10B050,
        0xB05010
    };
    
    return vg_draw_rectangle((uint16_t)n->sp->x, (uint16_t)n->sp->y, (uint16_t)n->sp->width, (uint16_t)n->sp->height, lane_colors[n->lane % 4]);
}


int (draw_sprite)(sprite *sp) {
    if (sp == NULL)
        return 1;
    
    if (sp->img.width > 0 && sp->width > (int) sp->img.width)
        return draw_xpm_scaled(sp->img, sp->colors, sp->x, sp->y, sp->width / (int) sp->img.width);
    return draw_xpm(sp->img, sp->colors, sp->x, sp->y);
}

void (draw_acceptance_zone)(const int *lane_x, sprite *targets[4]) {
    for (int i = 0; i < 4; i++) {
        if (targets == NULL || targets[i] == NULL || targets[i]->colors == NULL) continue;
        int x = lane_x[i] - NOTE_SPRITE_SIZE * NOTE_SCALE / 2;
        int y = ACCEPTANCE_Y_TOP;
        draw_xpm_scaled(targets[i]->img, targets[i]->colors, x, y, NOTE_SCALE);
    }
}

void (draw_time)(sprite *clock_sp, int remaining_secs, int x, int y) {
    int cursor = x;

    if (clock_sp && clock_sp->colors) {
        draw_xpm(clock_sp->img, clock_sp->colors, cursor, y);
        cursor += clock_sp->img.width + 4;
    }

    if (remaining_secs < 0) remaining_secs = 0;
    if (remaining_secs > 5999) remaining_secs = 5999;

    char time_buffer[8];
    snprintf(time_buffer, sizeof(time_buffer), "%02d:%02d", remaining_secs / 60, remaining_secs % 60);
    draw_text(time_buffer, cursor + 10, y + 10);
}

void (draw_score)(sprite *star_sp, int score, int x, int y) {
    if (star_sp == NULL) return;

    star_sp->x = x;
    star_sp->y = y;

    draw_sprite(star_sp);
    
    char buf[16];
    snprintf(buf, sizeof(buf), "%d", score);
    draw_text(buf, x + star_sp->width + 10, y + 10);
}
