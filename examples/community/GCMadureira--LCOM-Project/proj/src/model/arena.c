/** 
 * @file arena.c
 * @brief Source code file for all the logic related to the arena struct
 * 
 * This file implements all the functions for the arena struct.
 */


#include "arena.h"


arena* (arena_create)() {
  arena* new_arena = (arena*)malloc(sizeof(arena));

  new_arena->pos_x = 1264 - 576;
  new_arena->pos_y = 1264 - 432;
  new_arena->player = entity_create_full(1264 - 43, 1264 - 64, 0, 0, 3, 4, pharaoh_animations);
  new_arena->mouse = entity_create(NULL);
  new_arena->mouse->pos_x = vg_get_hres()/2;
  new_arena->mouse->pos_y = vg_get_vres()/2;
  new_arena->enemies = NULL;
  new_arena->player_attacks = NULL;
  new_arena->hearts = NULL;
  new_arena->background_image = &game_background_img;

  return new_arena;
}

int (arena_destroy)(arena* arena) {
  entity_destroy(arena->player);
  entity_list_destroy(arena->enemies);
  attack_list_destroy(arena->player_attacks);
  entity_list_destroy(arena->hearts);
  free(arena);
  return 0;
}
