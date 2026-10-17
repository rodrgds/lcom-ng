/** 
 * @file entity.h
 * @brief Header file for @ref entity.c
 * 
 * This file declares all the functions to be used for the entity and entity_node structs.
 */


#ifndef _ENTITY_PROJ_H_
#define _ENTITY_PROJ_H_

#include <lcom/lcf.h>
#include <math.h>

#include "../drivers/drivers.h"
#include "../viewer/animations.h"
#include "../viewer/static_images.h"
#include "structs.h"

/**
 * @brief Checks if an entity is idle
 * 
 * An entity is considered idle if both speeds are 0.
 * 
 * @param entity The pointer to the entity to check
 * @return true if it is idle, 1 otherwise
 */
bool (entity_is_idle)(entity* entity);

/**
 * @brief Creates an entity with default parameters except for the animation
 * 
 * Creates an entity by dynamically allocating space for it and then setting its members equal to the one passed as argument.

 * @param animations The pointer to the animations of the new entity
 * @return The pointer to the new entity
 */
entity* (entity_create)(animation* animations);

/**
 * @brief Creates an entity with all the parameters customized
 * 
 * Creates an entity by dynamically allocating space for it and then setting its members equal to the ones passed as arguments.
 * 
 * @param pos_x The x coordinate of the new entity
 * @param pos_y The y coordinate of the new entity
 * @param speed_x The speed on the x axis of the new entity
 * @param speed_y The speed on the y axis of the new entity
 * @param speed_multiplier The speed multiplier of the new entity
 * @param health The health of the new entity
 * @param animations The pointer to the animations of the new entity
 * @return The pointer to the new entity
 */
entity* (entity_create_full)(double pos_x, double pos_y, double speed_x, double speed_y, double speed_multiplier, uint32_t health, animation* animations);

/**
 * @brief Destroys an entity by freeing its memory
 * 
 * Frees the memory of the entity itself.
 * 
 * @param entity The pointer to the entity to destroy
 * @return 0 in case of success, 1 otherwise
 */
int (entity_destroy)(entity* entity);

/**
 * @brief Advances one frame to the entity
 * 
 * Normalizes the speed vector and then multiplies the result by the speed_multiplier, adding it to the position.
 * 
 * @param entity The pointer to the entity to move
 */
void (entity_move)(entity* entity);

/**
 * @brief Destroys an entity_node
 * 
 * Frees the memory of the entity member and then the entity_node itself.
 * 
 * @param node The pointer to the entity_node to destroy
 */
void (entity_node_destroy)(entity_node* node);

/**
 * @brief Adds the entity to the entity_node list
 * 
 * Creates a new entity_node to store the entity argument and then adds it to the end of the list.
 * 
 * @param list The pointer to the list to be added on
 * @param entity The pointer to the entity to add
 */
void (entity_list_add)(entity_node** list, entity* entity);

/**
 * @brief Destroys an entity_node list
 * 
 * Iterates through the linked list and calls entity_node_destroy() on each node.
 * 
 * @param list The pointer to the list to be destroyed
 */
void (entity_list_destroy)(entity_node* list);

#endif /*_ENTITY_PROJ_H_ */
