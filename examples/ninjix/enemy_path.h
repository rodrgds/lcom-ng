/**
 * @file enemy_path.h
 * @brief Path computation -- converts a tile map into a sequence of waypoints.
 * @ingroup group_enemies
 *
 * Scans the path_map_t tile grid from the spawn marker to the goal marker
 * and emits an ordered list of pixel-space waypoints that enemies follow.
 */

#ifndef PROJ_ENEMY_PATH_H
#define PROJ_ENEMY_PATH_H

#include <stdbool.h>
#include <stdint.h>

#include "path_map.h"

#define MAX_WAYPOINTS 512
#define ENEMY_PATH_TILE_SIZE 32
#define ENEMY_PATH_ORIGIN_X 8
#define ENEMY_PATH_ORIGIN_Y 8

bool enemy_path_compute(const path_map_t *map,
                        int16_t *waypoints_x, int16_t *waypoints_y,
                        uint16_t *waypoint_count, uint16_t max_waypoints);

#endif
