/**
 * @file enemy.c
 * @brief Implementation of enemy entity management.
 *
 * This file contains the implementation of the functions declared in enemy.h.
 */
#include "enemy.h"
#include <math.h>

unsigned int e_delay = 0;

int enemies = 0;

/* Bounds the recursive retry below so a surrounded enemy cannot overflow the stack. */
static int enemy_move_depth = 0;

enemy_t *enemy_init(int16_t start_x, int16_t start_y) {
  xpm_map_t walk_xpm[10] = {e_walk1, e_walk2, e_walk3, e_walk4, e_walk5, e_walk6, e_walk7, e_walk8, e_walk9, e_walk10};

  sprite_t *walk = create_sprite(walk_xpm, 10, start_x, start_y);
  if (walk == NULL) { /* Error Creating Sprite */
    return NULL;
  }

  // Create Idle
  xpm_map_t idle_xpm[6] = {e_idle1, e_idle2, e_idle3, e_idle4, e_idle5, e_idle6};
  sprite_t *idle = create_sprite(idle_xpm, 6, start_x, start_y);
  if (idle == NULL) {
    free(walk);
    return NULL;
  }

  xpm_map_t shoot_xpm[4] = {e_shoot1, e_shoot2, e_shoot3, e_shoot4};
  sprite_t *shoot = create_sprite(shoot_xpm, 4, start_x, start_y);
  if (idle == NULL) {
    free(walk);
    free(idle);
    return NULL;
  }

  enemy_t *enemy = (enemy_t *) malloc(sizeof(enemy_t));
  if (enemy == NULL) { /* Error Creating enemy */
    free(walk);
    free(idle);
    free(shoot);
    return NULL;
  }

  enemy->active = true;
  enemy->walk = walk;
  enemy->idle = idle;
  enemy->shoot = shoot;
  enemy->act = E_IDLE;  // Start in idle state
  enemy->direction = 0; // Start moving left

  enemy->shoot_cooldown_timer = ENEMY_SHOOT_COOLDOWN_BASE_TICKS + (rand() % (ENEMY_SHOOT_COOLDOWN_RANDOM_TICKS * 2)) - ENEMY_SHOOT_COOLDOWN_RANDOM_TICKS;
  if (enemy->shoot_cooldown_timer < 30)
    enemy->shoot_cooldown_timer = 30;
  return enemy;
}

void destroy_enemy(enemy_t *enemy) {
  if (enemy) {
    destroy_sprite(enemy->walk);
    destroy_sprite(enemy->idle);
    destroy_sprite(enemy->shoot);
    free(enemy);
  }
}

void enemy_move(enemy_t *enemy, wall_array_t *wall_arr, player_t *player, enemy_t **enemy_arr) {
  if (enemy->act == E_SHOOT && get_sprite_curr(enemy->shoot) < get_sprite_size(enemy->shoot)) {
    return;
  }
  set_enemy_act(enemy, E_WALK);
  int rd_x = 0, rd_y = 0;
  switch (enemy->direction) {
    case 0:
      rd_x = -1 * ENEMY_SPEED;
      rd_y = 0;
      break;
    case 1:
      rd_x = 0;
      rd_y = ENEMY_SPEED;
      break;
    case 2:
      rd_x = ENEMY_SPEED;
      rd_y = 0;
      break;
    case 3:
      rd_x = 0;
      rd_y = -1 * ENEMY_SPEED;
      break;
    default:
      break;
  }
  position_t *pos = get_sprite_pos(get_enemy_sprite(enemy));
  int prev_x = pos->x;
  int prev_y = pos->y;

  if (rd_x > 0 || rd_y > 0) {
    set_sprite_flip(enemy->idle, false);
    set_sprite_flip(enemy->walk, false);
  }
  else if (rd_x < 0 || rd_y < 0) {
    set_sprite_flip(enemy->idle, true);
    set_sprite_flip(enemy->walk, true);
  }

  pos->x += (int16_t) rd_x;
  pos->y += (int16_t) rd_y;

  set_enemy_pos(enemy, pos->x, pos->y);

  int dx = 0, dy = 0;
  if (collides_with_any_wall(get_enemy_sprite(enemy), wall_arr, &dx, &dy)) {
    set_enemy_pos(enemy, prev_x, prev_y);
    enemy->direction = enemy->direction + 1;
    if (enemy->direction > 3)
      enemy->direction = 0;
    if (enemy_move_depth < 4) {
      enemy_move_depth++;
      enemy_move(enemy, wall_arr, player, enemy_arr);
      enemy_move_depth--;
    }
  }



  sprite_t *sp1 = get_enemy_sprite(enemy);
  sprite_t *sp2 = get_player_sprite(player);

  position_t *pos1 = get_sprite_pos(sp1);
  uint16_t w1 = get_sprite_width(sp1);
  uint16_t h1 = get_sprite_height(sp1);

  position_t *pos2 = get_sprite_pos(sp2);
  uint16_t w2 = get_sprite_width(sp2);
  uint16_t h2 = get_sprite_height(sp2);

  // Check collision with player (igual ao check_collision)
  if (!(pos1->x + w1 <= pos2->x || pos2->x + w2 <= pos1->x ||
      pos1->y + h1 <= pos2->y || pos2->y + h2 <= pos1->y)){
    set_enemy_pos(enemy, prev_x, prev_y);
    enemy->direction = enemy->direction + 1;
    if (enemy->direction > 3)
      enemy->direction = 0;
    // tentar mover outra vez estava a fazer o jogo crashar
    //enemy_move(enemy, wall_arr, player);
  }

  for (int i = 0; i < enemies; ++i) {
      if (enemy_arr[i] && enemy_arr[i]->active && enemy_arr[i] != enemy) {

        sprite_t *sp3 = get_enemy_sprite(enemy_arr[i]);

        position_t *pos3 = get_sprite_pos(sp3);
        uint16_t w3 = get_sprite_width(sp3);
        uint16_t h3 = get_sprite_height(sp3);


        if (!(pos1->x + w1 <= pos3->x || pos3->x + w3 <= pos1->x ||
            pos1->y + h1 <= pos3->y || pos3->y + h3 <= pos1->y)) {
          set_enemy_pos(enemy, prev_x, prev_y);
          enemy->direction = enemy->direction + 1;
          if (enemy->direction > 3)
            enemy->direction = 0;
          break;
        }
      }
  }
}

