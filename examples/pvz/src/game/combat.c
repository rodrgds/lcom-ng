#include "combat.h"
#include "constants.h"

static void reset_zombie_attacks(Zombie *zombies, int max_zombies) {
    for (int z = 0; z < max_zombies; z++) {
        zombies[z].attacking = false;
    }
}

static void damage_plant_if_ready(Plant *plant, Zombie *zombie) {
    zombie->attacking = true;

    if (zombie->attack_cooldown > 0) {
        return;
    }

    damage_plant(plant, zombie->damage);
    zombie->attack_cooldown = zombie->attack_speed;
}

static void update_zombie_eating(Board *board, Zombie *zombies, int max_zombies, int row, int col) {
    Plant *plant = &board->cells[row][col].plant;

    if (plant->type == POTATO_MINE && plant->is_exploding) {
        return;
    }

    if (plant->type == SPIKEWEED) {
        return;
    }

    for (int z = 0; z < max_zombies; z++) {
        if (is_zombie_alive(&zombies[z]) && zombies[z].row == row) {
            int z_x = COL_START_X[GRID_COLS - 1] + zombies[z].curr_position;

            if (is_zombie_jumping(&zombies[z])) {
                continue;
            }

            if (try_start_pole_vault_jump(&zombies[z], col, z_x)) {
                continue;
            }

            if (z_x >= COL_START_X[col] &&
                z_x <= COL_START_X[col] + ZOMBIE_PLANT_COLLISION_W) {
                damage_plant_if_ready(plant, &zombies[z]);

                if (!is_plant_alive(plant)) {
                    board->cells[row][col].is_occupied = false;
                    break;
                }
            }
        }
    }
}

void update_combat(Board *board, Zombie *zombies, int max_zombies) {
    reset_zombie_attacks(zombies, max_zombies);

    for (int r = 0; r < BOARD_ROWS; r++) {
        for (int c = 0; c < BOARD_COLS; c++) {
            if (!board->cells[r][c].is_occupied) {
                continue;
            }

            update_zombie_eating(board, zombies, max_zombies, r, c);
        }
    }
}
