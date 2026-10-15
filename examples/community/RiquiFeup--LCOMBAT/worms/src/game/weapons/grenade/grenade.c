#include "grenade.h"
#include <stdlib.h>
#include "../../../config.h"
#include "../../game.h"
#include "../../terrain/terrain.h"

struct Grenade {
    Projectile *projectile;
    uint32_t ticks;
};

Grenade* grenade_create(int32_t x, int32_t y, double vx, double vy) {
    Grenade *grenade = malloc(sizeof(Grenade));
    if (grenade == NULL) return NULL;

    grenade->projectile = projectile_create((double)x, (double)y, vx, vy, GRENADE_HITBOX_SIZE, GRENADE_HITBOX_SIZE, GRENADE_DAMAGE);
    if (grenade->projectile == NULL) {
        free(grenade);
        return NULL;
    }

    grenade->ticks = 0;
    return grenade;
}

void grenade_destroy(Grenade *grenade) {
    if (grenade == NULL) return;
    projectile_destroy(grenade->projectile);
    free(grenade);
}

void grenade_update(Grenade *grenade) {
    if (grenade == NULL || !grenade_is_active(grenade)) return;
    projectile_update(grenade->projectile, SCREEN_W, SCREEN_H);
    grenade->ticks++;
}

Projectile* grenade_get_projectile(const Grenade *grenade) {
    if (grenade == NULL) return NULL;
    return grenade->projectile;
}

bool grenade_is_active(const Grenade *grenade) {
    if (grenade == NULL) return false;
    return projectile_is_active(grenade->projectile);
}

void grenade_deactivate(Grenade *grenade) {
    if (grenade == NULL) return;
    projectile_deactivate(grenade->projectile);
}

void grenade_advance_ticks(Grenade *grenade) {
    if (grenade == NULL) return;
    grenade->ticks++;
}

uint32_t grenade_get_ticks(const Grenade *grenade) {
    if (grenade == NULL) return 0;
    return grenade->ticks;
}

bool grenade_check_collision(const Grenade *grenade, struct GameContext *ctx, int *out_cx, int *out_cy) {
    if (grenade == NULL || ctx == NULL) return false;
    Projectile *p = grenade->projectile;
    if (p == NULL) return false;

    (void)ctx;

    Hitbox hb = projectile_get_hitbox(p);
    int cx = hb.x + hb.width / 2;
    int cy = hb.y + hb.height / 2;

    if (out_cx) *out_cx = cx;
    if (out_cy) *out_cy = cy;

    if (cy >= WATER_LEVEL_Y) return true;
    if (terrain_collides_hitbox(hb)) return true;

    return false;
}
