/** 
 * @file attack.h
 * @brief Header file for @ref attack.c
 * 
 * This file declares all the functions to be used for the attack and attack_node structs.
 */


#ifndef _ATTACK_PROJ_H_
#define _ATTACK_PROJ_H_

#include <lcom/lcf.h>
#include <math.h>

#include "../drivers/drivers.h"
#include "../viewer/animations.h"
#include "../viewer/static_images.h"
#include "structs.h"

/**
 * @brief Creates an attack with all the parameters customized
 * 
 * Creates an attack by dynamically allocating space for it and then setting its members equal to the ones passed as arguments.
 * 
 * @param pos_x The x coordinate of the new attack
 * @param pos_y The y coordinate of the new attack
 * @param speed_x The speed on the x axis of the new attack
 * @param speed_y The speed on the y axis of the new attack
 * @param damage The damage of the new attack
 * @param frames The frames duration of the new attack
 * @param animation The animation of the new attack
 * @return The pointer to the new attack
 */
attack* (attack_create_full)(double pos_x, double pos_y, double speed_x, double speed_y, uint32_t damage, uint32_t frames, animation animation);

/**
 * @brief Destroys an attack by freeing its memory
 * 
 * Frees the memory of the attack itself.
 * 
 * @param attack The pointer to the attack to destroy
 * @return 0 in case of success, 1 otherwise
 */
int (attack_destroy)(attack* attack);

/**
 * @brief Advances one frame to the attack
 * 
 * Adds the speed of the attack to its position.
 * 
 * @param attack The pointer to the attack to move
 */
void (attack_move)(attack* attack);

/**
 * @brief Destroys an attack_node
 * 
 * Frees the memory of the attack member and then the attack_node itself.
 * 
 * @param node The pointer to the attack_node to destroy
 */
void (attack_node_destroy)(attack_node* node);

/**
 * @brief Adds the attack to the attack_node list
 * 
 * Creates a new attack_node to store the attack argument and then adds it to the end of the list.
 * 
 * @param list The pointer to the list to be added on
 * @param attack The pointer to the attack to add
 */
void (attack_list_add)(attack_node** list, attack* attack);

/**
 * @brief Destroys an attack_node list
 * 
 * Iterates through the linked list and calls attack_node_destroy() on each node.
 * 
 * @param list The pointer to the list to be destroyed
 */
void (attack_list_destroy)(attack_node* list);

#endif /*_ATTACK_PROJ_H_ */
