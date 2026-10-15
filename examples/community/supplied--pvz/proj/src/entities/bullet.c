#include "bullet.h"
#include "constants.h"

void init_bullet(Bullet *bullet, int start_x, int start_y, int row, int damage,
                 ProjectileType type, ProjectileTrajectory trajectory) {
    bullet->x = start_x;
    bullet->y = start_y;
    bullet->start_x = start_x;
    bullet->start_y = start_y;
    bullet->base_y = start_y;
    bullet->target_x = start_x;
    bullet->total_distance = 1;
    bullet->row = row;
    bullet->speed = 4;     /* ~240px/s a 60Hz */
    bullet->damage = damage;
    bullet->frame = 0;
    bullet->anim_counter = 0;
    bullet->distance_traveled = 0;
    bullet->type = type;
    bullet->trajectory = trajectory;
    bullet->has_target = false;
    bullet->exploding = false;
    bullet->active = true;
}

void start_bullet_impact(Bullet *bullet) {
    if (bullet->type == PROJECTILE_CABBAGE) {
        bullet->active = false;
        return;
    }

    bullet->frame = PEA_IMPACT_FIRST_FRAME;
    bullet->anim_counter = 0;
    bullet->exploding = true;
}

void update_bullet(Bullet *bullet) {
    if (!bullet->active) return;

    bullet->anim_counter++;

    if (bullet->type == PROJECTILE_CABBAGE && !bullet->exploding) {
        if (bullet->anim_counter >= CABBAGE_PROJECTILE_FRAME_TICKS) {
            bullet->frame++;
            bullet->anim_counter = 0;

            if (bullet->frame >= CABBAGE_PROJECTILE_FRAMES) {
                bullet->frame = 0;
            }
        }
        return;
    }

    if (!bullet->exploding) return;

    if (bullet->anim_counter >= PEA_IMPACT_FRAME_TICKS) {
        bullet->frame++;
        bullet->anim_counter = 0;

        if (bullet->frame >= PEA_FRAMES) {
            bullet->active = false;
        }
    }
}
