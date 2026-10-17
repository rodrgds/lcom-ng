/** 
 * @file arena_controller.c
 * @brief Source code to control the arena
 * 
 * This file handles the logic behind the operation of the arena, including the player and the enemies using the functions in @ref enemy_controller.c.
 */


#include "arena_controller.h"
#include <stdio.h>


/**
 * @brief Spawns a heart to regenerate the player with a cooldown.
 * 
 * If the cooldown has passed, it spawns a heart in a random position inside the arena.
 * 
 * @param arena A pointer to the arena where the heart will be spawned
 */
static void (handle_heart_spawning)(arena* arena);

/**
 * @brief Spawns a secret entity once
 * 
 * Secret shhhhhhhh
 * 
 * @param arena A pointer to the arena where the secret will be spawned
 */
static void (handle_secret_spawning)(arena* arena);

/**
 * @brief Checks if the player is colliding with the heart.
 * 
 * Uses a simple box collision detection utilizing the image sizes of the sprites.
 * 
 * @param heart A pointer to the heart entity to check
 * @param player A pointer to the player entity to check
 * 
 * @return true if the entities are colliding, false otherwise
 */
static bool (check_heart_collision)(entity* heart, entity* player);

/**
 * @brief Checks the collisions of the hearts with the player and regenerates health
 * 
 * Iterates through the list of active hearts and checks if the player is colliding with each of them by calling check_heart_collision().
 * If the player is colliding with a heart and does not have full health, a health point is added to the player and the heart is removed.
 * 
 * @param arena A pointer to the arena where the collisions will be checked
 */
static void (handle_heart_collisions)(arena* arena);

/**
 * @brief Moves all the active attacks of the arena one frame
 * 
 * Iterates through the list of active attacks and calls attack_move() on each of them to advance one frame on the animation, unless the attack has expired, in which case it is removed.
 * 
 * @param arena A pointer to the arena whose attacks will be processed
 */
static void (attacks_move)(arena* arena);

/**
 * @brief Moves all the active entities of the arena one frame
 * 
 * Calls enemies_move() to move all the enemies and attacks_move() to move all the active attacks.
 * It also moves the player and the arena accordingly, limiting the arena coordinates.
 * 
 * @param arena A pointer to the arena whose entities will be moved.
 */
static void (arena_move_entities)(arena* arena);


static uint32_t arena_game_time = 0;         ///< Time the arena was active in seconds
static uint32_t arena_starting_frame = 0;    ///< Track the frame when the arena was created
static unsigned long last_damage_time = 0;   ///< Track when we last damaged the player
static unsigned long last_auto_attack = 0;   ///< Track when the player last auto attacked
static unsigned long last_ranged_attack = 0; ///< Track when the player last used a ranged attack
static unsigned long last_heart_spawn = 0;   ///< Track when we last spawned a heart
static bool secret_spawned = false;          ///< Track if the secret enemy has been spawned


uint32_t (get_arena_starting_frame)() {
  return arena_starting_frame;
}

uint32_t (get_arena_game_time)() {
  return arena_game_time;
}

void (setup_arena_controller)() {
  last_auto_attack = 0;
  last_ranged_attack = 0;
  arena_starting_frame = get_current_frame();
  arena_game_time = 0;
  last_damage_time = 0;
  last_heart_spawn = 0;
  secret_spawned = false;
}

void (handle_ranged_attack)(arena* arena) {
  // Cooldown of 5 seconds (300/60fps = 5)
  if (get_current_frame() - last_ranged_attack >= 300) {
    attack* new_attack = attack_create_full(
      arena->pos_x + arena->mouse->pos_x - lightning_attack_animation.sprites[0]->width/2, 
      arena->pos_y + arena->mouse->pos_y - lightning_attack_animation.sprites[0]->height/2, 
      0, 0, 1, 56, 
      lightning_attack_animation
    );

    attack_list_add(&(arena->player_attacks), new_attack);

    last_ranged_attack = get_current_frame();
  }
}

void (handle_auto_attack)(arena* arena) {
  if (get_current_frame() - last_auto_attack >= 120) {
    // if the player is moving right or last moved right, attack on the right, else left
    double attack_x = arena->player->speed_x > 0 || arena->player->last_horizontal_direction ? 
    arena->player->pos_x + arena->player->animations->sprites[0]->width + 5 :
    arena->player->pos_x - khopesh_attack_right_animation.sprites[0]->width - 5;

    double attack_y = arena->player->pos_y + arena->player->animations->sprites[0]->height/2 -  khopesh_attack_right_animation.sprites[0]->height/2;

    animation animation = arena->player->speed_x > 0 || arena->player->last_horizontal_direction ? 
    khopesh_attack_right_animation : 
    khopesh_attack_left_animation;

    attack* new_attack = attack_create_full(attack_x, attack_y, 0, 0, 1, 24, animation);

    attack_list_add(&(arena->player_attacks), new_attack);

    last_auto_attack = get_current_frame();
  }
}

