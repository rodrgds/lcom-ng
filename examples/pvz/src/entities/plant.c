#include "pvz_platform.h"
#include <string.h>

#include "plant.h"
#include "constants.h"

static PlantClass get_plant_class(PlantType type) {
    switch(type) {
        case SUNFLOWER:
            return PRODUCER;
        case PEASHOOTER:
        case REPEATER:
        case CABBAGE:
            return SHOOTER;
        case POTATO_MINE:
            return MINE;
        case SPIKEWEED:
            return TRAP;
        case WALL_NUT:
        default:
            return OBSTACLE;
    }
}

static int get_plant_health(PlantType type) {
    switch(type) {
        case SUNFLOWER: return SUNFLOWER_HEALTH;
        case PEASHOOTER: return PEASHOOTER_HEALTH;
        case REPEATER: return REPEATER_HEALTH;
        case CABBAGE: return CABBAGE_HEALTH;
        case WALL_NUT: return WALL_NUT_HEALTH;
        case POTATO_MINE: return POTATO_MINE_HEALTH;
        case SPIKEWEED: return SPIKEWEED_HEALTH;
        default: return PEASHOOTER_HEALTH;
    }
}

int get_plant_cost(PlantType type) {
    switch(type) {
        case SUNFLOWER: return SUNFLOWER_COST;
        case PEASHOOTER: return PEASHOOTER_COST;
        case REPEATER: return REPEATER_COST;
        case CABBAGE: return CABBAGE_COST;
        case WALL_NUT: return WALL_NUT_COST;
        case POTATO_MINE: return POTATO_MINE_COST;
        case SPIKEWEED: return SPIKEWEED_COST;
        default: return PEASHOOTER_COST;
    }
}

static int get_shooting_frame_count(PlantType type) {
    switch(type) {
        case CABBAGE: return CABBAGE_SHOOTING_FRAMES;
        case REPEATER: return REPEATER_SHOOTING_FRAMES;
        case PEASHOOTER:
        default:
            return PEASHOOTER_SHOOTING_FRAMES;
    }
}

static int get_projectile_release_frame(PlantType type) {
    switch(type) {
        case CABBAGE: return CABBAGE_PROJECTILE_RELEASE_FRAME;
        default: return get_shooting_frame_count(type);
    }
}

static void release_plant_projectiles(ShooterPlant *shooter) {
    shooter->pending_projectiles = shooter->projectiles_per_shot;
    shooter->burst_cooldown = 0;
    shooter->isProjectilePending = true;
    shooter->projectile_released = true;
}

static void init_shooter(ShooterPlant *shooter, PlantType type) {
    memset(shooter, 0, sizeof(*shooter));

    shooter->trajectory = STRAIGHT_LINE;
    shooter->isShooting = false;
    shooter->isProjectilePending = false;
    shooter->projectile_released = false;

    switch(type) {
        case CABBAGE:
            shooter->fire_rate = CABBAGE_FIRE_RATE;
            shooter->projectile_damage = CABBAGE_PROJECTILE_DAMAGE;
            shooter->projectiles_per_shot = CABBAGE_PROJECTILE_COUNT;
            shooter->burst_interval = 0;
            shooter->trajectory = PARABOLA;
            break;

        case REPEATER:
            shooter->fire_rate = REPEATER_FIRE_RATE;
            shooter->projectile_damage = REPEATER_PROJECTILE_DAMAGE;
            shooter->projectiles_per_shot = REPEATER_PROJECTILE_COUNT;
            shooter->burst_interval = REPEATER_BURST_INTERVAL;
            break;

        case PEASHOOTER:
        default:
            shooter->fire_rate = PEASHOOTER_FIRE_RATE;
            shooter->projectile_damage = PEASHOOTER_PROJECTILE_DAMAGE;
            shooter->projectiles_per_shot = PEASHOOTER_PROJECTILE_COUNT;
            shooter->burst_interval = 0;
            break;
    }
}

