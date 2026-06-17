/**
 * @file placement.h
 * @brief Tower placement validation -- pixel-to-tile conversion and collision checks.
 * @ingroup group_towers
 *
 * Converts mouse pixel coordinates to tile coordinates and validates
 * whether a tile can accept a tower (not on path, within bounds, not
 * already occupied).
 */

#ifndef PROJ_PLACEMENT_H
#define PROJ_PLACEMENT_H

#include <stdint.h>
#include <stdbool.h>
#include "path_map.h"
#include "tower_field.h"

#define PLACEMENT_MAP_ORIGIN_X 8
#define PLACEMENT_MAP_ORIGIN_Y 8
#define PLACEMENT_TILE_SIZE 32

typedef enum {
  TILE_PLACEMENT_OK = 0,
  TILE_PLACEMENT_OUT_OF_BOUNDS,
  TILE_PLACEMENT_ON_PATH,
  TILE_PLACEMENT_OCCUPIED, //serve tanto para T no ASCII como para as colocadas pela seed para debug
} tile_placement_result_t;

/**
 * @brief Convert screen pixel coordinates to tile coordinates.
 *
 * Accounts for the map origin offset and tile size.  Returns false if
 * the pixel position is outside the tile map bounds.
 *
 * @param map      Path map defining the tile grid.
 * @param pixel_x  Screen X in pixels.
 * @param pixel_y  Screen Y in pixels.
 * @param col_out  Output: tile column index.
 * @param row_out  Output: tile row index.
 * @return true if the conversion is valid, false if out of bounds.
 */
bool pixels_to_tile_coordinates(const path_map_t *map, int16_t pixel_x, int16_t pixel_y, uint16_t *col_out, uint16_t *row_out);

/**
 * @brief Validate whether a tile can accept a tower placement.
 *
 * Checks bounds, path occupancy, and whether another tower already
 * occupies the tile.
 *
 * @param map    Path map defining walkable / buildable tiles.
 * @param field  Tower field to check for existing towers.
 * @param col    Tile column to validate.
 * @param row    Tile row to validate.
 * @return TILE_PLACEMENT_OK, TILE_PLACEMENT_OUT_OF_BOUNDS,
 *         TILE_PLACEMENT_ON_PATH, or TILE_PLACEMENT_OCCUPIED.
 */
tile_placement_result_t validate_tile_placement(const path_map_t *map, const tower_field_t *field, int16_t col, int16_t row);


#endif
