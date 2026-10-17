#ifndef __MAP_H
#define __MAP_H

#include <lcom/lcf.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../drivers/graphics/graphics.h"
#include "../sprites/sprite.h"
#include "../sprites/falling_block.h"
#include "../objects/block.h"
#include "../xpm/background/level_background1.xpm"
#include "../xpm/background/level_background2.xpm"
#include "../xpm/background/level_background3.xpm"
#include "../xpm/background/wall1.xpm"
#include "../xpm/background/wall2.xpm"
#include "../xpm/background/lava1.xpm"
#include "../xpm/background/lava2.xpm"
#include "../xpm/background/lava3.xpm"
#include "../xpm/background/water1.xpm"
#include "../xpm/background/water2.xpm"
#include "../xpm/background/water3.xpm"
#include "../xpm/background/rope.xpm"

#include "../xpm/background/fire_door1.xpm"
#include "../xpm/background/fire_door2.xpm"
#include "../xpm/background/fire_door3.xpm"
#include "../xpm/background/fire_door4.xpm"
#include "../xpm/background/water_door1.xpm"
#include "../xpm/background/water_door2.xpm"
#include "../xpm/background/water_door3.xpm"
#include "../xpm/background/water_door4.xpm"
#include "../xpm/background/poison1.xpm"
#include "../xpm/background/poison2.xpm"
#include "../xpm/background/poison3.xpm"

#define MAPS_PATH "/home/lcom/labs/proj/src/maps/map"
#define MAPS_EXT ".txt"
#define TILE_SIZE 32

Sprite* ropes[10][10];


typedef struct {
    int x, y;
    uint32_t rows, columns;
    char* map;
    Falling_Block* blocks[10];
    int n_blocks;
} Map;

Map* map1;
Map* map2;
Map* map3;
Map* current_map;

/**
 * @brief Creates a map corresponding to the given level.
 *
 * @param level The level of the map.
 * @return Map* The created map.
 */
Map* create_map(int level);

/**
 * @brief Loads all the maps.
 *
 * @return 1 if an error occurs while loading the maps, 0 otherwise.
 */
int load_maps();

/**
 * @brief Draws the given map.
 *
 * @param map The map to be drawn.
 * @return 1 if an image fails to load, 0 otherwise.
 */
int draw_map(Map* map);

/**
 * @brief Changes the current level to the next level.
 *
 * @return 1 if the current level was the last, 0 otherwise.
 */
int nextLevel();

/**
 * @brief Returns the letter corresponding to the tile at the given position on the map.
 * 
 * @param map The map from which to retrieve the tile.
 * @param x The x position on the map.
 * @param y The y position on the map.
 * @return char The letter corresponding to the tile at the given coordinates.


 */
char get_tile(Map* map, u_int32_t x, u_int32_t y);

/**
 * @brief Resets all the falling blocks to their original position
 * 
 */
void (reset_falling_blocks)();

#endif /* __MAP_H */
