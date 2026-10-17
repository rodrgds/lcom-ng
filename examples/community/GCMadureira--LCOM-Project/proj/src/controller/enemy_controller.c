/** 
 * @file enemy_controller.c
 * @brief Source code to control the enemies
 * 
 * This file handles the logic for the enemies, including the their spawn mechanics and collisions.
 */

#include "enemy_controller.h"

/**
 * @def MAX_ENEMIES
 * @brief Maximum number of enemies that can exist at once.
 *
 * Increases by 1 every two seconds (every 120 frames).
 */
#define MAX_ENEMIES 50 + (get_current_frame() - get_arena_starting_frame())/120

/**
 * @def NORMAL_SPAWN_RATE
 * @brief Base enemy spawn rate, starts at 2 per second and increases over time.
 *
 * Starts at 120 frames per enemy (2/sec), down to 10 frames per enemy (6/sec) as time progresses.
 */
#define NORMAL_SPAWN_RATE 120 - MIN((get_current_frame() - get_arena_starting_frame())/120, 110)

/**
 * @def MUMMY_SPAWN_RATE
 * @brief Mummy enemies spawn rate.
 *
 * No mummy spawns for the first 2 minutes (7200 frames). After that, spawn rate follows the normal spawn rate formula with a 2-minute delay.
 */
#define MUMMY_SPAWN_RATE ((get_current_frame() - get_arena_starting_frame()) < 7200 ? INFINITY : 120 - MIN((get_current_frame() - get_arena_starting_frame() - 7200)/60, 110))

static uint32_t enemy_count = 0;                  ///< Track the active enemy count
static unsigned long last_normal_enemy_spawn = 0; ///< Track when we last spawned a normal enemy
static unsigned long last_mummy_spawn = 0;        ///< Track when we last spawned a mummy enenmy

void (setup_enemy_controller)() {
  enemy_count = 0;
  last_normal_enemy_spawn = 0;
  last_mummy_spawn = 0;
}

void (handle_enemy_spawning)(arena* arena) {
  // Spawn a new enemy every 120 frames (2 seconds at 60 FPS) when starting
  if (get_current_frame() - last_normal_enemy_spawn >= NORMAL_SPAWN_RATE  && enemy_count < MAX_ENEMIES) {
    spawn_enemy(arena, 2, 1, rand()%2 == 0 ? enemy1_animations : enemy2_animations);
    last_normal_enemy_spawn = get_current_frame();
    enemy_count++;
  }
  if (get_current_frame() - last_mummy_spawn >= MUMMY_SPAWN_RATE  && enemy_count < MAX_ENEMIES) {
    spawn_enemy(arena, 1, 25, mummy_animations);
    last_mummy_spawn = get_current_frame();
    enemy_count++;
  }
}

void (spawn_enemy)(arena* arena, double speed_multiplier, uint32_t health, animation* animations) {
  // Randomly choose which side of the screen to spawn from
  int side = rand() % 4;  // 0: top, 1: right, 2: bottom, 3: left
  
  double pos_x, pos_y;
  const int SPAWN_OFFSET = 10;  // Distance from arena edge
  
  // Position the enemy offscreen based on the chosen side
  switch (side) {
    case 0: // Top
      // 2528 -> Arena width | 54 -> Enemy width
      pos_x = rand() % (arena->background_image->width - 54);  // Ensure enemy fits within arena width
      pos_y = SPAWN_OFFSET;
      break;
    case 1: // Right
      // 2528 -> Arena width | 87 -> Enemy height
      pos_x = arena->background_image->width - SPAWN_OFFSET;
      pos_y = rand() % (arena->background_image->height - 87);  // Ensure enemy fits within arena height
      break;
    case 2: // Bottom
      // 2528 -> Arena width | 54 -> Enemy width
      pos_x = rand() % (arena->background_image->width - 54);  // Ensure enemy fits within arena width
      pos_y = arena->background_image->height - SPAWN_OFFSET;
      break;
    case 3: // Left
      // 2528 -> Arena width | 87 -> Enemy height
      pos_x = SPAWN_OFFSET;
      pos_y = rand() % (arena->background_image->height - 87);  // Ensure enemy fits within arena height
      break;
  }
  
  // Create the enemy entity with its animations
  entity* new_enemy = entity_create_full(pos_x, pos_y, 0, 0, speed_multiplier, health, animations);
  entity_list_add(&(arena->enemies), new_enemy);
}

