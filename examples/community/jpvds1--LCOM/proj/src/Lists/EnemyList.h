/**

@file EnemyList.h
@brief Header file containing declarations of functions, variables and structs needed to use the enemy list.

*/
#ifndef ENEMYLIST_H
#define ENEMYLIST_H

#include <lcom/lcf.h>
#include <stdio.h>
#include <stdlib.h>

extern struct EnemyNode *EnemyFirst;
extern struct EnemyNode *EnemyAdded;
extern int EnemySize;

struct Enemy
{
    uint16_t x;
    uint16_t y;
    uint16_t min_x;
    uint16_t max_x;
    uint16_t min_y;
    uint16_t max_y;
    int direction;
};

struct EnemyNode
{
    struct Enemy* enemy;
    struct EnemyNode* next;
};

/**
 * @brief prints current position and direction of EnemyNode node
 * 
 * @param node EnemyNode to be printed
 */
void el_printNode(const struct EnemyNode *node);
/**
 * @brief creates a new node with position x and y and limited to [min_x, max_x] and [min_y, max_y], separated from the list and returns it
 * 
 * @param x x position of EnemyNode
 * @param y y position of EnemyNode
 * @param min_x minimum x position for EnemyNode
 * @param max_x maximum x position for EnemyNode
 * @param min_y minimum y position for EnemyNode
 * @param max_y maximum y position for EnemyNode
 * @return struct EnemyNode* Returns the enemy node created with the passed arguments
 */
struct EnemyNode *el_getNewNode(uint16_t x, uint16_t y, uint16_t min_x, uint16_t max_x, uint16_t min_y, uint16_t max_y);
/**
 * @brief prints whole list of EnemyNodes
 * 
 * @param list EnemyNode from where to start printing EnemyList
 */
void el_printList(const struct EnemyNode *list);
/**
 * @brief frees allocated memory of all nodes
 * 
 * @param list EnemyNode from where to start freeing allocated memory from EnemyList
 */
void el_CleanUp(struct EnemyNode *list);
/**
 * @brief frees allocated memory of an EnemyNode
 * 
 * @param cnode EnemyNode whose allocated memory should be free
 */
void el_CleanNode(struct EnemyNode *cnode);
/**
 * @brief removes first node of the list
 * 
 */
void el_removeFirst();
/**
 * @brief removes selected node
 * 
 * @param rnode EnemyNode to be removed
 */
void el_removeNode(struct EnemyNode *rnode);
/**
 * @brief add a new node to the list
 * 
 * @param newNode EnemyNode to be added to the EnemyList
 */
void el_addNode(const struct EnemyNode *newNode);

#endif //ENEMYLIST_H
