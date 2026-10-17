/**
 * @file game.c
 * @brief Implementation of game state and entity management.
 *
 * This file contains the implementation of the functions declared in game.h.
 */
#include "game.h"

// --- Reloading and Ammo Globals ---
#define RELOAD_DURATION_SECONDS 1
int reload_timer_ticks = 0;

extern int enemies;
extern state_t state;
uint8_t secs = 0, mins = 0, hours = 0;

/**
 * @struct s_game
 * @brief Structure representing the main game state and entities.
 *
 * Contains all game entities and resources, including the player, enemies, bullets,
 * walls, health bar, timer, ammo display, and the current score.
 */
struct s_game {
  wall_array_t wall_arr;               /**< Array of walls */
  player_t *player;                    /**< Pointer to the player */
  bullet_t *bullets[MAX_BULLETS];      /**< Array of pointers to bullets  */
  enemy_t *enemy[MAX_ENEMIES];         /**< Array of pointers to enemies */
  sprite_t *ammo_display_sprite;       /**< Sprite for displaying ammo */
  sprite_t *h_bar;                     /**< Sprite for the health bar */
  draw_timer_t *ts;                    /**< Timer sprite for displaying time */
  unsigned int score;                  /**< Current game score */
};

game_t *create_game(unsigned v_res, unsigned h_res) {
  game_t *game = malloc(sizeof(game_t));
  if (!game) {
    return NULL;
  }

  //_______________ FILL GAME _____________________
  // Load level 1 walls
  game->wall_arr = load_walls_from_array(level1, LEVEL1_ROWS, 48, 48);

  /***********************/
  /******* Bullets *******/
  /***********************/
  for (int i = 0; i < MAX_BULLETS; ++i) {
    game->bullets[i] = NULL;
  }

  /***********************/
  /******* PLAYER ********/
  /***********************/
  game->player = player_init(h_res / 2, v_res / 2);
  if (game->player == NULL) {
    for (int i = 0; i < MAX_BULLETS; ++i) {
      destroy_bullet(game->bullets[i]);
    }

    for (int i = 0; i < game->wall_arr.count; ++i) {
      destroy_wall(game->wall_arr.walls[i]);
    }
    printf("\x1b[0;31mError creating player\x1b[0m\n");
    return NULL;
  }
  game->score = 0;
  /***********************/
  /******* ENEMIES *******/
  /***********************/
  for (int i = 0; i < MAX_ENEMIES; ++i) {
    game->enemy[i] = NULL;
  }

  if (create_enemies(game->enemy, INITIAL_ENEMIES,&(game->wall_arr),get_player_pos(game->player),v_res, h_res)) {
    destroy_player(game->player);
    for (int i = 0; i < MAX_BULLETS; ++i) {
      destroy_bullet(game->bullets[i]);
    }

    for (int i = 0; i < game->wall_arr.count; ++i) {
      destroy_wall(game->wall_arr.walls[i]);
    }
    printf("\x1b[0;31mError creating enemy\x1b[0m\n");
    return NULL;
  }

  /***********************/
  /***** HEALTH BAR ******/
  /***********************/
  xpm_map_t h_bar_arr[1] = {heart_xpm};
  game->h_bar = create_sprite(h_bar_arr, 1, 40, 60);
  if (game->h_bar == NULL) {
    destroy_player(game->player);
    destroy_enemies(game->enemy);
    for (int i = 0; i < MAX_BULLETS; ++i) {
      destroy_bullet(game->bullets[i]);
    }

    for (int i = 0; i < game->wall_arr.count; ++i) {
      destroy_wall(game->wall_arr.walls[i]);
    }
    printf("\x1b[0;31mError creating health bar sprite\x1b[0m\n");
    return NULL;
  }

  /***********************/
  /******** TIMER ********/
  /***********************/
  game->ts = create_timer(5, 5);
  if (game->ts == NULL) {
    destroy_player(game->player);
    destroy_sprite(game->h_bar);
    destroy_enemies(game->enemy);
    for (int i = 0; i < MAX_BULLETS; ++i) {
      destroy_bullet(game->bullets[i]);
    }

    for (int i = 0; i < game->wall_arr.count; ++i) {
      destroy_wall(game->wall_arr.walls[i]);
    }
    printf("\x1b[0;31mError creating timer\x1b[0m\n");
    return NULL;
  }

  // --- Create Ammo Display Sprite ---
  xpm_map_t ammo_xpm_map[1] = {ammo_xpm};
  game->ammo_display_sprite = create_sprite(ammo_xpm_map, 1, 0, 0);
  if (!game->ammo_display_sprite) {
    destroy_player(game->player);
    destroy_sprite(game->h_bar);
    destroy_timer(game->ts);
    destroy_enemies(game->enemy);
    for (int i = 0; i < MAX_BULLETS; ++i) {
      destroy_bullet(game->bullets[i]);
    }

    for (int i = 0; i < game->wall_arr.count; ++i) {
      destroy_wall(game->wall_arr.walls[i]);
    }
    printf("Error creating ammo display sprite\n");
    return NULL;
  }
  return game;
}

