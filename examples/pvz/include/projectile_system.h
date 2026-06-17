#ifndef PROJECTILE_SYSTEM_H
#define PROJECTILE_SYSTEM_H

#include "board.h"
#include "plant.h"
#include "zombie.h"

void spawn_projectile(Board *board, int row, int start_x, int start_y, int damage,
                      ProjectileTrajectory trajectory);
void update_projectiles(Board *board, Zombie *zombies, int max_zombies);
void draw_projectiles(Board *board);

#endif
