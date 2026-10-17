#ifndef _MAP_H_
#define _MAP_H_

#include"src/Model/Elements/Element.h"
#include "src/Model/Elements/player.h"
#include "src/Model/Map/Sun.h"
#include "src/Model/Map/Moon.h"
#include "src/Model/Map/Cloud.h"

/**
 * @file Map.h
 * @brief Map structure and its associated operations.
 *
 * Aggregates various game elements
 * (sun, moon, clouds) and the player. 
 * 
 * 
 */

/**
 * @struct Map
 * @brief Represents the entire scene of the game.
 *
 * Contains all elements that compose the map, 
 * including dynamic elements like the player, sun, moon, and clouds.
 */

typedef struct {
    Element **elements;
    size_t count_elements;
    Sun sun;
    Moon moon;
    Cloud cloud;
    Cloud cloud2;
    Player *player;
} Map;

/**
 * @brief Calculates the number of block elements that fit horizontally in the screen.
 *
 * @return Number of blocks based on screen resolution and block size.
 */

int Number_block();

/**
 * @brief Allocates and initializes a new Map.
 *
 * Creates a new map, initializes its elements, sun, moon, clouds, and player.
 *
 * @return Pointer to the newly created Map, or NULL on failure.
 */

Map *new_Map(void);

/**
 * @brief Frees the memory allocated for the map and its components.
 *
 * @param m Pointer to the Map instance to be deleted.
 */

void delete_Map(Map *m);


#endif
