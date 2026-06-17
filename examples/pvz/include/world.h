#ifndef WORLD_H
#define WORLD_H

#include "zombie.h"

void update_world(Zombie *zombies, int max_zombies);
void draw_world(Zombie *zombies, int max_zombies);
void draw_paused_world(Zombie *zombies, int max_zombies);

#endif
