/**
 * @file player.c
 * @brief Implementation of player entity management.
 *
 * This file contains the implementation of the functions declared in player.h.
 */
#include "player.h"

unsigned int delay = 0;

/**
 * @struct player
 * @brief Structure representing the player entity.
 *
 * Contains the player's current action, animation sprites (walk, idle, shoot),
 * reloading state, current ammo, and health.
 */
struct player {
  action act;                           /**< Current action/state of the player */
  sprite_t *walk;                       /**< Sprite for walking animation */
  sprite_t *idle;                       /**< Sprite for idle animation */
  sprite_t *shoot;                      /**< Sprite for shooting animation */
  bool is_reloading;                    /**< Whether the player is currently reloading */
  unsigned int current_ammo_in_magazine;/**< Current ammo in the magazine */
  unsigned int health;                  /**< Player's health */
};

player_t *player_init(int16_t start_x, int16_t start_y) {
  // Create Walk
  xpm_map_t walk_xpm[10] = {walk1, walk2, walk3, walk4, walk5, walk6, walk7, walk8, walk9, walk10};

  sprite_t *walk = create_sprite(walk_xpm, 10, start_x, start_y);
  if (walk == NULL) { /* Error Creating Sprite */
    return NULL;
  }

  // Create Idle
  xpm_map_t idle_xpm[7] = {idle1, idle2, idle3, idle4, idle5, idle6, idle7};

  sprite_t *idle = create_sprite(idle_xpm, 7, start_x, start_y);
  if (idle == NULL) { /* Error Creating Sprite */
    free(walk);
    return NULL;
  }

  // Create Shoot
  xpm_map_t shoot_xpm[12] = {shoot1, shoot2, shoot3, shoot4, shoot5, shoot6, shoot7, shoot8, shoot9, shoot10, shoot11, shoot12};

  sprite_t *shoot = create_sprite(shoot_xpm, 12, start_x, start_y);
  if (idle == NULL) { /* Error Creating Sprite */
    free(walk);
    free(idle);
    return NULL;
  }

  // Create Player
  player_t *player = (player_t *) malloc(sizeof(player_t));
  if (player == NULL) { /* Error Creating player */
    free(walk);
    free(idle);
    free(shoot);
    return NULL;
  }

  player->walk = walk;
  player->idle = idle;
  player->shoot = shoot;
  player->health = 3;
  player->act = A_IDLE;
  player->is_reloading = false;
  player->current_ammo_in_magazine = MAX_AMMO_IN_MAGAZINE;

  return player;
}

void destroy_player(player_t *player) {
  if (player) {
    destroy_sprite(player->walk);
    destroy_sprite(player->idle);
    destroy_sprite(player->shoot);
    free(player);
  }
  player = NULL;
}

void player_take_damage(player_t *player) {
    if (player) {
        player->health -= 1;
    }
}

void player_move(player_t *player, int16_t dx, int16_t dy) {
  position_t *pos = get_player_pos(player);

  uint16_t x = pos->x, y = pos->y;
  set_player_pos(player, x + dx, y + dy);

  if (dx > 0 || dy > 0) {
    set_sprite_flip(player->idle, false);
    set_sprite_flip(player->walk, false);
  }
  else if (dx < 0 || dy < 0) {
    set_sprite_flip(player->idle, true);
    set_sprite_flip(player->walk, true);
  }

  set_player_act(player, A_WALK);
}

int player_draw(player_t *player) {
  delay++;
  if (player->act == A_IDLE) {
    return draw_player_sprite(player->idle);
  }
  else if (player->act == A_SHOOT) {
    unsigned int curr = get_sprite_curr(player->shoot);
    unsigned int size = get_sprite_size(player->shoot);
    if (size == curr) {
      player->act = A_IDLE;
      if (draw_sprite(player->idle)) {
        return 1;
      }
      set_sprite_curr(player->shoot, 0);
      delay++;
      return 0;
    }
    else {
      return draw_player_sprite(player->shoot);
    }
  }
  else if (player->act == A_WALK) {
    return draw_player_sprite(player->walk);
  }
  return 1;
}

int draw_player_sprite(sprite_t *sp) {
  if (delay >= 2) {
    if (draw_sprite_animate(sp)) {
      return 1;
    }
    delay = 0;
    return 0;
  }
  else {
    return draw_sprite(sp);
  }
}

// Getters
uint16_t get_player_width(player_t *player) {
  if (player->act == A_IDLE) {
    return get_sprite_width(player->idle);
  }
  else if (player->act == A_SHOOT) {
    return get_sprite_width(player->shoot);
  }
  else {
    return get_sprite_width(player->walk);
  }
}

uint16_t get_player_height(player_t *player) {
  if (player->act == A_IDLE) {
    return get_sprite_height(player->idle);
  }
  else if (player->act == A_SHOOT) {
    return get_sprite_height(player->shoot);
  }
  else {
    return get_sprite_height(player->walk);
  }
}

position_t *get_player_pos(player_t *player) {
  if (player->act == A_IDLE) {
    return get_sprite_pos(player->idle);
  }
  else if (player->act == A_SHOOT) {
    return get_sprite_pos(player->shoot);
  }
  else {
    return get_sprite_pos(player->walk);
  }
}

unsigned int get_player_health(player_t *player) {
  return player->health;
}

// Function to keep player within screen boundaries
void player_check_bounds(player_t *player, unsigned int screen_width, unsigned int screen_height) {
  uint16_t width = get_player_width(player);
  uint16_t height = get_player_height(player);
  position_t *pos = get_player_pos(player);

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

sprite_t *get_player_sprite(player_t *player) {
  if (player->act == A_IDLE) {
    return player->idle;
  }
  else if (player->act == A_SHOOT) {
    return player->shoot;
  }
  else {
    return player->walk;
  }
}

bool get_player_is_reloading(player_t *player) {
  return player->is_reloading;
}

void set_player_is_reloading(player_t *player, bool is_reloading) {
  player->is_reloading = is_reloading;
}

unsigned int get_player_current_ammo_in_magazine(player_t *player) {
  return player->current_ammo_in_magazine;
}

void set_player_current_ammo_in_magazine(player_t *player, unsigned int ammo) {
  player->current_ammo_in_magazine = ammo;
}

void set_player_act(player_t *player, action act) {
  player->act = act;
}

void set_player_flip_on_shoot(player_t *player, bool flip) {
  set_sprite_flip(player->idle, flip);
  set_sprite_flip(player->shoot, flip);
}

void set_player_idle(player_t *player) {
  if (player->act == A_SHOOT) {
    unsigned int curr = get_sprite_curr(player->shoot);
    unsigned int size = get_sprite_size(player->shoot);
    if (size == curr) {
      player->act = A_IDLE;
      set_sprite_curr(player->shoot, 0);
    }
  }
  else if (player->act == A_WALK) {
    unsigned int curr = get_sprite_curr(player->walk);
    unsigned int size = get_sprite_size(player->walk);
    if (size == curr) {
      player->act = A_IDLE;
      set_sprite_curr(player->walk, 0);
    }
  }
}

void set_player_pos(player_t *player, uint16_t x, uint16_t y) {
  position_t *pw = get_sprite_pos(player->walk);
  pw->x = x;
  pw->y = y;

  position_t *ps = get_sprite_pos(player->shoot);
  ps->x = x;
  ps->y = y;

  position_t *pi = get_sprite_pos(player->idle);
  pi->x = x;
  pi->y = y;
}