int enemy_draw(enemy_t *enemy) {
  e_delay++;
  if (enemy->act == E_IDLE) {
    return draw_enemy_sprite(enemy->idle);
  }
  else if (enemy->act == E_SHOOT) {
    unsigned int curr = get_sprite_curr(enemy->shoot);
    unsigned int size = get_sprite_size(enemy->shoot);
    if (size == curr) {
      enemy->act = E_IDLE;
      if (draw_sprite(enemy->idle)) {
        return 1;
      }
      set_sprite_curr(enemy->shoot, 0);
      e_delay++;
      return 0;
    }
    else {
      return draw_enemy_sprite(enemy->shoot);
    }
  }
  else if (enemy->act == E_WALK) {
    return draw_enemy_sprite(enemy->walk);
  }
  return 1;
}

int draw_enemy_sprite(sprite_t *sp) {
  if (e_delay >= 2) {
    if (draw_sprite_animate(sp)) {
      return 1;
    }
    e_delay = 0;
    return 0;
  }
  else {
    return draw_sprite(sp);
  }
}

// Getters
uint16_t get_enemy_width(enemy_t *enemy) {
  if (enemy->act == E_IDLE) {
    return get_sprite_width(enemy->idle);
  }
  else if (enemy->act == E_SHOOT) {
    return get_sprite_width(enemy->shoot);
  }
  else {
    return get_sprite_width(enemy->walk);
  }
}

uint16_t get_enemy_height(enemy_t *enemy) {
  if (enemy->act == E_IDLE) {
    return get_sprite_height(enemy->idle);
  }
  else if (enemy->act == E_SHOOT) {
    return get_sprite_height(enemy->shoot);
  }
  else {
    return get_sprite_height(enemy->walk);
  }
}

position_t *get_enemy_pos(enemy_t *enemy) {
  if (enemy->act == E_IDLE) {
    return get_sprite_pos(enemy->idle);
  }
  else if (enemy->act == E_SHOOT) {
    return get_sprite_pos(enemy->shoot);
  }
  else {
    return get_sprite_pos(enemy->walk);
  }
}

// Function to keep enemy within screen boundaries
void enemy_check_bounds(enemy_t *enemy, unsigned int screen_width, unsigned int screen_height) {
  uint16_t width = get_enemy_width(enemy);
  uint16_t height = get_enemy_height(enemy);
  position_t *pos = get_enemy_pos(enemy);

  if (pos->x < 0) {
    pos->x = 0;
  }
  if (pos->y < 0) {
    pos->y = 0;
  }
  if (pos->x + width > (int16_t) screen_width) {
    pos->x = screen_width - width;
  }
  if (pos->y + height > (int16_t) screen_height) {
    pos->y = screen_height - height;
  }
}