int timer_update_game(game_t *game, unsigned h_res, unsigned v_res, int counter) {
  // --- RELOAD TIMER LOGIC ---
  if (get_player_is_reloading(game->player)) {
    reload_timer_ticks++;
    if (reload_timer_ticks >= RELOAD_DURATION_SECONDS * 60) {
      set_player_current_ammo_in_magazine(game->player, MAX_AMMO_IN_MAGAZINE);
      set_player_is_reloading(game->player, false);
      reload_timer_ticks = 0;
    }
  }
  // --- END RELOAD TIMER LOGIC ---
  for (int i = 0; i < enemies; ++i) {
    if (game->enemy[i] != NULL && game->enemy[i]->active) {
      set_enemy_act(game->enemy[i], E_IDLE);
      enemy_t *current_enemy = game->enemy[i];
      current_enemy->shoot_cooldown_timer--;

      if (current_enemy->shoot_cooldown_timer <= 0) {
        position_t *player_sprite_pos = get_player_pos(game->player);
        int16_t player_target_x = player_sprite_pos->x + get_player_width(game->player) / 2;
        int16_t player_target_y = player_sprite_pos->y + get_player_height(game->player) / 2;

        // Enemy's position (center of sprite)
        position_t *enemy_sprite_pos = get_sprite_pos(get_enemy_sprite(current_enemy));
        uint16_t enemy_width = get_sprite_width(get_enemy_sprite(current_enemy));
        uint16_t enemy_height = get_sprite_height(get_enemy_sprite(current_enemy));
        int16_t enemy_shoot_x = enemy_sprite_pos->x + enemy_width / 2;
        int16_t enemy_shoot_y = enemy_sprite_pos->y + enemy_height / 2;

        // Find an empty slot for the new bullet
        int bullet_slot = -1;
        for (int k = 0; k < MAX_BULLETS; ++k) {
          if (game->bullets[k] == NULL || !game->bullets[k]->active) {
            if (game->bullets[k] != NULL) {
              destroy_bullet(game->bullets[k]);
              game->bullets[k] = NULL;
            }
            bullet_slot = k;
            break;
          }
        }

        if (bullet_slot != -1) {
          game->bullets[bullet_slot] = bullet_init(
            enemy_shoot_x, enemy_shoot_y,
            player_target_x, player_target_y,
            false);
          if (enemy_shoot_x < player_target_x) {
              set_enemy_flip_on_shoot(current_enemy, false); // Face right
          } else {
              set_enemy_flip_on_shoot(current_enemy, true);  // Face left
          }
          set_enemy_act(current_enemy, E_SHOOT);
          if (game->bullets[bullet_slot] == NULL) {
            printf("\x1b[0;31mFailed to create enemy bullet\x1b[0m\n");
          }
        }
        // Reset cooldown timer
        current_enemy->shoot_cooldown_timer = ENEMY_SHOOT_COOLDOWN_BASE_TICKS + (rand() % (ENEMY_SHOOT_COOLDOWN_RANDOM_TICKS * 2)) - ENEMY_SHOOT_COOLDOWN_RANDOM_TICKS;
        if (current_enemy->shoot_cooldown_timer < 30)
          current_enemy->shoot_cooldown_timer = 30; // Min cooldown
      }
    }
  }
  // --- ENEMY UPDATE LOGIC ---
  if (counter % 60 == 0) {
    move_enemies(game->enemy, h_res, v_res, &(game->wall_arr), game->player);
    secs++;
  }

  if (secs == 60) {
    if (enemies < MAX_ENEMIES) {
      if (create_enemies(game->enemy, ADD_ENEMIES,&(game->wall_arr),get_player_pos(game->player),v_res, h_res)) {
        printf("\x1b[0;31mError creating another enemy\x1b[0m\n");
        return 1;
      }
    }

    mins++;
    secs = 0;
  }
  if (mins >= 60) {
    hours++;
    mins = 0;
  }

  // --- BULLET UPDATE LOGIC ---
  for (int i = 0; i < MAX_BULLETS; ++i) {
    if (game->bullets[i] != NULL) { // Check for NULL before accessing active
      if (game->bullets[i]->active) {
        bullet_move(game->bullets[i]);
        bullet_check_bounds_deactivate(game->bullets[i], h_res, v_res);
      }
    }
  }
  // --- END BULLET UPDATE LOGIC ---

  // --- COLLISION DETECTION LOGIC ---
  for (int i = 0; i < MAX_BULLETS; ++i) {
    if (game->bullets[i] != NULL && game->bullets[i]->active) {
      bullet_t *current_bullet = game->bullets[i];
      sprite_t *bullet_sprite = current_bullet->sprite;
      if (collides_with_any_wall(bullet_sprite, &(game->wall_arr), NULL, NULL)) {
        current_bullet->active = false;
        continue;
      }

      if (current_bullet->is_from_player) {
        for (int j = 0; j < enemies; ++j) {
          if (game->enemy[j] != NULL && game->enemy[j]->active) {
            if (check_collision(bullet_sprite, get_enemy_sprite(game->enemy[j]))) {
              current_bullet->active = false;
              game->enemy[j]->active = false;
              game->score += 1;
              break;
            }
          }
        }
      }
      else {
        if (game->player != NULL) {
          sprite_t *player_sprite_for_collision = get_player_sprite(game->player);
          if (check_collision(bullet_sprite, player_sprite_for_collision)) {
            current_bullet->active = false;
            player_take_damage(game->player);
            printf("Player hit! Health: %d\n", get_player_health(game->player));
            if (get_player_health(game->player) <= 0) {
              state = GAME_OVER;
              secs = 0;
              mins = 0;
              hours = 0;
            }
          }
        }
      }
    }
  }
  return 0;
}

