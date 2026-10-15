#include "weapon_viewer_utils.h"
#include <math.h>
#include <stddef.h>
#include "../../../engine/graphics/renderer.h"
#include "../../../config.h"
#include "../../terrain/terrain.h"

void weapon_draw_rotated(const Projectile *p, Sprite *const *frames, uint8_t count) {
    if (p == NULL || frames == NULL || count == 0) return;

    Hitbox hb = projectile_get_hitbox(p);

    double vx = projectile_get_vx(p);
    double vy = projectile_get_vy(p);

    double angle = atan2(-vy, vx);
    if (angle < 0) {
        angle += 2 * M_PI;
    }

    int index = (int)((angle + M_PI / count) * count / (2 * M_PI)) % count;

    const Sprite *frame = frames[index];
    if (frame == NULL) return;

    int draw_x = hb.x - (sprite_get_width(frame) / 2) + (hb.width / 2);
    int draw_y = hb.y - (sprite_get_height(frame) / 2) + (hb.height / 2);

    renderer_draw_sprite(frame, draw_x, draw_y);
}

void weapon_draw_aim_trajectory(int wx, int wy, double aim_angle, int aim_power,
                                int dir, Sprite *const *frames, uint8_t count,
                                double projectile_speed, uint16_t projectile_size) {
    if (frames == NULL || count == 0) return;

    int index = (int)(aim_angle * count / (2 * M_PI)) + count / 4;
    index %= count;
    if (index < 0) index += count;
    const Sprite *frame = frames[index];
    if (frame == NULL) return;

    int tx = wx + dir * WEAPON_MUZZLE_OFFSET_X;
    int ty = wy + WEAPON_MUZZLE_OFFSET_Y;
    renderer_draw_sprite(frame, tx - sprite_get_width(frame) / 2, ty - sprite_get_height(frame) / 2);

    if (aim_power <= 0) return;

    double speed = projectile_speed * aim_power / (double)WEAPON_AIM_MAX_POWER;
    double ptx = tx - projectile_size / 2;
    double pty = ty - projectile_size / 2;
    double tvx = cos(aim_angle) * speed;
    double tvy = sin(aim_angle) * speed;

    for (int step = 0; step < WEAPON_AIM_PREVIEW_STEPS; step++) {
        ptx += tvx;
        pty += tvy;
        tvy += PROJECTILE_GRAVITY;

        int cx = (int)ptx + projectile_size / 2;
        int cy = (int)pty + projectile_size / 2;

        if (cx < 0 || cx >= SCREEN_W || cy >= SCREEN_H) break;
        if (cy >= 0 && terrain_collides(cx, cy)) break;

        if (cy >= 0 && step % WEAPON_AIM_DOT_INTERVAL == 0) {
            renderer_draw_rect(cx, cy, WEAPON_AIM_DOT_SIZE, WEAPON_AIM_DOT_SIZE, COLOR_WHITE);
        }
    }

    int bx = wx - WEAPON_AIM_BAR_OFFSET_X;
    int by = wy - WEAPON_AIM_BAR_OFFSET_Y;
    renderer_draw_rect(bx, by, WEAPON_AIM_BAR_W, WEAPON_AIM_BAR_H, COLOR_GREY);
    renderer_draw_rect(bx, by,
                       WEAPON_AIM_BAR_W * aim_power / WEAPON_AIM_MAX_POWER,
                       WEAPON_AIM_BAR_H, COLOR_LIME);
}
