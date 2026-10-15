/**
 * @file Entity.h
 * @brief This file contains functions regarding the creation and collisions between game entities
 * @date 2024-06-02
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <stdio.h>
#include <stdlib.h>
#include "../utils/aux_structs/HurtBox.h"
#include "../utils/aux_structs/Position.h"


typedef enum {
    PLAYER,
    CAT,
    CAT0,
    CAT1,
    CAT2,
    CAT3,
    CAT4,
    WALL,
    CLOUD,
} EntityType;

typedef enum {
    X_COLLISION,
    Y_COLLISION,
    XY_COLLISION,
    NO_COLLISION
} CollisionType;

typedef struct {
    Position* position;
    HurtBox* hurtBox;
    double velocity;        // pixels/s
    int direction;     // angle, 0 being right
    EntityType type;  
    void* typeInfo;         // entity type specific info
} Entity;
/**
 * @brief Creates a cat with a random color in chosen position
 * 
 * @param x chosen x axis position
 * @param y chosen y axis position
 * @return pointer to new cat
 */
Entity* newCat(int x, int y);  //returns pointer to new cat with random color
/**
 * @brief Creates a player in chosen position
 * 
 * @param x chosen x axis position
 * @param y chosen y axis position
 * @return pointer to new player
 */
Entity* newPlayer(int x, int y);
/**
 * @brief Creates a wall in chosen position
 * 
 * @param x chosen x axis position
 * @param y chosen y axis position
 * @return pointer to new wall 
 */
Entity* newWall(int x, int y);
/**
 * @brief Creates a cloud in chosen position, with chosen velocity
 * 
 * @param x chosen x axis position
 * @param y chosen y axis position
 * @param velocity chosen velocity
 * @return pointer to new cloud 
 */
Entity *newCloud(int x, int y, double velocity);

/**
 * @brief Frees the memory allocated to entity
 * 
 * @param entity entity to delete
 */
void deleteEntity(Entity* entity);
/**
 * @brief Calculates a future position based on the entity's speed without collisions in mind
 * 
 * @param entity 
 * @return future position 
 */
Position candidatePos(Entity* entity);
/**
 * @brief Checks if entityA in cposA (candidate position) would collide with entity B
 * 
 * @param cPosA candidate position
 * @param entityA entity a
 * @param entityB entity b
 * @return the type of collision 
 */
CollisionType checkCollision(Position cPosA, Entity* entityA, Entity* entityB);


#endif
