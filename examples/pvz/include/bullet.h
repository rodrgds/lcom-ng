#ifndef BULLET_H
#define BULLET_H

#include <stdbool.h>
#include "plant.h"

typedef enum {
    PROJECTILE_PEA,
    PROJECTILE_CABBAGE
} ProjectileType;

typedef struct {
    int x;
    int y;
    int start_x;
    int start_y;
    int base_y;
    int target_x;
    int total_distance;
    int row;
    int speed;
    int damage;
    int frame;
    int anim_counter;
    int distance_traveled;
    ProjectileType type;
    ProjectileTrajectory trajectory;
    bool has_target;
    bool exploding;
    bool active;
} Bullet;

void init_bullet(Bullet *bullet, int start_x, int start_y, int row, int damage,
                 ProjectileType type, ProjectileTrajectory trajectory);
void start_bullet_impact(Bullet *bullet);
void update_bullet(Bullet *bullet);

#endif