bool (check_player_collision)(entity* enemy, entity* player) {
  // reduce the hitboxes by 5 pixels to make the collisions more accurate
  // Simple box collision detection | Using correct sprite dimensions
  const double ENEMY_WIDTH = enemy->animations->sprites[0]->width - 5;
  const double ENEMY_HEIGHT = enemy->animations->sprites[0]->height - 5;
  const double PLAYER_WIDTH = player->animations->sprites[0]->width - 5;
  const double PLAYER_HEIGHT = player->animations->sprites[0]->height - 5;

  // Check if the boxes overlap, needs to be improved
  return (enemy->pos_x + 5 < player->pos_x + PLAYER_WIDTH &&
          enemy->pos_x + ENEMY_WIDTH > player->pos_x + 5 &&
          enemy->pos_y + 5 < player->pos_y + PLAYER_HEIGHT &&
          enemy->pos_y + ENEMY_HEIGHT > player->pos_y + 5);
}


bool (check_attack_collision)(entity* enemy, attack* attack) {
  const double ENEMY_WIDTH = enemy->animations->sprites[0]->width;
  const double ENEMY_HEIGHT = enemy->animations->sprites[0]->height;
  const double ATTACK_WIDTH = attack->animation.sprites[0]->width + 5;
  const double ATTACK_HEIGHT = attack->animation.sprites[0]->height  + 5;

  return (enemy->pos_x < attack->pos_x + ATTACK_WIDTH &&
          enemy->pos_x + ENEMY_WIDTH > attack->pos_x - 5 &&
          enemy->pos_y < attack->pos_y + ATTACK_HEIGHT &&
          enemy->pos_y + ENEMY_HEIGHT > attack->pos_y - 5);
}


bool (handle_enemy_collisions)(arena* arena) {
  entity_node* current_enemy = arena->enemies;
  entity_node* previous_enemy = NULL;

  outer:
  while(current_enemy != NULL) {
    entity* enemy = current_enemy->entity;
    attack_node* current_attack = arena->player_attacks;

    //check collisions with the attacks of the player, remove the enemy if health = 0
    while(current_attack != NULL) {
      attack* attack = current_attack->attack;
      if(check_attack_collision(enemy, attack)) {
        if(enemy->health <= attack->damage) {
          // Remove the enemy
          entity_node* victim = current_enemy;
          current_enemy = current_enemy->next_entity;
          if(previous_enemy == NULL) 
            arena->enemies = current_enemy;
          else
            previous_enemy->next_entity = current_enemy;
          entity_node_destroy(victim);
          --enemy_count;
          goto outer;
        }
        else current_enemy->entity->health -= current_attack->attack->damage;
      }
      current_attack = current_attack->next_attack;
    }

    // Check for collision with player
    if (check_player_collision(enemy, arena->player)) {
      return true;  // Collision detected
    }

    previous_enemy = current_enemy;
    current_enemy = current_enemy->next_entity;
  }

  return false;  // No collision
}

void (enemies_move)(arena* arena) {
  entity_node* enemy_node = arena->enemies;
  while(enemy_node != NULL) {
    entity* enemy = enemy_node->entity;

    // Calculate direction to player
    enemy->speed_x = arena->player->pos_x - enemy->pos_x;
    enemy->speed_y = arena->player->pos_y - enemy->pos_y;

    enemy->last_horizontal_direction = enemy->speed_x > 0 ? true : false;

    // Move the enemy
    entity_move(enemy);

    enemy_node = enemy_node->next_entity;
  }
}
