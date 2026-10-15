#include "projectile_system.h"
#include "constants.h"
#include "render.h"

static ProjectileType projectile_type_for_trajectory(ProjectileTrajectory trajectory) {
    return trajectory == PARABOLA ? PROJECTILE_CABBAGE : PROJECTILE_PEA;
}

static void lock_parabolic_target(Bullet *bullet, Zombie *zombies, int max_zombies) {
    int best_target = GRID_END_X;
    bool found = false;

    for (int z = 0; z < max_zombies; z++) {
        if (zombies[z].alive && zombies[z].row == bullet->row) {
            int z_x = COL_START_X[GRID_COLS - 1] + zombies[z].curr_position;
            int target_x = z_x + PLANT_SHOOT_X_OFFSET;

            if (target_x > bullet->start_x &&
                (!found || target_x < best_target)) {
                best_target = target_x;
                found = true;
            }
        }
    }

    if (!found || best_target <= bullet->start_x) {
        best_target = bullet->start_x + 240;
    }

    bullet->target_x = best_target;
    bullet->total_distance = bullet->target_x - bullet->start_x;
    if (bullet->total_distance <= 0) {
        bullet->total_distance = 1;
    }
    bullet->has_target = true;
}

static void hit_zombie_at_projectile(Board *board, int bullet_index, Zombie *zombies, int max_zombies) {
    Bullet *bullet = &board->bullets[bullet_index];

    for (int z = 0; z < max_zombies; z++) {
        if (zombies[z].alive && zombies[z].row == bullet->row) {
            int z_x = COL_START_X[GRID_COLS - 1] + zombies[z].curr_position;

            if (bullet->x >= z_x - ZOMBIE_HITBOX_LEFT_PADDING &&
                bullet->x <= z_x + ZOMBIE_HITBOX_RIGHT_PADDING) {
                damage_zombie(&zombies[z], bullet->damage);
                start_bullet_impact(bullet);
                return;
            }
        }
    }
}

static bool update_parabolic_projectile(Bullet *bullet) {
    int progress_num;
    int arc_offset;

    bullet->x += bullet->speed;
    bullet->distance_traveled = bullet->x - bullet->start_x;

    progress_num = bullet->distance_traveled;
    if (progress_num < 0) progress_num = 0;
    if (progress_num > bullet->total_distance) {
        progress_num = bullet->total_distance;
    }

    arc_offset = (4 * CABBAGE_PROJECTILE_ARC_HEIGHT *
                  progress_num *
                  (bullet->total_distance - progress_num)) /
                 (bullet->total_distance * bullet->total_distance);
    bullet->y = bullet->base_y - arc_offset;

    return bullet->x >= bullet->target_x;
}

void spawn_projectile(Board *board, int row, int start_x, int start_y, int damage,
                      ProjectileTrajectory trajectory) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!board->bullets[i].active) {
            init_bullet(&board->bullets[i], start_x, start_y, row, damage,
                        projectile_type_for_trajectory(trajectory), trajectory);
            break;
        }
    }
}

void update_projectiles(Board *board, Zombie *zombies, int max_zombies) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!board->bullets[i].active) {
            continue;
        }

        if (board->bullets[i].exploding) {
            update_bullet(&board->bullets[i]);
            continue;
        }

        if (board->bullets[i].trajectory == PARABOLA) {
            if (!board->bullets[i].has_target) {
                lock_parabolic_target(&board->bullets[i], zombies, max_zombies);
            }
            if (update_parabolic_projectile(&board->bullets[i])) {
                hit_zombie_at_projectile(board, i, zombies, max_zombies);
                board->bullets[i].active = false;
                continue;
            }
        } else {
            board->bullets[i].x += board->bullets[i].speed;
        }

        update_bullet(&board->bullets[i]);

        if (board->bullets[i].x > SCREEN_W) {
            board->bullets[i].active = false;
            continue;
        }

        hit_zombie_at_projectile(board, i, zombies, max_zombies);
    }
}

void draw_projectiles(Board *board) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (board->bullets[i].active) {
            if (board->bullets[i].type == PROJECTILE_CABBAGE) {
                draw_cabbage_projectile(board->bullets[i].x, board->bullets[i].y,
                                        board->bullets[i].frame);
            } else {
                draw_bullet(board->bullets[i].x, board->bullets[i].y,
                            board->bullets[i].frame);
            }
        }
    }
}