static void (handle_heart_spawning)(arena* arena) {
  // Spawn a heart every 3600 frames (1 minute at 60 FPS)
  if (get_current_frame() - last_heart_spawn >= 3600) {
    // Randomly choose a position within the arena bounds
    double pos_x = rand() % (arena->background_image->width - 40);  // 40 is the heart's width
    double pos_y = rand() % (arena->background_image->height - 40); // 40 is the heart's height

    
    // Create the heart entity
    entity* new_heart = entity_create_full(pos_x, pos_y, 0, 0, 0, 1, &heart_animation);
    entity_list_add(&(arena->hearts), new_heart);
    
    last_heart_spawn = get_current_frame();
  }
}

static void (handle_secret_spawning)(arena* arena) {
  // Spawn the secret enemy at 3 minutes (180 seconds) if not already spawned
  if (!secret_spawned && arena_game_time >= 180) {
    // Choose one corner of the arena at random
    int corner = rand() % 4;  // 0: top-left, 1: top-right, 2: bottom-left, 3: bottom-right
    double pos_x, pos_y;
    
    switch(corner) {
      case 0: // top-left
        pos_x = 50;
        pos_y = 50;
        break;
      case 1: // top-right
        pos_x = arena->background_image->width - 50;
        pos_y = 50;
        break;
      case 2: // bottom-left
        pos_x = 50;
        pos_y = arena->background_image->height - 50;
        break;
      case 3: // bottom-right
        pos_x = arena->background_image->width - 50;
        pos_y = arena->background_image->height - 50;
        break;
    }
    
    // Create the secret entity (with 0 speed so it doesn't move)
    entity* secret = entity_create_full(pos_x, pos_y, 0, 0, 0, 0xFFFFFFFF, secret_animations);
    entity_list_add(&(arena->enemies), secret);
  
    secret_spawned = true;
  }
}

static bool (check_heart_collision)(entity* heart, entity* player) {
  // Simple box collision detection
  return (heart->pos_x < player->pos_x + player->animations[0].sprites[0]->width &&
          heart->pos_x + heart->animations[0].sprites[0]->width > player->pos_x &&
          heart->pos_y < player->pos_y + player->animations[0].sprites[0]->height &&
          heart->pos_y + heart->animations[0].sprites[0]->height > player->pos_y);
}

static void (handle_heart_collisions)(arena* arena) {
  entity_node* current_heart = arena->hearts;
  entity_node* previous_heart = NULL;

  while(current_heart != NULL) {
    entity* heart = current_heart->entity;
    
    // Check for collision with player
    if (check_heart_collision(heart, arena->player)) {
      // Increase player health if not at max
      if (arena->player->health < 4) {
        arena->player->health++;
      }
      
      // Remove the heart
      entity_node* victim = current_heart;
      current_heart = current_heart->next_entity;
      if (previous_heart == NULL) 
        arena->hearts = current_heart;
      else 
        previous_heart->next_entity = current_heart;
      entity_node_destroy(victim);
      continue;
    }
    
    previous_heart = current_heart;
    current_heart = current_heart->next_entity;
  }
}

static void (attacks_move)(arena* arena) {
  attack_node* current_attack = arena->player_attacks;
  attack_node* previous_attack = NULL;

  while(current_attack != NULL) {
    attack* attack = current_attack->attack;
    if(attack->frames == 0) {
      attack_node* victim = current_attack;
      current_attack = current_attack->next_attack;
      if(previous_attack == NULL)
        arena->player_attacks = current_attack;
      else
        previous_attack->next_attack = current_attack;
      attack_node_destroy(victim);
      continue;
    }
    else
      attack_move(attack);

    previous_attack = current_attack;
    current_attack = current_attack->next_attack;
  }
}

static void (arena_move_entities)(arena* arena) {
  enemies_move(arena); //move all the enemies
  entity_move(arena->player); //move the player
  attacks_move(arena); //move all the attacks


  arena->pos_x = arena->player->pos_x - vg_get_hres()/2 + arena->player->animations[0].sprites[0]->width/2;
  arena->pos_y = arena->player->pos_y - vg_get_vres()/2 + arena->player->animations[0].sprites[0]->height/2;
  
  arena->pos_x = MIN(MAX(0, arena->pos_x), game_background_img.width - vg_get_hres());
  arena->pos_y = MIN(MAX(0, arena->pos_y), game_background_img.height - vg_get_vres());
}

int (arena_process_frame)(arena* arena) {
  // If handle_enemy_collisions() returns true, the player has collided with an enemy
  // Add a damage cooldown of 60 frames (1 second at 60 FPS)
  if (handle_enemy_collisions(arena) && (get_current_frame() - last_damage_time >= 60)) {
    arena->player->health--;
    if (arena->player->health > 0)
      last_damage_time = get_current_frame();
    else
      return 1;
  }
  // handle heart collisions
  handle_heart_collisions(arena);

  // spawn the enemies and hearts
  handle_enemy_spawning(arena);
  handle_heart_spawning(arena);
  handle_secret_spawning(arena);
  handle_auto_attack(arena);
  
  // move everything
  arena_move_entities(arena);

  // update the active time
  if ((get_current_frame() - arena_starting_frame) % 60 == 0)
    ++arena_game_time;
    
  return 0;
}
