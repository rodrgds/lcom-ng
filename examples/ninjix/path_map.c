/**
 * @file path_map.c
 * @brief Path map implementation -- tile grid init, autotiling, and ASCII level loading.
 * @ingroup group_levels
 */

#include "path_map.h"

#include <stdlib.h>
#include <string.h>

/*
 * Autotile lookup table for the 16 possible 4-way neighbor masks.
 *
 * Maps each mask (N=1, S=2, E=4, W=8) to (grid_x, grid_y) coordinates
 * in the 192x256 tileset (6 cols x 8 rows of 32x32 tiles).
 *
 * Rows 1-2: path pieces  (grid_y 0-1)
 * Row 3-4:  edge/filler  (grid_y 2-3)
 * Row 5:    path caps    (grid_y 4)
 * Rows 6-8: grass tiles  (grid_y 5-7)
 */
static const tile_coord_t autotile_coords[16] = {
  {4, 2},  /* 0x0  (none)        → filler R3C5            */
  {3, 4},  /* 0x1  (N)           → cap up R5C4            */
  {2, 4},  /* 0x2  (S)           → cap down R5C3          */
  {0, 0},  /* 0x3  (N+S)         → vertical A R1C1        */
  {0, 4},  /* 0x4  (E)           → cap right R5C1         */
  {5, 0},  /* 0x5  (N+E)         → corner RU A R1C6       */
  {2, 0},  /* 0x6  (S+E)         → corner RD A R1C3       */
  {4, 2},  /* 0x7  (N+S+E)       → filler R3C5 (no T-tile)*/
  {1, 4},  /* 0x8  (W)           → cap left R5C2          */
  {4, 0},  /* 0x9  (N+W)         → corner LU A R1C5       */
  {3, 0},  /* 0xA  (S+W)         → corner LD A R1C4       */
  {4, 2},  /* 0xB  (N+S+W)       → filler R3C5 (no T-tile)*/
  {1, 0},  /* 0xC  (E+W)         → horizontal A R1C2      */
  {4, 2},  /* 0xD  (N+E+W)       → filler R3C5 (no T-tile)*/
  {4, 2},  /* 0xE  (S+E+W)       → filler R3C5 (no T-tile)*/
  {4, 2},  /* 0xF  (N+S+E+W)     → filler R3C5 (no cross) */
};

static bool path_map_is_marker_path(path_tile_t tile)
{
  return tile == PATH_TILE_PATH || tile == PATH_TILE_SPAWN || tile == PATH_TILE_GOAL;
}

int path_map_init(path_map_t *map, uint16_t width, uint16_t height)
{
  if (map == NULL || width == 0 || height == 0)
    return 1;

  memset(map, 0, sizeof(*map));
  map->width = width;
  map->height = height;
  map->tiles = calloc((size_t)width * height, sizeof(*map->tiles));
  if (map->tiles == NULL)
  {
    memset(map, 0, sizeof(*map));
    return 1;
  }

  return 0;
}

void path_map_destroy(path_map_t *map)
{
  if (map == NULL)
    return;

  free(map->tiles);
  memset(map, 0, sizeof(*map));
}

path_tile_t path_map_get_tile(const path_map_t *map, uint16_t x, uint16_t y)
{
  if (map == NULL || map->tiles == NULL || x >= map->width || y >= map->height)
    return PATH_TILE_EMPTY;

  return map->tiles[(size_t)y * map->width + x];
}

int path_map_set_tile(path_map_t *map, uint16_t x, uint16_t y, path_tile_t tile)
{
  if (map == NULL || map->tiles == NULL || x >= map->width || y >= map->height)
    return 1;

  map->tiles[(size_t)y * map->width + x] = tile;
  return 0;
}

bool path_map_tile_is_path(path_tile_t tile)
{
  return path_map_is_marker_path(tile);
}

