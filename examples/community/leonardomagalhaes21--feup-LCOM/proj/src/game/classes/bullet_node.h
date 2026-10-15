/**
 * @file bullet_node.h
 * @brief Contains functions for managing bullet nodes in a linked list.
 */

#ifndef BULLET_NODE_H_
#define BULLET_NODE_H_

#include "bullet.h"
#include <math.h>
#include <stdlib.h>


/**
 * @brief Struct representing a node in a linked list of bullets.
 *
 * This struct holds a pointer to a bullet and a pointer to the next node in the linked list.
 * Each node in the list contains one bullet and a reference to the next node.
 */
typedef struct bullet_node {
    bullet *shot;
    struct bullet_node *next; 
} bullet_node;

/**
 * @brief Struct representing a linked list of bullets.
 *
 * @param head Pointer to the first node in the linked list
 *
 * This struct holds a pointer to the head of the linked list of bullets.
 * It provides an interface for adding, removing, and managing bullets in the list.
 */
typedef struct {
    bullet_node *head;
} bullet_list;


/**
 * @brief Creates a new bullet node.
 *
 * @param shot The bullet to associate with the node.
 * @return A pointer to the newly created bullet node, or NULL if memory allocation fails.
 */
bullet_node *createBulletNode(bullet *shot);


/**
 * @brief Destroys a bullet node and releases associated memory.
 *
 * @param node The bullet node to destroy.
 */
void destroyBulletNode(bullet_node *node);


/**
 * @brief Destroys all bullet nodes in a linked list.
 *
 * @param head Pointer to the head of the linked list.
 */
void destroyAllBullets(bullet_node *head);


/**
 * @brief Adds a new bullet to the beginning of a linked list.
 *
 * @param head Pointer to the pointer to the head of the linked list.
 * @param shot Pointer to the bullet to be added.
 */
void addBullet(bullet_node **head, bullet *shot);


/**
 * @brief Removes a bullet from the linked list.
 *
 * @param head Pointer to the pointer to the head of the linked list.
 * @param node_to_remove Pointer to the bullet node to be removed.
 */
void removeBullet(bullet_node **head, bullet_node *node_to_remove);

#endif
