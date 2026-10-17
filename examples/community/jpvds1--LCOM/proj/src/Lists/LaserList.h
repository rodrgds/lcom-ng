/**

@file LaserList.h
@brief Header file containing declarations of functions, variables and structs needed to use the laser list.

*/
#ifndef LASERLIST_H
#define LASERLIST_H

#include <lcom/lcf.h>
#include <stdio.h>
#include <stdlib.h>

extern struct LaserNode *LaserFirst;
extern struct LaserNode *LaserAdded ;

struct Laser
{
    uint16_t x;
    uint16_t y;
    int user;
};

struct LaserNode
{
    struct Laser* laser;
    struct LaserNode* next;
};

/**
 * @brief prints current position of LaserNode node
 * 
 * @param node LaserNode to be printed
 */
void ll_printNode(const struct LaserNode *node);
/**
 * @brief creates a new node with position x and y (user = 1 if comes from user and 0 if comes from enemy), separated from the list and returns it
 * 
 * @param x x position of the node
 * @param y y position of the node
 * @param user set to 1 if laser comes from user and 0 if it comes from an enemy
 * @return struct LaserNode* Return the new node with the passed position and user values
 */
struct LaserNode *ll_getNewNode(uint16_t x, uint16_t y, int user);
/**
 * @brief prints whole list of LaserNodes
 * 
 * @param list LaserNode from where to start printing LaserList
 */
void ll_printList(const struct LaserNode *list);
/**
 * @brief frees allocated memory of all nodes
 * 
 * @param list LaserNode from where to start freeing LaserList
 */
void ll_CleanUp(struct LaserNode *list);
/**
 * @brief frees allocated memory of a LaserNode
 * 
 * @param cnode LaserNode whose allocated memory should be freed
 */
void ll_CleanNode(struct LaserNode *cnode);
/**
 * @brief removes first LaserNode of the list
 * 
 */
void ll_removeFirst();
/**
 * @brief removes selected LaserNode
 * 
 * @param rnode LaserNode to be removed
 */
void ll_removeNode(struct LaserNode *rnode);
/**
 * @brief add a new LaserNode to the list
 * 
 * @param newNode LaserNode to be added to the LaserList
 */
void ll_addNode(const struct LaserNode *newNode);

#endif //LASERLIST_H
