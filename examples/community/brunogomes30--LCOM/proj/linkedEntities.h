#ifndef LINKED_ENTITIES_H
#define LINKED_ENTITIES_H

#include "sprite.h"

/** @defgroup linkedEntities
 * @{
 *
 * Functions related to Double Linked List 
 */

typedef struct EntitiesLinkedElement EntitiesLinkedElement;

typedef enum{PLAYER, ENEMY, BULLET, GUN, SPRITE} EntityType;

/**
 * @brief Double Linked List with all the entities.Has the previous and next to be more efficient on removing and inserting entities
 * 
 * 
 */
 struct EntitiesLinkedElement{
  EntitiesLinkedElement *previous;
  void *data;
  EntityType type;
  uint8_t remove;
  EntitiesLinkedElement *next;
};

typedef struct {
  EntitiesLinkedElement * firstElement;
  EntitiesLinkedElement * lastElement;
} DoubleLinkedList;

//First element is the player

/**
 * @brief Add entity to list
 * 
 * @param list 
 * @param data 
 * @param type 
 */
void addEntity(DoubleLinkedList *list, void *data, EntityType type);

/**
 * @brief Remove entity from list
 * 
 * @param list 
 * @param linkedElement 
 */
void removeLinkedElement(DoubleLinkedList *list, EntitiesLinkedElement *linkedElement);
#endif
