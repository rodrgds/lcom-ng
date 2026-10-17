#include "projectile.h"
#include "v_card.h"
#include "game_state.h"
#include "zombie.h"
#include <stdio.h>
#include "sprites/plants/peashooter/peashooter_projectile.xpm"

#define MAX_PROJECTILES 30

typedef struct {
    bool active;
    int16_t x, y;
    uint8_t lane;
    int16_t damage;
} projectile_t;

static projectile_t projectiles[MAX_PROJECTILES];
static xpm_image_t peashooter_projectile_img;
static uint8_t *peashooter_projectile_pixmap = NULL;

void projectiles_init() {
    for (int i = 0; i < MAX_PROJECTILES; i++) projectiles[i].active = false;
    peashooter_projectile_pixmap = xpm_load((xpm_map_t) peashooter_projectile_xpm, XPM_8_8_8_8, &peashooter_projectile_img);
}

void spawn_projectile(uint8_t lane, int16_t x, int16_t y) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!projectiles[i].active) {
            projectiles[i].active = true;
            projectiles[i].x = x;
            projectiles[i].y = y;
            projectiles[i].lane = lane;
            projectiles[i].damage = 20;
            return;
        }
    }
}

void projectiles_update() {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!projectiles[i].active) continue;

        projectiles[i].x += 4;
        if (projectiles[i].x > 800) { 
            projectiles[i].active = false; 
            continue; 
        }

        // check hit
        if (zombies_check_hit(projectiles[i].lane, projectiles[i].x, projectiles[i].damage)) {
            projectiles[i].active = false;
        }
    }
}

void projectiles_render() {
    if (peashooter_projectile_pixmap == NULL) return;
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!projectiles[i].active) continue;
        int draw_x = projectiles[i].x - (peashooter_projectile_img.width / 2);
        int draw_y = projectiles[i].y - (peashooter_projectile_img.height / 2);
        vg_draw_xpm(peashooter_projectile_img, peashooter_projectile_pixmap, draw_x, draw_y);
    }
}
