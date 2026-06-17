#include "plant_system.h"

static bool zombie_is_in_front_of_plant(Zombie *zombie, int row, int col) {
    int z_x = COL_START_X[GRID_COLS - 1] + zombie->curr_position;
    int zombie_center_x = z_x + ZOMBIE_CENTER_X_OFFSET;
    int plant_shoot_x = COL_START_X[col] + PLANT_SHOOT_X_OFFSET;

    return zombie->alive &&
           zombie->row == row &&
           z_x <= GRID_END_X &&
           zombie_center_x >= plant_shoot_x;
}

static bool plant_has_zombie_ahead(Zombie *zombies, int max_zombies, int row, int col) {
    for (int z = 0; z < max_zombies; z++) {
        if (zombie_is_in_front_of_plant(&zombies[z], row, col)) {
            return true;
        }
    }

    return false;
}

static void update_plant_shooting(Board *board, int row, int col, bool zombie_ahead) {
    Plant *plant = &board->cells[row][col].plant;
    ShooterPlant *shooter = &plant->shooter;

    if (plant->class != SHOOTER) {
        return;
    }

    if (consume_plant_projectile_pending(shooter)) {
        spawn_projectile(
            board,
            row,
            COL_START_X[col] + PLANT_SHOOT_X_OFFSET,
            ROW_START_Y[row] + (shooter->trajectory == PARABOLA ? -4 : 10),
            shooter->projectile_damage,
            shooter->trajectory
        );
    }

    if (zombie_ahead) {
        if (can_plant_shoot(plant, shooter)) {
            start_plant_shooting(plant, shooter);
        } else {
            update_plant_fire_cooldown(shooter);
        }
    } else {
        reset_plant_fire_cooldown(shooter);
    }
}

static void update_sunflower_production(Board *board, int row, int col) {
    Plant *plant = &board->cells[row][col].plant;
    ProducerPlant *producer = &plant->producer;

    if (plant->class != PRODUCER) {
        return;
    }

    if (producer->production_cooldown <= 0) {
        spawn_sun_at(COL_START_X[col], ROW_START_Y[row]);
        producer->production_cooldown = producer->production_rate;
    } else {
        producer->production_cooldown--;
    }
}

static void update_potato_mine(Board *board, Zombie *zombies, int max_zombies, int row, int col) {
    Plant *plant = &board->cells[row][col].plant;
    bool exploded = false;

    if (plant->type != POTATO_MINE) {
        return;
    }

    if (plant->is_exploding) {
        plant->explosion_counter++;
        if (plant->explosion_counter >= POTATO_MINE_EXPLOSION_SPEED) {
            plant->explosion_counter = 0;
            plant->explosion_frame++;
        }

        if (plant->explosion_frame >= POTATO_MINE_EXPLOSION_FRAMES) {
            board->cells[row][col].is_occupied = false;
        }
        return;
    }

    if (!plant->is_armed) {
        plant->arm_timer++;
        if (plant->arm_timer >= POTATO_MINE_ARM_TICKS) {
            plant->is_armed = true;
        }
        return;
    }

    for (int z = 0; z < max_zombies; z++) {
        int z_x = COL_START_X[GRID_COLS - 1] + zombies[z].curr_position;

        if (zombies[z].alive &&
            zombies[z].row == row &&
            z_x >= COL_START_X[col] &&
            z_x <= COL_START_X[col] + ZOMBIE_PLANT_COLLISION_W) {
            damage_zombie(&zombies[z], POTATO_MINE_DAMAGE);
            exploded = true;
        }
    }

    if (exploded) {
        plant->is_exploding = true;
        plant->explosion_frame = 0;
        plant->explosion_counter = 0;
    }
}

static bool zombie_above_spikeweed(Zombie *zombie, int row, int col) {
    int z_x = COL_START_X[GRID_COLS - 1] + zombie->curr_position;
    int zombie_center_x = z_x + ZOMBIE_CENTER_X_OFFSET;

    return zombie->alive &&
           zombie->row == row &&
           zombie_center_x >= COL_START_X[col] &&
           zombie_center_x <= COL_START_X[col] + GRID_CELL_W;
}

static void update_spikeweed(Board *board, Zombie *zombies, int max_zombies, int row, int col) {
    Plant *plant = &board->cells[row][col].plant;
    bool zombie_present = false;

    if (plant->type != SPIKEWEED) {
        return;
    }

    if (plant->attack_anim_counter > 0) {
        plant->attack_anim_counter--;
    }
    plant->is_attacking = plant->attack_anim_counter > 0;

    for (int z = 0; z < max_zombies; z++) {
        if (zombie_above_spikeweed(&zombies[z], row, col)) {
            zombie_present = true;
            break;
        }
    }

    if (!zombie_present) {
        plant->attack_counter = 0;
        return;
    }

    if (plant->attack_counter > 0) {
        plant->attack_counter--;
        return;
    }

    for (int z = 0; z < max_zombies; z++) {
        if (zombie_above_spikeweed(&zombies[z], row, col)) {
            damage_zombie(&zombies[z], SPIKEWEED_DAMAGE);
        }
    }

    plant->attack_counter = SPIKEWEED_ATTACK_RATE;
    plant->is_attacking = true;
    plant->attack_anim_counter = SPIKEWEED_ATTACK_ANIM_TICKS;
}

void update_plants(Board *board, Zombie *zombies, int max_zombies) {
    for (int r = 0; r < BOARD_ROWS; r++) {
        for (int c = 0; c < BOARD_COLS; c++) {
            if (!board->cells[r][c].is_occupied) {
                continue;
            }

            update_plant_animation(&board->cells[r][c].plant);
            update_potato_mine(board, zombies, max_zombies, r, c);
            update_spikeweed(board, zombies, max_zombies, r, c);
            if (!board->cells[r][c].is_occupied) {
                continue;
            }
            update_plant_shooting(board, r, c,
                                  plant_has_zombie_ahead(zombies, max_zombies, r, c));
            update_sunflower_production(board, r, c);
        }
    }
}
