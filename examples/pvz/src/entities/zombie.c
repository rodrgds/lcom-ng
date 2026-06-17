#include "pvz_platform.h"

#include "zombie.h"
#include "constants.h"

static Zombie zombies[MAX_ZOMBIES];

Zombie *get_zombies(void) {
    return zombies;
}

void clear_zombies(void) {
    for (int i = 0; i < MAX_ZOMBIES; i++) {
        zombies[i].alive = false;
    }
}

static void set_zombie_stats(Zombie *zombie, int health, int speed, int damage, int attack_speed) {
    zombie->health = health;
    zombie->speed = speed;
    zombie->damage = damage;
    zombie->attack_speed = attack_speed;
}

static void finish_pole_vault_jump(Zombie *zombie) {
    zombie->curr_position = zombie->jump_target_position;
    zombie->is_jumping = false;
    zombie->has_pole = false;
    zombie->jump_progress_ticks = 0;
    zombie->frame = 0;
    zombie->anim_counter = 0;
    zombie->anim_speed = 15;
    zombie->speed = ZOMBIE_SPEED;
}

void init_zombie(Zombie *zombie, int row, ZombieType type) {
    zombie->row = row;
    zombie->curr_position = ZOMBIE_INITIAL_OFFSET;
    zombie->type = type;

    zombie->frame = 0;
    zombie->jump_progress_ticks = 0;
    zombie->jump_start_position = 0.0f;
    zombie->jump_target_position = 0.0f;
    zombie->alive = true;
    zombie->attacking = false;
    zombie->has_pole = false;
    zombie->is_jumping = false;
    zombie->attack_cooldown = 0;

    zombie->anim_speed = 15;
    zombie->anim_counter = 0;

    switch(type) {
        case REGULAR_ZOMBIE:
            set_zombie_stats(zombie,
                             REGULAR_ZOMBIE_HEALTH,
                             ZOMBIE_SPEED,
                             ZOMBIE_DAMAGE,
                             REGULAR_ZOMBIE_ATTACK_SPEED);
            break;

        case CONEHEAD_ZOMBIE:
            set_zombie_stats(zombie,
                             CONEHEAD_ZOMBIE_HEALTH,
                             ZOMBIE_SPEED,
                             ZOMBIE_DAMAGE,
                             CONEHEAD_ZOMBIE_ATTACK_SPEED);
            break;

        case POLE_VAULTING_ZOMBIE:
            set_zombie_stats(zombie,
                             POLE_VAULTING_ZOMBIE_HEALTH,
                             POLE_VAULTING_ZOMBIE_SPEED,
                             ZOMBIE_DAMAGE,
                             POLE_VAULTING_ZOMBIE_ATTACK_SPEED);
            zombie->has_pole = true;
            break;
    }
}

static void start_pole_vault_jump(Zombie *zombie, int plant_col) {
    int landing_screen_x;

    if (zombie->type != POLE_VAULTING_ZOMBIE || !zombie->has_pole || zombie->is_jumping) {
        return;
    }

    landing_screen_x = COL_START_X[plant_col] - POLE_VAULTING_LANDING_OFFSET;

    zombie->is_jumping = true;
    zombie->attacking = false;
    zombie->frame = 0;
    zombie->anim_counter = 0;
    zombie->anim_speed = POLE_VAULTING_JUMP_ANIM_SPEED;
    zombie->jump_progress_ticks = 0;
    zombie->jump_start_position = zombie->curr_position;
    zombie->jump_target_position = (float) (landing_screen_x - COL_START_X[GRID_COLS - 1]);
}

bool is_zombie_jumping(Zombie *zombie) {
    return zombie->is_jumping;
}

bool pole_vaulting_zombie_has_pole(Zombie *zombie) {
    return zombie->type == POLE_VAULTING_ZOMBIE && zombie->has_pole;
}

bool try_start_pole_vault_jump(Zombie *zombie, int plant_col, int zombie_screen_x) {
    if (!pole_vaulting_zombie_has_pole(zombie) || zombie->is_jumping) {
        return false;
    }

    if (zombie_screen_x < COL_START_X[plant_col] || zombie_screen_x > COL_START_X[plant_col] + 60) {
        return false;
    }

    start_pole_vault_jump(zombie, plant_col);
    return true;
}

void update_zombie_animation(Zombie *zombie) {
    zombie->anim_counter++;

    if (zombie->anim_counter >= zombie->anim_speed) {
        if (zombie->type == POLE_VAULTING_ZOMBIE && zombie->is_jumping) {
            if (zombie->frame < POLE_VAULTING_JUMP_FRAMES - 1) {
                zombie->frame++;
            }
        } else {
            zombie->frame++;
        }

        zombie->anim_counter = 0;
    }
}

void move_zombie(Zombie *zombie) {
    if (!zombie->alive) {
        return;
    }

    if (zombie->is_jumping) {
        float progress;

        zombie->jump_progress_ticks++;
        progress = (float) zombie->jump_progress_ticks / POLE_VAULTING_JUMP_TICKS;

        if (progress >= 1.0f) {
            finish_pole_vault_jump(zombie);
            return;
        }

        zombie->curr_position = zombie->jump_start_position +
                                (zombie->jump_target_position - zombie->jump_start_position) * progress;
        return;
    }

    if (!zombie->attacking) {
        zombie->curr_position -= DELTA_TIME * zombie->speed;
    }

    if (zombie->attack_cooldown > 0) {
        zombie->attack_cooldown--;
    }
}

void damage_zombie(Zombie *zombie, int damage) {
    zombie->health -= damage;
    if (zombie->health <= 0) {
        zombie->health = 0;
        zombie->alive = false;
    }
}

bool is_zombie_alive(Zombie *zombie) {
    return zombie->alive;
}
