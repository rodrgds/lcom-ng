#include "sun.h"
#include "v_card.h"
#include "game_state.h"
#include <stdlib.h>
#include <stdio.h>
#include "sprites/misc/sun.xpm"
#include "sprites/misc/sun_fading.xpm"

#define SUN_BLINK_THRESHOLD 180
#define SUN_BLINK_PERIOD 15

static sun_t suns_array[MAX_SUNS];
static xpm_image_t sun_img;
static uint8_t *sun_pixmap = NULL;
static xpm_image_t sun_fade_img;
static uint8_t *sun_fade_pixmap = NULL;

void suns_init() {
    for (int i = 0; i < MAX_SUNS; i++) suns_array[i].active = false;
    
    sun_pixmap = xpm_load((xpm_map_t) sun_xpm, XPM_8_8_8_8, &sun_img);
    if (sun_pixmap == NULL) printf("Error loading sun xpm.\n");

    sun_fade_pixmap = xpm_load((xpm_map_t) sun_fading_xpm, XPM_8_8_8_8, &sun_fade_img);
    if (sun_fade_pixmap == NULL) printf("Error loading sun fading xpm.\n");
}

void spawn_sun(int16_t x, int16_t start_y, int16_t target_y) {
    for (int i = 0; i < MAX_SUNS; i++) {
        if (!suns_array[i].active) {
            suns_array[i].active = true;
            suns_array[i].x = x;
            suns_array[i].y = start_y;
            suns_array[i].target_y = target_y;
            suns_array[i].lifetime = 600;
            return;
        }
    }
}

void suns_update() {
    // sun fall
    for (int i = 0; i < MAX_SUNS; i++) {
        if (!suns_array[i].active) continue;
        if (suns_array[i].y < suns_array[i].target_y) suns_array[i].y += 1;
        
        suns_array[i].lifetime--;
        if (suns_array[i].lifetime == 0) suns_array[i].active = false;
    }

    // sun spawn from sky
    if (timer_counter % 300 == 0) {
        int margin = 26;
        int16_t sx = margin + (rand() % (800 - 2 * margin));
        int16_t sy = 100 + margin + (rand() % (500 - 2 * margin));

        int16_t start_y = sy - 200;
        if (start_y < margin) {
            start_y = margin;
        }

        spawn_sun(sx, start_y, sy);
    }
}

void suns_render() {
    for (int i = 0; i < MAX_SUNS; i++) {
        if (!suns_array[i].active) continue;

        bool use_fade = suns_array[i].lifetime <= SUN_BLINK_THRESHOLD &&
                        ((suns_array[i].lifetime / SUN_BLINK_PERIOD) % 2 == 0);

        xpm_image_t current_img = use_fade && sun_fade_pixmap != NULL ? sun_fade_img : sun_img;
        uint8_t *current_pixmap = use_fade && sun_fade_pixmap != NULL ? sun_fade_pixmap : sun_pixmap;

        if (current_pixmap != NULL) {
            int draw_x = suns_array[i].x - (current_img.width / 2);
            int draw_y = suns_array[i].y - (current_img.height / 2);
            vg_draw_xpm(current_img, current_pixmap, draw_x, draw_y);
        }
    }
}

bool suns_check_click(int16_t mx, int16_t my) {
    for (int i = 0; i < MAX_SUNS; i++) {
        if (!suns_array[i].active) continue;
        if (mx >= suns_array[i].x - 25 && mx <= suns_array[i].x + 25 &&
            my >= suns_array[i].y - 25 && my <= suns_array[i].y + 25) {
            suns_count += 25;
            suns_array[i].active = false;
            return true;
        }
    }
    return false;
}
