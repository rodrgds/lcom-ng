#ifndef PLANT_SYSTEM_H
#define PLANT_SYSTEM_H

#include "board.h"
#include "zombie.h"
#include "constants.h"
#include "projectile_system.h"

void update_plants(Board *board, Zombie *zombies, int max_zombies);

#endif
