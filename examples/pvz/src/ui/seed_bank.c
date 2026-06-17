#include <stdlib.h>

#include "seed_bank.h"
#include "constants.h"

/* Slots dinâmicos — configurados pelo ecrã de seleção */
static PlantType seed_slot_types[SEED_BANK_SLOTS] = {
    SUNFLOWER,
    PEASHOOTER,
    WALL_NUT,
    REPEATER,
    POTATO_MINE,
    SPIKEWEED
};

static int active_slots = SEED_BANK_SLOTS;

void set_seed_slots(const PlantType *types, int count) {
    int n = count < SEED_BANK_SLOTS ? count : SEED_BANK_SLOTS;
    for (int i = 0; i < n; i++) {
        seed_slot_types[i] = types[i];
    }
    active_slots = n;
}

PlantType get_seed_slot_type(int slot) {
    if (slot < 0 || slot >= active_slots) {
        return PEASHOOTER;
    }

    return seed_slot_types[slot];
}

int get_seed_slot_x(int slot) {
    return SEED_BANK_X + slot * (SEED_BANK_SLOT_W + SEED_BANK_SLOT_GAP);
}

int get_seed_slot_for_type(PlantType type) {
    for (int i = 0; i < active_slots; i++) {
        if (seed_slot_types[i] == type) {
            return i;
        }
    }

    return -1;
}

bool get_seed_from_position(int x, int y, PlantType *type) {
    if (type == NULL ||
        y < SEED_BANK_Y ||
        y >= SEED_BANK_Y + SEED_BANK_SLOT_H) {
        return false;
    }

    for (int i = 0; i < active_slots; i++) {
        int slot_x = get_seed_slot_x(i);
        if (x >= slot_x && x < slot_x + SEED_BANK_SLOT_W) {
            *type = seed_slot_types[i];
            return true;
        }
    }

    return false;
}
