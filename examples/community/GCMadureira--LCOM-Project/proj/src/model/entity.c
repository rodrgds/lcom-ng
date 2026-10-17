/** 
 * @file entity.c
 * @brief Source code file for all the logic related to the entity and entity_node structs
 * 
 * This file implements all the functions for the entity and entity_node structs.
 */


#include "entity.h"


bool (entity_is_idle)(entity* entity) {
  return entity->speed_x == 0 && entity->speed_y == 0;
}

entity* (entity_create)(animation* animations) {
  entity* new_entity = (entity*)malloc(sizeof(entity));

  new_entity->pos_x = 0;
  new_entity->pos_y = 0;
  new_entity->speed_x = 0;
  new_entity->speed_y = 0;
  new_entity->health = 0;
  new_entity->last_horizontal_direction = true;
  new_entity->animations = animations;

  return new_entity;
}

entity* (entity_create_full)(double pos_x, double pos_y, double speed_x, double speed_y, double speed_multiplier, uint32_t health, animation* animations) {
  entity* new_entity = (entity*)malloc(sizeof(entity));

  new_entity->pos_x = pos_x;
  new_entity->pos_y = pos_y;
  new_entity->speed_x = speed_x;
  new_entity->speed_y = speed_y;
  new_entity->speed_multiplier = speed_multiplier;
  new_entity->last_horizontal_direction = false;
  new_entity->health = health;
  new_entity->animations = animations;

  return new_entity;
}

int (entity_destroy)(entity* entity) {
  free(entity);
  return 0;
}

void (entity_move)(entity* entity){
  // dividing by the lenght normalizes the vector, then multiply it by the speed
  double lenght = sqrt(entity->speed_x * entity->speed_x + entity->speed_y * entity->speed_y); 
  if(lenght == 0) return;
  double scale = entity->speed_multiplier/lenght;

  entity->pos_x += scale * (entity->speed_x);
  entity->pos_y += scale * (entity->speed_y);

  // bound the position inside the background image size (arena size)
  entity->pos_x = MIN(MAX(0, entity->pos_x), game_background_img.width - entity->animations[0].sprites[0]->width);
  entity->pos_y = MIN(MAX(0, entity->pos_y), game_background_img.height - entity->animations[0].sprites[0]->height);
}

void (entity_node_destroy)(entity_node* node) {
  free(node->entity);
  free(node);
}

void (entity_list_add)(entity_node** list, entity* entity) {
  entity_node* new_node = (entity_node*)malloc(sizeof(entity_node));
  new_node->entity = entity;
  new_node->next_entity = *list;
  *list = new_node;
}

void (entity_list_destroy)(entity_node* list){
  entity_node* current = list;
  while(current != NULL) {
    entity_node* victim = current;
    current = current->next_entity;
    free(victim->entity);
    free(victim);
  }
}
