#include "bazooka.h"
#include <stdlib.h>
#include "../../../config.h"
#include "../../game.h"
#include "../../terrain/terrain.h"
struct Bazooka {
    Projectile *projectile;
    uint32_t ticks;
};

Bazooka* bazooka_create(double x, double y, double vx, double vy) {
    Bazooka *bazooka = malloc(sizeof(Bazooka));
    if (bazooka == NULL) return NULL;

    bazooka->projectile = projectile_create(x, y, vx, vy, MISSILE_HITBOX_SIZE, MISSILE_HITBOX_SIZE, BAZOOKA_DAMAGE);
    if (bazooka->projectile == NULL) {
        free(bazooka);
        return NULL;
    }

    bazooka->ticks = 0;
    return bazooka;
}

void bazooka_destroy(Bazooka *bazooka) {
    if (bazooka == NULL) return;
    projectile_destroy(bazooka->projectile);
    free(bazooka);
}

void bazooka_update(Bazooka *bazooka) {
    if (bazooka == NULL || !bazooka_is_active(bazooka)) return;
    projectile_update(bazooka->projectile, SCREEN_W, SCREEN_H);
    bazooka->ticks++;
}

Projectile* bazooka_get_projectile(const Bazooka *bazooka) {
    if (bazooka == NULL) return NULL;
    return bazooka->projectile;
}

bool bazooka_is_active(const Bazooka *bazooka) {
    if (bazooka == NULL) return false;
    return projectile_is_active(bazooka->projectile);
}

void bazooka_deactivate(Bazooka *bazooka) {
    if (bazooka == NULL) return;
    projectile_deactivate(bazooka->projectile);
}

bool bazooka_check_collision(const Bazooka *bazooka, struct GameContext *ctx, int *out_cx, int *out_cy) {
    if (bazooka == NULL || ctx == NULL) return false;
    Projectile *p = bazooka->projectile;
    if (p == NULL) return false;

    Hitbox hb = projectile_get_hitbox(p);
    int cx = hb.x + hb.width / 2;
    int cy = hb.y + hb.height / 2;

    if (out_cx) *out_cx = cx;
    if (out_cy) *out_cy = cy;

    if (cy >= WATER_LEVEL_Y) return true;
    if (terrain_collides_hitbox(hb)) return true;

    int firing_team = game_context_get_current_team(ctx);
    for (int i = 0; i < MAX_WORMS; i++) {
        Worm *w = game_context_get_worm(ctx, i);
        if (w != NULL && worm_is_alive(w)) {
            if (game_context_get_worm_team(i) == firing_team &&
                bazooka->ticks < BAZOOKA_TEAM_INVULNERABLE_TICKS) {
                continue;
            }

            Hitbox whb = worm_get_damage_hitbox(w);
            if (hitbox_intersects(&hb, &whb)) return true;
        }
    }

    return false;
}
