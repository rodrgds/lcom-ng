#ifndef LCOM_LIST_H
#define LCOM_LIST_H

#include <lcom/lcf.h>
#include <stdlib.h>
#include <stdio.h>
#include "hitbox.h"
#include "game.h"

/**
 * @brief Structure representing a node in the linked list.
 */
typedef struct node{
    invis_hitbox node;  /*The invisible hitbox stored in the node*/
    struct node *next;  /*Pointer to the next node in the list*/
}node;

/**
 * @brief Structure representing a node in the enemy linked list.
 */
typedef struct enemy_node{
    enemy_character node;        /*The enemy character stored in the node*/
    struct enemy_node *next;     /*Pointer to the next enemy character stored in the node*/
}enemy_node;

/**
 * @brief Creates an empty linked list with a given initial invisible hitbox.
 *
 * @param a The initial invisible hitbox for the list.
 */
void createList(invis_hitbox a);

/**
 * @brief Adds a node with the given invisible hitbox to the linked list.
 *
 * @param a The invisible hitbox to be added to the list.
 */
void addNode(invis_hitbox a);

/**
 * @brief Deletes the last node from the linked list.
 */
void deleteNode();

/**
 * @brief Deletes the entire linked list.
 */
void deleteList();

/**
 * @brief Creates an empty linked list for enemy characters with a given initial character.
 *
 * @param a The initial enemy character for the list.
 */
void createEnemyList(enemy_character a);

/**
 * @brief Adds a node with the given enemy character to the enemy linked list.
 *
 * @param a The enemy character to be added to the list.
 */
void addEnemyNode(enemy_character a);

/**
 * @brief Deletes the specified enemy node from the enemy linked list.
 *
 * @param a Pointer to the enemy node to be deleted.
 */
void deleteEnemyNode(enemy_node *a);

/**
 * @brief Deletes the last node from the enemy linked list.
 */
void deleteLastEnemyNode();

/**
 * @brief Deletes the entire enemy linked list.
 */
void deleteEnemyList();

#endif
