/**
 * @file path_map.h
 * @brief Path map data structures -- tile grid, autotiling masks, and level loading.
 * @ingroup group_levels
 *
 * Path maps are tile grids where each cell is empty, path, spawn, goal, or
 * tower. Autotiling computes a 4-neighbor bitmask to pick the correct road
 * sprite. Level definitions are ASCII maps parsed via path_map_load_level().
 */

#ifndef PROJ_PATH_MAP_H
#define PROJ_PATH_MAP_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
  /* Four-way neighbor bits used to pick the correct road sprite. */
  PATH_NEIGHBOR_NORTH = 0x1,
  PATH_NEIGHBOR_SOUTH = 0x2,
  PATH_NEIGHBOR_EAST = 0x4,
  PATH_NEIGHBOR_WEST = 0x8,
} path_neighbor_mask_t;

typedef enum {
  PATH_TILE_EMPTY = 0,
  PATH_TILE_PATH,
  PATH_TILE_SPAWN,
  PATH_TILE_GOAL,
  PATH_TILE_TOWER,
} path_tile_t;

typedef struct {
  uint8_t grid_x;
  uint8_t grid_y;
} tile_coord_t;

typedef struct {
  path_neighbor_mask_t neighbor_mask;
  tile_coord_t coords;
} autotile_pick_t;

typedef struct {
  /* Grid stored in row-major order: tiles[y * width + x]. */
  uint16_t width;
  uint16_t height;
  path_tile_t *tiles;
  /* Optional markers parsed from the level layout. */
  int16_t spawn_x;
  int16_t spawn_y;
  int16_t goal_x;
  int16_t goal_y;
  bool has_spawn;
  bool has_goal;
} path_map_t;

struct wave;

typedef struct {
  uint16_t width;
  uint16_t height;
  /* ASCII level map: one row per line using '.', '#', 'S', 'G', 'T'. */
  const char *data;
  const struct wave *waves;
  uint16_t wave_count;
  uint16_t wave_bonus_base;
  uint16_t wave_bonus_step;
  uint16_t start_coins;
} level_t;

int path_map_init(path_map_t *map, uint16_t width, uint16_t height);
void path_map_destroy(path_map_t *map);
path_tile_t path_map_get_tile(const path_map_t *map, uint16_t x, uint16_t y);
int path_map_set_tile(path_map_t *map, uint16_t x, uint16_t y, path_tile_t tile);
bool path_map_tile_is_path(path_tile_t tile);
/* Reads the four adjacent tiles and returns the bitmask for autotiling. */
path_neighbor_mask_t path_map_get_neighbor_mask(const path_map_t *map, uint16_t x, uint16_t y);
/* Converts a 4-bit neighbor mask into a spritesheet grid position. */
tile_coord_t path_map_get_autotile_coords(path_neighbor_mask_t neighbor_mask);
/* Convenience helper that performs both the neighbor scan and the lookup. */
autotile_pick_t path_map_pick_autotile(const path_map_t *map, uint16_t x, uint16_t y);
int path_map_load_level(path_map_t *map, const level_t *level);

#endif