int draw_game(game_t *game) {
  if (draw_health_bar(get_player_health(game->player), game->h_bar)) {
    printf("\x1b[0;31mError drawing health bar\x1b[0m\n");
    return 1;
  }

  if (game->ammo_display_sprite) {
    position_t *ammo_sprite_pos = get_sprite_pos(game->ammo_display_sprite);

    sprite_t *health_sprite_ref = game->h_bar;
    position_t *health_bar_initial_pos = get_sprite_pos(health_sprite_ref);
    uint16_t health_sprite_h = get_sprite_height(health_sprite_ref);

    uint16_t ammo_w = get_sprite_width(game->ammo_display_sprite);
    uint16_t ammo_spacing = 2;

    ammo_sprite_pos->y = health_bar_initial_pos->y + health_sprite_h + 5;

    for (unsigned int i = 0; i < get_player_current_ammo_in_magazine(game->player); ++i) {
      ammo_sprite_pos->x = health_bar_initial_pos->x + (i * (ammo_w + ammo_spacing));
      if (draw_sprite(game->ammo_display_sprite)) {
        printf("\x1b[0;31mError drawing ammo[%d]\x1b[0m\n", i);
        return 1;
      }
    }
  }

  if (player_draw(game->player)) {
    printf("\x1b[0;31mError drawing player\x1b[0m\n");
    return 1;
  }

  if (draw_enemies(game->enemy)) {
    printf("\x1b[0;31mError drawing enemies\x1b[0m\n");
    return 1;
  }

  // Draw bullets
  for (int i = 0; i < MAX_BULLETS; ++i) {
    if (game->bullets[i] != NULL && game->bullets[i]->active) {
      if (bullet_draw(game->bullets[i])) {
        printf("\x1b[0;31mError drawing bullet[%d]\x1b[0m\n", i);
        return 1;
      }
    }
  }

  if (draw_timer(game->ts, hours, mins, secs, 40, 50)) {
    printf("\x1b[0;31mError drawing timer\x1b[0m\n");
    return 1;
  };

  for (int i = 0; i < game->wall_arr.count; ++i) {
    if (wall_draw(game->wall_arr.walls[i])) {
      printf("\x1b[0;31mError drawing wall[%d]\x1b[0m\n", i);
      return 1;
    }
  }
  return 0;
}

