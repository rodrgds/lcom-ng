#ifndef _MAP_CONTROLLER_H_
#define _MAP_CONTROLLER_H_

#include "src/include.h"
#include "src/Model/Map/Map.h"
#include "src/Model/Elements/Bgs.h"

/**
 * @file MapController.h
 * @brief Declares MapController structure and functions to manage the game map and elements.
 *
 * Provides functions to manipulate the map,
 * retrieve player instance, and update tree growth and map state.
 */

typedef struct{
    Map *map;

} MapController;

static MapController *mc;

void MapInit();
MapController *get_MapController();
MapController *new_MapController(void);
void delete_MapController();
void add(MapController *mc, ElementType type , Position position, 
        int16_t width, int16_t height, bool block_up);
bool delete(MapController *mc, size_t idx);
void check_up(MapController *mc);
Player* get_player();
void update_Trees(MapController *mc);
void update_map();


#endif