/* Scan the four neighbors around a tile and encode them as a bitmask. */
path_neighbor_mask_t path_map_get_neighbor_mask(const path_map_t *map, uint16_t x, uint16_t y)
{
  if (map == NULL || map->tiles == NULL || x >= map->width || y >= map->height)
    return (path_neighbor_mask_t)0;

  path_neighbor_mask_t mask = (path_neighbor_mask_t)0;

  if (y > 0 && path_map_is_marker_path(path_map_get_tile(map, x, (uint16_t)(y - 1))))
    mask = (path_neighbor_mask_t)(mask | PATH_NEIGHBOR_NORTH);
  if ((uint32_t)y + 1 < map->height && path_map_is_marker_path(path_map_get_tile(map, x, (uint16_t)(y + 1))))
    mask = (path_neighbor_mask_t)(mask | PATH_NEIGHBOR_SOUTH);
  if ((uint32_t)x + 1 < map->width && path_map_is_marker_path(path_map_get_tile(map, (uint16_t)(x + 1), y)))
    mask = (path_neighbor_mask_t)(mask | PATH_NEIGHBOR_EAST);
  if (x > 0 && path_map_is_marker_path(path_map_get_tile(map, (uint16_t)(x - 1), y)))
    mask = (path_neighbor_mask_t)(mask | PATH_NEIGHBOR_WEST);

  return mask;
}

/* Use the neighbor mask as an index into the spritesheet coordinate table. */
tile_coord_t path_map_get_autotile_coords(path_neighbor_mask_t neighbor_mask)
{
  return autotile_coords[(uint8_t)neighbor_mask & 0x0F];
}

/* Perform the full autotile decision for one grid cell. */
autotile_pick_t path_map_pick_autotile(const path_map_t *map, uint16_t x, uint16_t y)
{
  path_neighbor_mask_t neighbor_mask = path_map_get_neighbor_mask(map, x, y);

  autotile_pick_t pick;
  pick.neighbor_mask = neighbor_mask;
  pick.coords = path_map_get_autotile_coords(neighbor_mask);
  return pick;
}

int path_map_load_level(path_map_t *map, const level_t *level)
{
  if (map == NULL || level == NULL || level->data == NULL || level->width == 0 || level->height == 0)
    return 1;

  /* Rebuild the map from scratch so the loader can be reused safely. */
  path_map_destroy(map);
  if (path_map_init(map, level->width, level->height) != 0)
    return 1;

  uint16_t row = 0;
  uint16_t col = 0;

  for (const char *cursor = level->data; *cursor != '\0'; cursor++)
  {
    char symbol = *cursor;

    /* Ignore Windows line endings if they show up in the level text. */
    if (symbol == '\r')
      continue;

    if (symbol == '\n')
    {
      if (col != level->width)
      {
        path_map_destroy(map);
        return 1;
      }

      row++;
      col = 0;
      continue;
    }

    if (row >= level->height || col >= level->width)
    {
      path_map_destroy(map);
      return 1;
    }

    path_tile_t tile;
    switch (symbol)
    {
      case '.':
        tile = PATH_TILE_EMPTY;
        break;

      case '#':
        tile = PATH_TILE_PATH;
        break;

      case 'S':
        tile = PATH_TILE_SPAWN;
        map->spawn_x = (int16_t)col;
        map->spawn_y = (int16_t)row;
        map->has_spawn = true;
        break;

      case 'G':
        tile = PATH_TILE_GOAL;
        map->goal_x = (int16_t)col;
        map->goal_y = (int16_t)row;
        map->has_goal = true;
        break;

      case 'T':
        tile = PATH_TILE_TOWER;
        break;

      default:
        path_map_destroy(map);
        return 1;
    }

    map->tiles[(size_t)row * map->width + col] = tile;
    col++;
  }

  /* Accept either a trailing newline or an exact final row. */
  if ((row == level->height && col == 0) ||
      (row == (uint16_t)(level->height - 1) && col == level->width))
    return 0;

  path_map_destroy(map);
  return 1;
}