unsigned int get_game_score(game_t *game) {
  if (!game){
    printf("No game.\n");
    return 0;
  }
  return game->score;
}

enemy_t **get_game_enemies(game_t *game) {
  return game->enemy;
}

void timer_update_post_event_game(game_t *game) {
  // --- POST-COLLISION BULLET CLEANUP ---
  for (int i = 0; i < MAX_BULLETS; ++i) {
    if (game->bullets[i] != NULL && !game->bullets[i]->active) {
      destroy_bullet(game->bullets[i]);
      game->bullets[i] = NULL;
    }
  }
  // --- END POST-COLLISION BULLET CLEANUP ---
}

void game_idle(game_t *game) {
  set_player_idle(game->player);
}

void destroy_game(game_t *game) {
  destroy_player(game->player);
  destroy_sprite(game->h_bar);
  destroy_timer(game->ts);
  destroy_enemies(game->enemy);
  if (game->ammo_display_sprite) {
    destroy_sprite(game->ammo_display_sprite);
  }
  for (int i = 0; i < MAX_BULLETS; ++i) {
    destroy_bullet(game->bullets[i]);
  }

  for (int i = 0; i < game->wall_arr.count; ++i) {
    destroy_wall(game->wall_arr.walls[i]);
  }
  free(game->wall_arr.walls);
}

player_t *get_game_player(game_t *game) {
  return game->player;
}

wall_array_t get_game_wall_arr(game_t *game) {
  return game->wall_arr;
}

bullet_t **get_game_bullets(game_t *game) {
  return game->bullets;
}

bool check_collision(sprite_t *sp1, sprite_t *sp2) {
  if (!sp1 || !sp2)
    return false;

  position_t *pos1 = get_sprite_pos(sp1);
  uint16_t w1 = get_sprite_width(sp1);
  uint16_t h1 = get_sprite_height(sp1);

  position_t *pos2 = get_sprite_pos(sp2);
  uint16_t w2 = get_sprite_width(sp2);
  uint16_t h2 = get_sprite_height(sp2);

  // Check if one rectangle is on the left side of the other
  if (pos1->x + w1 < pos2->x || pos2->x + w2 < pos1->x) {
    return false;
  }
  // Check if one rectangle is above the other
  if (pos1->y + h1 < pos2->y || pos2->y + h2 < pos1->y) {
    return false;
  }
  // If none of the above, they are colliding
  return true;
}