static void init_producer(ProducerPlant *producer, PlantType type) {
    memset(producer, 0, sizeof(*producer));

    if (type == SUNFLOWER) {
        producer->production_rate = SUNFLOWER_PRODUCTION_RATE;
        producer->production_cooldown = SUNFLOWER_PRODUCTION_RATE;
    }
}

void init_plant(Plant *plant, int row, int col, PlantType type) {
    memset(plant, 0, sizeof(*plant));

    plant->row = row;
    plant->col = col;
    plant->type = type;
    plant->class = get_plant_class(type);

    plant->anim_speed = 10;
    plant->anim_counter = 0;

    plant->health = get_plant_health(type);
    plant->seed_cooldown = 0;
    plant->cost = get_plant_cost(type);
    plant->frame = 0;
    plant->arm_timer = 0;
    plant->attack_counter = 0;
    plant->attack_anim_counter = 0;
    plant->isAlive = true;
    plant->is_armed = type != POTATO_MINE;

    switch (plant->class) {
        case SHOOTER:
            init_shooter(&plant->shooter, type);
            break;
        case PRODUCER:
            init_producer(&plant->producer, type);
            break;
        case TRAP:
        case OBSTACLE:
        default:
            break;
    }
}

void update_plant_animation(Plant *plant) {
    ShooterPlant *shooter = plant->class == SHOOTER ? &plant->shooter : NULL;

    plant->anim_counter++;

    if (plant->anim_counter >= plant->anim_speed) {
        if (shooter != NULL && shooter->isShooting) {
            shooter->shooting_frame++;
        } else {
            plant->frame++;
        }

        plant->anim_counter = 0;

        if (shooter != NULL &&
            shooter->isShooting &&
            !shooter->projectile_released &&
            shooter->shooting_frame >= get_projectile_release_frame(plant->type)) {
            release_plant_projectiles(shooter);
        }

        if (shooter != NULL &&
            shooter->isShooting &&
            shooter->shooting_frame >= get_shooting_frame_count(plant->type)) {
            shooter->isShooting = false;
            shooter->shooting_frame = 0;

            if (!shooter->projectile_released) {
                release_plant_projectiles(shooter);
            }
        }
    }
}

int can_plant_shoot(Plant *plant, ShooterPlant *shooter) {
    return plant->class == SHOOTER &&
           shooter->fire_rate > 0 &&
           shooter->fire_cooldown <= 0 &&
           !shooter->isShooting &&
           !shooter->isProjectilePending;
}

void start_plant_shooting(Plant *plant, ShooterPlant *shooter) {
    shooter->isShooting = true;
    shooter->shooting_frame = 0;
    shooter->projectile_released = false;
    plant->anim_counter = 0;
    shooter->fire_cooldown = shooter->fire_rate;
}

void update_plant_fire_cooldown(ShooterPlant *shooter) {
    if (shooter->fire_cooldown > 0) {
        shooter->fire_cooldown--;
    }
}

void reset_plant_fire_cooldown(ShooterPlant *shooter) {
    shooter->fire_cooldown = 0;
}

int consume_plant_projectile_pending(ShooterPlant *shooter) {
    if (!shooter->isProjectilePending) return 0;

    if (shooter->burst_cooldown > 0) {
        shooter->burst_cooldown--;
        return 0;
    }

    shooter->pending_projectiles--;

    if (shooter->pending_projectiles <= 0) {
        shooter->pending_projectiles = 0;
        shooter->isProjectilePending = false;
    } else {
        shooter->burst_cooldown = shooter->burst_interval;
    }

    return 1;
}

void damage_plant(Plant *plant, int damage) {
    if (!plant->isAlive)
        return;

    plant->health -= damage;

    if (plant->health <= 0) {
        plant->health = 0;
        plant->isAlive = false;
    }
}

int is_plant_alive(Plant *plant) {
    return plant->isAlive;
}
