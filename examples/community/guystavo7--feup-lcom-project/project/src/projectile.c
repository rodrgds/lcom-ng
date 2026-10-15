#include "../include/components/projectile.h"
/** @file projectile.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */
#include "../include/data.h"
#include "../resources/particles/dust/dust.h"
#include <math.h>
#include <stdlib.h>

static void drawCircle(Terminal* terminal, int xm, int ym, int r, uint32_t color) {
    int x = -r, y = 0, err = 2 - 2 * r;
    do {
        drawPixel(terminal, xm - x, ym + y, color);
        drawPixel(terminal, xm - y, ym - x, color);
        drawPixel(terminal, xm + x, ym - y, color);
        drawPixel(terminal, xm + y, ym + x, color);
        r = err;
        if (r <= y) err += ++y * 2 + 1;
        if (r > x || err > y) err += ++x * 2 + 1;
    } while (x < 0);
}

struct projectile {
    ProjectileType type;
    Vec2d* pos;
    Vec2d* vel;
    AnimatedSprite* sprite;
    // SpriteXPM* sprite;
    BoxCollider* collider;
    union {
        struct {
            int max_pierces;
            int current_pierces;
        } piercing;

        struct {
            float target_x;
            float target_y;
            float turn_speed;
        } homing;

        struct {
            float target_x;
            float target_y;
            float gravity;
            bool landed;
            AnimatedSprite* landing_sprites[3];
        } sky_fall;

        struct {
            bool damage_dealt;
        } near;
    } extra_data;

    BoxCollider* hit_enemies[16];
    int hit_count;

    bool active;
};

Projectile* projectileCreate(ProjectileType type, float x, float y, float vel_x, float vel_y, AnimatedSprite* sprite) {
    Projectile* p = malloc(sizeof(Projectile));
    if (!p) return NULL;

    p->type = type;
    if (type == PROJ_SKY_FALL) {
        p->pos = vec2dCreate(x, y - 400.0f);
        p->vel = vec2dCreate(0.0f, 0.0f);
    } else {
        p->pos = vec2dCreate(x, y);
        p->vel = vec2dCreate(vel_x, vel_y);
    }
    p->sprite = sprite;
    p->active = true;
    p->hit_count = 0;

    int w = 16;
    int h = 16;
    if (sprite != NULL) {
        w = animatedSpriteGetWidth(sprite);
        h = animatedSpriteGetHeight(sprite);
    }

    Vec2d pos = {p->pos->x, p->pos->y};
    Vec2d offset = {0, 0};
    Vec2d size = {w, h};
    p->collider = boxColliderNew(pos, offset, size, 0);

    if (type == PROJ_PIERCING) {
        p->extra_data.piercing.max_pierces = 2;
        p->extra_data.piercing.current_pierces = 0;
    } else if (type == PROJ_HOMING) {
        p->extra_data.homing.target_x = x;
        p->extra_data.homing.target_y = y;
        p->extra_data.homing.turn_speed = 50.0f;
    } else if (type == PROJ_SKY_FALL) {
        p->extra_data.sky_fall.target_x = x;
        p->extra_data.sky_fall.target_y = y;
        p->extra_data.sky_fall.gravity = 380.0f;
        p->extra_data.sky_fall.landed = false;

        SpriteXPM** sprites1 = malloc(sizeof(SpriteXPM*) * DUST_FRAMES);
        SpriteXPM** sprites2 = malloc(sizeof(SpriteXPM*) * DUST_FRAMES);
        SpriteXPM** sprites3 = malloc(sizeof(SpriteXPM*) * DUST_FRAMES);
        for (int i = 0; i < DUST_FRAMES; i++) {
            sprites1[i] = newSpriteXPM((xpm_map_t)dustXPM[i], XPM_8_8_8, 0, 0);
            sprites2[i] = newSpriteXPM((xpm_map_t)dustXPM[i], XPM_8_8_8, 0, 0);
            sprites3[i] = newSpriteXPM((xpm_map_t)dustXPM[i], XPM_8_8_8, 0, 0);
        }
        p->extra_data.sky_fall.landing_sprites[0] = animatedSpriteNew(DUST_FRAME_SPEED, sprites1, DUST_FRAMES, ONE_SHOT);
        p->extra_data.sky_fall.landing_sprites[1] = animatedSpriteNew(DUST_FRAME_SPEED, sprites2, DUST_FRAMES, ONE_SHOT);
        p->extra_data.sky_fall.landing_sprites[2] = animatedSpriteNew(DUST_FRAME_SPEED, sprites3, DUST_FRAMES, ONE_SHOT);
    } else if (type == PROJ_NEAR) {
        p->extra_data.near.damage_dealt = false;
    }

    return p;
}

