#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <stdbool.h>
#include "constants.h"

typedef enum {
    REGULAR_ZOMBIE,
    CONEHEAD_ZOMBIE,
    POLE_VAULTING_ZOMBIE
} ZombieType;

typedef struct {

    int row;
    float curr_position;

    ZombieType type;

    int anim_counter;
    int anim_speed;

    int health;
    int speed;
    int damage;
    int attack_speed;
    int attack_cooldown;

    int frame;
    int jump_progress_ticks;

    float jump_start_position;
    float jump_target_position;

    bool alive;
    bool attacking;
    bool has_pole;
    bool is_jumping;
} Zombie;

/* Initialization */
void init_zombie(Zombie *zombie, int row, ZombieType type);
Zombie *get_zombies(void);
void clear_zombies(void);

/* Updates only this zombie's own movement/animation state. */
void move_zombie(Zombie *zombie);
void update_zombie_animation(Zombie *zombie);

/* Combat */
void damage_zombie(Zombie *zombie, int damage);

/* State */
bool is_zombie_alive(Zombie *zombie);
bool is_zombie_jumping(Zombie *zombie);
bool pole_vaulting_zombie_has_pole(Zombie *zombie);
bool try_start_pole_vault_jump(Zombie *zombie, int plant_col, int zombie_screen_x);

#endif
