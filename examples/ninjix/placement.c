/**
 * @file placement.c
 * @brief Placement validation implementation -- pixel-to-tile conversion and tile checks.
 * @ingroup group_towers
 */

#include "placement.h"
#include <stddef.h>

bool pixels_to_tile_coordinates(const path_map_t *map, int16_t pixel_x, int16_t pixel_y, uint16_t *col_out, uint16_t *row_out) {
  if (map == NULL || col_out == NULL || row_out == NULL)
    return false;

  int16_t col = (int16_t)((pixel_x - PLACEMENT_MAP_ORIGIN_X) / PLACEMENT_TILE_SIZE);
  int16_t row = (int16_t)((pixel_y - PLACEMENT_MAP_ORIGIN_Y) / PLACEMENT_TILE_SIZE);

  if (col < 0 || row < 0)
    return false;

  if ((uint16_t)col >= map->width || (uint16_t)row >= map->height)
    return false;

  *col_out = (uint16_t)col;
  *row_out = (uint16_t)row;
  return true;
}

static bool tile_has_tower(const path_map_t *map, const tower_field_t *field, uint16_t col, uint16_t row) {
  if (field == NULL)
    return false;

  for (uint16_t i = 0; i < field->tower_count; i++) {
    const tower_t *tower = &field->towers[i];
    if (tower->type == NULL)
      continue;
    int16_t center_x = (int16_t)(tower->x + tower->type->frame_width / 2);
    int16_t center_y = (int16_t)(tower->y + tower->type->frame_height / 2);

    uint16_t tower_col;
    uint16_t tower_row;
    if (!pixels_to_tile_coordinates(map, center_x, center_y, &tower_col, &tower_row))
      continue;

    if (tower_col == col && tower_row == row)
      return true;
  }

  return false;
}

tile_placement_result_t validate_tile_placement(const path_map_t *map, const tower_field_t *field, int16_t col, int16_t row){
  if (map == NULL)
    return TILE_PLACEMENT_OUT_OF_BOUNDS;

  if (col < 0 || row < 0)
    return TILE_PLACEMENT_OUT_OF_BOUNDS;
  
  if (col >= (int16_t)map->width || row >= (int16_t)map->height)
    return TILE_PLACEMENT_OUT_OF_BOUNDS;

  path_tile_t tile;
  tile = path_map_get_tile(map, (uint16_t)col, (uint16_t)row);
  
  if (path_map_tile_is_path(tile))
    return TILE_PLACEMENT_ON_PATH;

  if (tile == PATH_TILE_TOWER)
    return TILE_PLACEMENT_OCCUPIED;

  if (tile_has_tower(map, field, (uint16_t)col, (uint16_t)row))
    return TILE_PLACEMENT_OCCUPIED;
  
  return TILE_PLACEMENT_OK;
}