void projectileReset(Projectile* p, ProjectileType type, float x, float y, float vel_x, float vel_y, SpriteXPM** arrowSprites, int index) {
    if (!p) return;

    p->type = type;
    p->active = true;
    p->hit_count = 0;

    if (type == PROJ_SKY_FALL) {
        vec2dSetX(p->pos, x);
        vec2dSetY(p->pos, -100.0f);
        vec2dSetX(p->vel, 0.0f);
        vec2dSetY(p->vel, 0.0f);
    } else {
        vec2dSetX(p->pos, x);
        vec2dSetY(p->pos, y);
        vec2dSetX(p->vel, vel_x);
        vec2dSetY(p->vel, vel_y);
    }

    if (p->sprite) {
        if (type == PROJ_NORMAL || type == PROJ_PIERCING) {
            animatedSpriteSetSpriteAtIndex(p->sprite, 0, arrowSprites[index]);
        }
        animatedSpriteStart(p->sprite);
    }

    boxColliderSetX(p->collider, vec2dGetX(p->pos));
    boxColliderSetY(p->collider, vec2dGetY(p->pos));

    if (type == PROJ_PIERCING) {
        p->extra_data.piercing.max_pierces = 2;
        p->extra_data.piercing.current_pierces = 0;
    } else if (type == PROJ_HOMING) {
        p->extra_data.homing.target_x = x;
        p->extra_data.homing.target_y = y;
        p->extra_data.homing.turn_speed = 50.0f;
    } else if (type == PROJ_SKY_FALL) {
        p->extra_data.sky_fall.target_x = x;
        p->extra_data.sky_fall.target_y = y;
        p->extra_data.sky_fall.gravity = 380.0f;
        p->extra_data.sky_fall.landed = false;
        for (int i = 0; i < 3; i++) {
            animatedSpriteStart(p->extra_data.sky_fall.landing_sprites[i]);
        }
    } else if (type == PROJ_NEAR) {
        p->extra_data.near.damage_dealt = false;
    }
}

void projectileFree(Projectile* p) {
    if (!p) return;
    vec2dFree(p->pos);
    vec2dFree(p->vel);
    boxColliderFree(p->collider);
    if (p->type == PROJ_SKY_FALL) {
        for (int i = 0; i < 3; i++) {
            if (p->extra_data.sky_fall.landing_sprites[i]) {
                animatedSpriteFree(p->extra_data.sky_fall.landing_sprites[i]);
            }
        }
    }
    // The sprites are stored in the tower so calling animatedSpriteFree will destroy everything
    free(p->sprite);
    free(p);
}

static void homingProjectileUpdate(Projectile* p, double delta_t, Terminal* terminal) {
    // logica simples de virar para o target
    float dx = p->extra_data.homing.target_x - vec2dGetX(p->pos);
    float dy = p->extra_data.homing.target_y - vec2dGetY(p->pos);
    float dist = sqrt(dx * dx + dy * dy);

    if (dist > EPSILON) {
        float ndx = dx / dist;
        float ndy = dy / dist;

        float current_vel_x = vec2dGetX(p->vel);
        float current_vel_y = vec2dGetY(p->vel);
        float current_speed = sqrt(current_vel_x * current_vel_x + current_vel_y * current_vel_y);

        // Aproximar a velocidade da direção do target (interpolação simples)
        float turn = p->extra_data.homing.turn_speed * delta_t;
        float new_vel_x = ndx * turn;
        float new_vel_y = ndy * turn;

        // Normalizar a nova velocidade para manter o speed original
        float new_speed = sqrt(new_vel_x * new_vel_x + new_vel_y * new_vel_y);
        if (new_speed > EPSILON) {
            vec2dSetX(p->vel, (new_vel_x / new_speed) * current_speed);
            vec2dSetY(p->vel, (new_vel_y / new_speed) * current_speed);
        }
    }
}

void skyFallUpdate(Projectile* p, double delta_t, Terminal* terminal) {
    if (p->extra_data.sky_fall.landed) {
        if (animatedSpriteIsFinished(p->extra_data.sky_fall.landing_sprites[0])) {
            p->active = false;
        } else {
            int cx = (int)p->extra_data.sky_fall.target_x + 38;
            int cy = (int)p->extra_data.sky_fall.target_y + 100;
            animatedSpriteUpdate(cx, cy, p->extra_data.sky_fall.landing_sprites[0], terminal, delta_t, false);
            animatedSpriteUpdate(cx - 60, cy - 20, p->extra_data.sky_fall.landing_sprites[1], terminal, delta_t, false);
            animatedSpriteUpdate(cx + 60, cy - 20, p->extra_data.sky_fall.landing_sprites[2], terminal, delta_t, false);
        }
        return;
    }

    if (vec2dGetY(p->pos) >= p->extra_data.sky_fall.target_y) {
        p->extra_data.sky_fall.landed = true;
        for (int i = 0; i < 3; i++) {
            animatedSpriteStart(p->extra_data.sky_fall.landing_sprites[i]);
        }
        return;
    }

    float current_vel_y = vec2dGetY(p->vel);
    float new_vel_y = current_vel_y + p->extra_data.sky_fall.gravity * delta_t;
    vec2dSetY(p->vel, new_vel_y);

    int w = 16;
    int h = 16;
    if (p->sprite != NULL) {
        w = animatedSpriteGetWidth(p->sprite);
        h = animatedSpriteGetHeight(p->sprite);
    }
    int cx = (int)p->extra_data.sky_fall.target_x + w / 2;
    int cy = (int)p->extra_data.sky_fall.target_y + h / 2;
    drawCircle(terminal, cx, cy, w / 2, 0xFF0000);
}

