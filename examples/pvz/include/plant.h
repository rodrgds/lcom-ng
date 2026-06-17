#ifndef PLANT_H
#define PLANT_H

#include <stdbool.h>

typedef enum {
    SUNFLOWER,
    PEASHOOTER,
    REPEATER,
    CABBAGE,
    WALL_NUT,
    POTATO_MINE,
    SPIKEWEED
} PlantType;

typedef enum {
    PRODUCER,
    SHOOTER,
    OBSTACLE,
    MINE,
    TRAP
} PlantClass;

typedef enum {
    STRAIGHT_LINE,
    PARABOLA
} ProjectileTrajectory;

typedef struct {
    int fire_rate;
    int fire_cooldown;
    int projectile_damage;
    int projectiles_per_shot;
    int pending_projectiles;
    int burst_interval;
    int burst_cooldown;
    ProjectileTrajectory trajectory;
    int shooting_frame;
    bool isShooting;
    bool isProjectilePending;
    bool projectile_released;
} ShooterPlant;

typedef struct {
    int production_rate;
    int production_cooldown;
} ProducerPlant;

typedef struct {
    int row;
    int col;

    PlantType type;
    PlantClass class;

    int anim_counter;
    int anim_speed;

    int health;
    int seed_cooldown;
    int cost;
    int frame;
    int arm_timer;
    int explosion_frame;
    int explosion_counter;
    int attack_counter;
    int attack_anim_counter;

    bool isAlive;
    bool is_armed;
    bool is_exploding;
    bool is_attacking;

    ShooterPlant shooter;
    ProducerPlant producer;
} Plant;

void init_plant(Plant *plant, int row, int col, PlantType type);

void update_plant_animation(Plant *plant);
int can_plant_shoot(Plant *plant, ShooterPlant *shooter);
void start_plant_shooting(Plant *plant, ShooterPlant *shooter);
void update_plant_fire_cooldown(ShooterPlant *shooter);
void reset_plant_fire_cooldown(ShooterPlant *shooter);
int consume_plant_projectile_pending(ShooterPlant *shooter);

void damage_plant(Plant *plant, int damage);

int is_plant_alive(Plant *plant);
int get_plant_cost(PlantType type);

#endif
