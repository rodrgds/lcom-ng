/**

@file AnimationList.h
@brief Header file containing declarations of functions, variables and structs needed to use the animation list.

*/

#ifndef ANIMATION_H
#define ANIMATION_H

#include <lcom/lcf.h>
#include <stdio.h>
#include <stdlib.h>

extern struct AnimationNode *AnimationFirst;
extern struct AnimationNode *AnimationAdded;
extern int AnimationSize;

struct Animation
{
    uint16_t x;
    uint16_t y;
    uint16_t curr_xpm;
    uint16_t time_to_change;
};

struct AnimationNode
{
    struct Animation* animation;
    struct AnimationNode* next;
};

/**
 * @brief Prints a specific node of the list in a human friendly way
 * 
 * @param node The node of type struct AnimationNode to be printed
 */
void al_printNode(const struct AnimationNode *node);
/**
 * @brief creates a new node with position x and y separated from the list and returns it
 * 
 * @param x The x position of the AnimationNode
 * @param y The y position  of the AnimationNode
 * @return struct AnimationNode* Return the new node with the passed position
 */
struct AnimationNode *al_getNewNode(uint16_t x, uint16_t y);
/**
 * @brief prints whole list of AnimationNodes
 * 
 * @param list AnimationNode from where to start printing the AnimationList
 */
void al_printList(const struct AnimationNode *list);
/**
 * @brief frees allocated memory of all nodes
 * 
 * @param list AnimationNode from where to start freeing allocated memory from AnimationList
 */
void al_CleanUp(struct AnimationNode *list);
/**
 * @brief frees allocated memory of an AnimationNode
 * 
 * @param cnode AnimationNode whose allocated memory should be freed
 */
void al_CleanNode(struct AnimationNode *cnode);
/**
 * @brief removes first node of the list
 * 
 */
void al_removeFirst();
/**
 * @brief removes selected node
 * 
 * @param rnode AnimationNode to be removed
 */
void al_removeNode(struct AnimationNode *rnode);
/**
 * @brief add a new node to the end of the list
 * 
 * @param newNode AnimationNode to be added to the AnimationList
 */
void al_addNode(const struct AnimationNode *newNode);

#endif //AnimationLIST_H