void projectileUpdate(Projectile* p, double delta_t, Terminal* terminal) {
    if (!p->active) return;

    if (p->type == PROJ_HOMING) {
        homingProjectileUpdate(p, delta_t, terminal);
    } else if (p->type == PROJ_SKY_FALL) {
        skyFallUpdate(p, delta_t, terminal);
    }

    Vec2d* displacement = vec2dCreate(vec2dGetX(p->vel), vec2dGetY(p->vel));
    vec2dMultiply(displacement, delta_t);
    vec2dAddVec(p->pos, displacement);
    vec2dFree(displacement);

    boxColliderSetX(p->collider, vec2dGetX(p->pos));
    boxColliderSetY(p->collider, vec2dGetY(p->pos));

    if (p->sprite != NULL && !(p->type == PROJ_SKY_FALL && p->extra_data.sky_fall.landed)) {
        animatedSpriteUpdate((int)p->pos->x, (int)p->pos->y, p->sprite, terminal, delta_t, false);
        if (p->type == PROJ_NEAR && animatedSpriteIsFinished(p->sprite)) {
            p->active = false;
        }
    }
}

void projectileUpdateHomingTarget(Projectile* p, float target_x, float target_y) {
    if (p->type == PROJ_HOMING) {
        p->extra_data.homing.target_x = target_x;
        p->extra_data.homing.target_y = target_y;
    }
}

void projectileUpdateSkyFallTarget(Projectile* p, float target_x, float target_y) {
    if (p->type == PROJ_SKY_FALL) {
        p->extra_data.sky_fall.target_x = target_x;
        p->extra_data.sky_fall.target_y = target_y;
        vec2dSetX(p->pos, target_x);
    }
}

float projectileGetX(Projectile* p) {
    return vec2dGetX(p->pos);
}

float projectileGetY(Projectile* p) {
    return vec2dGetY(p->pos);
}

float projectileGetVelX(Projectile* p) {
    return vec2dGetX(p->vel);
}

float projectileGetVelY(Projectile* p) {
    return vec2dGetY(p->vel);
}

ProjectileType projectileGetType(Projectile* p) {
    return p->type;
}

bool projectileIsActive(Projectile* p) {
    return p->active;
}

void projectileSetActive(Projectile* p, bool active) {
    p->active = active;
}

bool projectileCanDealDamage(Projectile* p) {
    if (p->type == PROJ_NEAR) {
        return !p->extra_data.near.damage_dealt;
    }
    return true;
}

BoxCollider* projectileGetCollider(Projectile* p) {
    return p->collider;
}

int projectileGetPierces(Projectile* p) {
    if (p->type == PROJ_PIERCING) {
        return p->extra_data.piercing.current_pierces;
    }
    return 0;
}

void projectileAddPierce(Projectile* p) {
    if (p->type == PROJ_PIERCING) {
        p->extra_data.piercing.current_pierces++;
        if (p->extra_data.piercing.current_pierces >= p->extra_data.piercing.max_pierces) {
            p->active = false;
        }
    } else if (p->type == PROJ_NEAR) {
        p->extra_data.near.damage_dealt = true;
    } else if (p->type == PROJ_SKY_FALL) {
        // Do not deactivate sky fall projectiles on hit
    } else {
        p->active = false;
    }
}

void projectileSetMaxPierce(Projectile* p, int max) {
    if (p->type == PROJ_PIERCING) {
        p->extra_data.piercing.max_pierces = max;
    }
}

void projectileUpdadeSpeed(Projectile* p, float incX, float incY) {
    if (p->type == PROJ_SKY_FALL) {
        vec2dSetX(p->vel, vec2dGetX(p->vel) + incX);
        vec2dSetY(p->vel, vec2dGetY(p->vel) + incY);
    }
}

bool projectileHasHit(Projectile* p, BoxCollider* enemy) {
    for (int i = 0; i < p->hit_count; i++) {
        if (p->hit_enemies[i] == enemy) return true;
    }
    return false;
}

void projectileAddHit(Projectile* p, BoxCollider* enemy) {
    if (p->hit_count < 16) {
        p->hit_enemies[p->hit_count++] = enemy;
    }
}