int create_enemies(enemy_t **enemy_arr, int num, wall_array_t *wall_arr, position_t *player_pos, unsigned vres, unsigned hres) {
  enemy_t *temp_enemy = enemy_init(0, 0);
  if (!temp_enemy)
    return 1;

  int enemy_width = get_enemy_width(temp_enemy);
  int enemy_height = get_enemy_height(temp_enemy);
  destroy_enemy(temp_enemy);

  int attempts = 0;
  int created = 0;

  while (created < num && enemies < MAX_ENEMIES && attempts < 1000) {
    attempts++;

    int x = rand() % (hres - enemy_width);
    int y = rand() % (vres - enemy_height);

    enemy_t *temp = enemy_init(x, y);
    if (!temp)
      continue;

    int dx = 0, dy = 0;
    bool wall_collision = collides_with_any_wall(get_enemy_sprite(temp), wall_arr, &dx, &dy);
    if (wall_collision) {
      destroy_enemy(temp);
      continue;
    }

    // Distance from the player
    float dist = sqrtf((x - player_pos->x) * (x - player_pos->x) + (y - player_pos->y) * (y - player_pos->y));
    if (dist < SPAWN_DISTANCE) {
      destroy_enemy(temp);
      continue;
    }

    // Shouldn't be drawn on top of another enemy
    bool enemy_collision = false;
    for (int i = 0; i < enemies; i++) {

      if (enemy_arr[i] == NULL)
        continue;

      position_t *pos_i = get_enemy_pos(enemy_arr[i]);
      int w = get_enemy_width(enemy_arr[i]);
      int h = get_enemy_height(enemy_arr[i]);

      if (!(x + enemy_width <= pos_i->x || pos_i->x + w <= x ||
            y + enemy_height <= pos_i->y || pos_i->y + h <= y)) {
        enemy_collision = true;
        break;
      }
    }
    if (enemy_collision) {
      destroy_enemy(temp);
      continue;
    }

    enemy_arr[enemies] = temp;

    if (enemy_arr[enemies] == NULL) {
      for (int j = 0; j < enemies; j++) destroy_enemy(enemy_arr[j]);
      return 1;
    }

    else if (enemy_arr[enemies] != NULL) {
      enemies++;
      created++;
    }
  }

  return 0;
}

void destroy_enemies(enemy_t **enemy_arr) {
  for (int j = 0; j < enemies; j++) {
    if (enemy_arr[j] != NULL) {
      destroy_enemy(enemy_arr[j]);
      enemy_arr[j] = NULL;
    }
  }
  enemies = 0;
}

sprite_t *get_enemy_sprite(enemy_t *enemy) {
  if (enemy->act == E_IDLE) {
    return enemy->idle;
  }
  else if (enemy->act == E_SHOOT) {
    return enemy->shoot;
  }
  else {
    return enemy->walk;
  }
}

int draw_enemies(enemy_t **enemy_arr) {
  for (int j = 0; j < enemies; j++) {
    if (enemy_arr[j] != NULL && enemy_arr[j]->active) {
      if (enemy_draw(enemy_arr[j]))
        return 1;
    }
  }
  return 0;
}

void move_enemies(enemy_t **enemy_arr, unsigned int screen_width, unsigned int screen_height, wall_array_t *wall_arr, player_t *player) {
  for (int j = 0; j < enemies; j++) {
    if (enemy_arr[j] != NULL && enemy_arr[j]->active) {
      struct enemy *enemy = enemy_arr[j];
      enemy_move(enemy, wall_arr, player, enemy_arr);
    }
  }
}

void set_enemy_act(enemy_t *enemy, enemy_action act) {
  enemy->act = act;
}

void set_enemy_flip_on_shoot(enemy_t *enemy, bool flip) {
  set_sprite_flip(enemy->idle, flip);
  set_sprite_flip(enemy->shoot, flip);
}

void set_enemy_idle(enemy_t *enemy) {
  if (enemy->act == E_SHOOT) {
    unsigned int curr = get_sprite_curr(enemy->shoot);
    unsigned int size = get_sprite_size(enemy->shoot);
    if (size == curr) {
      enemy->act = E_IDLE;
      set_sprite_curr(enemy->shoot, 0);
    }
  }
  else if (enemy->act == E_WALK) {
    unsigned int curr = get_sprite_curr(enemy->walk);
    unsigned int size = get_sprite_size(enemy->walk);
    if (size == curr) {
      enemy->act = E_IDLE;
      set_sprite_curr(enemy->walk, 0);
    }
  }
}

void set_enemy_pos(enemy_t *enemy, uint16_t x, uint16_t y) {
  position_t *pw = get_sprite_pos(enemy->walk);
  pw->x = x;
  pw->y = y;

  position_t *ps = get_sprite_pos(enemy->shoot);
  ps->x = x;
  ps->y = y;

  position_t *pi = get_sprite_pos(enemy->idle);
  pi->x = x;
  pi->y = y;
}
