/** 
 * @file attack.c
 * @brief Source code file for all the logic related to the attack and attack_node structs
 * 
 * This file implements all the functions for the attack and attack_node structs.
 */


#include "attack.h"

attack* (attack_create_full)(double pos_x, double pos_y, double speed_x, double speed_y, uint32_t damage, uint32_t frames, animation animation) {
  attack* new_attack = (attack*)malloc(sizeof(attack));

  new_attack->pos_x = pos_x;
  new_attack->pos_y = pos_y;
  new_attack->speed_x = speed_x;
  new_attack->speed_y = speed_y;
  new_attack->damage = damage;
  new_attack->frames = frames;
  new_attack->animation = animation;

  return new_attack;
}

int (attack_destroy)(attack* attack){
  free(attack);
  return 0;
}

void (attack_move)(attack* attack){
  attack->frames--;
  attack->pos_x -= attack->speed_x;
  attack->pos_y -= attack->speed_y;
}

void (attack_list_add)(attack_node** list, attack* attack) {
  attack_node* new_node = (attack_node*)malloc(sizeof(attack_node));
  new_node->attack = attack;
  new_node->next_attack = *list;
  *list = new_node;
}

void (attack_node_destroy)(attack_node* node){
  free(node->attack);
  free(node);
}

void (attack_list_destroy)(attack_node* list) {
  attack_node* current = list;
  while(current != NULL) {
    attack_node* victim = current;
    current = current->next_attack;
    attack_node_destroy(victim);
  }
}
