#include "pvz_platform.h"
#include "lawnmower.h"

/* Array global — uma lawnmower por linha */
static Lawnmower lawnmowers[GRID_ROWS];

void init_lawnmowers(void) {
    for (int r = 0; r < GRID_ROWS; r++) {
        lawnmowers[r].row       = r;
        lawnmowers[r].x         = LAWNMOWER_START_X;
        lawnmowers[r].rolling   = false;
        lawnmowers[r].available = true;
    }
}

bool try_activate_lawnmower(int row) {
    if (row < 0 || row >= GRID_ROWS) return false;

    if (lawnmowers[row].available && !lawnmowers[row].rolling) {
        lawnmowers[row].rolling   = true;
        lawnmowers[row].available = false;
        return true;  /* zombie não conta como vida perdida */
    }
    return false;  /* já foi usada — perde vida */
}

bool is_lawnmower_rolling(int row) {
    if (row < 0 || row >= GRID_ROWS) return false;
    return lawnmowers[row].rolling;
}

const Lawnmower *get_lawnmowers(void) {
    return lawnmowers;
}

void update_lawnmowers(Zombie *zombies, int max_zombies) {
    for (int r = 0; r < GRID_ROWS; r++) {
        if (!lawnmowers[r].rolling) continue;

        /* Move para a direita */
        lawnmowers[r].x += LAWNMOWER_SPEED * DELTA_TIME;

        /* Mata todos os zombies nesta linha que a lawnmower alcança */
        for (int z = 0; z < max_zombies; z++) {
            if (!zombies[z].alive || zombies[z].row != r) continue;

            int z_x = COL_START_X[GRID_COLS - 1] + (int)zombies[z].curr_position;

            /* colisão simples: lawnmower sobrepõe o zombie */
            if (lawnmowers[r].x >= z_x - LAWNMOWER_W &&
                lawnmowers[r].x <= z_x + ZOMBIE_PLANT_COLLISION_W) {
                damage_zombie(&zombies[z], zombies[z].health);
            }
        }

        /* Sai do ecrã — desaparece */
        if (lawnmowers[r].x > LAWNMOWER_EXIT_X) {
            lawnmowers[r].rolling = false;
            /* available continua false — não volta a aparecer */
        }
    }
}
