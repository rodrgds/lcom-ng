/**
 * @file enemy_path.c
 * @brief Path computation -- scans tile grid and produces ordered waypoint sequence.
 * @ingroup group_enemies
 */

#include "enemy_path.h"

#include <stdlib.h>

#include "path_map.h"

static bool is_traversable(path_tile_t tile)
{
  return tile == PATH_TILE_PATH || tile == PATH_TILE_SPAWN || tile == PATH_TILE_GOAL;
}

bool enemy_path_compute(const path_map_t *map,
                        int16_t *waypoints_x, int16_t *waypoints_y,
                        uint16_t *waypoint_count, uint16_t max_waypoints)
{
  if (map == NULL || waypoints_x == NULL || waypoints_y == NULL || waypoint_count == NULL)
    return false;

  if (!map->has_spawn || !map->has_goal)
    return false;

  uint16_t w = map->width;
  uint16_t h = map->height;
  uint32_t cell_count = (uint32_t)w * h;

  bool *visited = calloc(cell_count, sizeof(bool));
  int16_t *prev_x = malloc(cell_count * sizeof(int16_t));
  int16_t *prev_y = malloc(cell_count * sizeof(int16_t));
  int16_t *queue_x = malloc(cell_count * sizeof(int16_t));
  int16_t *queue_y = malloc(cell_count * sizeof(int16_t));

  if (visited == NULL || prev_x == NULL || prev_y == NULL || queue_x == NULL || queue_y == NULL)
  {
    free(visited);
    free(prev_x);
    free(prev_y);
    free(queue_x);
    free(queue_y);
    return false;
  }

  for (uint32_t i = 0; i < cell_count; i++)
  {
    prev_x[i] = -1;
    prev_y[i] = -1;
  }

  uint16_t head = 0;
  uint16_t tail = 0;

  int16_t spawn_idx = (int16_t)((uint32_t)map->spawn_y * w + map->spawn_x);
  visited[spawn_idx] = true;
  queue_x[tail] = map->spawn_x;
  queue_y[tail] = map->spawn_y;
  tail++;

  bool found = false;
  int16_t goal_x = map->goal_x;
  int16_t goal_y = map->goal_y;

  while (head < tail)
  {
    int16_t cx = queue_x[head];
    int16_t cy = queue_y[head];
    head++;

    if (cx == goal_x && cy == goal_y)
    {
      found = true;
      break;
    }

    static const int16_t dirs[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
    for (int d = 0; d < 4; d++)
    {
      int16_t nx = (int16_t)(cx + dirs[d][0]);
      int16_t ny = (int16_t)(cy + dirs[d][1]);

      if (nx < 0 || nx >= (int16_t)w || ny < 0 || ny >= (int16_t)h)
        continue;

      if (!is_traversable(path_map_get_tile(map, (uint16_t)nx, (uint16_t)ny)))
        continue;

      uint32_t ni = (uint32_t)ny * w + (uint32_t)nx;
      if (visited[ni])
        continue;

      visited[ni] = true;
      prev_x[ni] = cx;
      prev_y[ni] = cy;
      queue_x[tail] = nx;
      queue_y[tail] = ny;
      tail++;
    }
  }

  free(visited);
  free(queue_x);
  free(queue_y);

  if (!found)
  {
    free(prev_x);
    free(prev_y);
    return false;
  }

  int16_t path_x[MAX_WAYPOINTS];
  int16_t path_y[MAX_WAYPOINTS];
  uint16_t path_len = 0;

  int16_t cx = goal_x;
  int16_t cy = goal_y;
  while (cx >= 0 && cy >= 0)
  {
    uint32_t idx = (uint32_t)cy * w + (uint32_t)cx;

    path_x[path_len] = cx;
    path_y[path_len] = cy;
    path_len++;

    if (path_len >= 512)
    {
      free(prev_x);
      free(prev_y);
      return false;
    }

    int16_t px = prev_x[idx];
    int16_t py = prev_y[idx];
    if (px == cx && py == cy)
      break;
    cx = px;
    cy = py;
  }

  free(prev_x);
  free(prev_y);

  if (path_len > max_waypoints)
    return false;

  for (uint16_t i = 0; i < path_len; i++)
  {
    uint16_t ri = (uint16_t)(path_len - 1 - i);
    waypoints_x[i] = (int16_t)(ENEMY_PATH_ORIGIN_X + path_x[ri] * ENEMY_PATH_TILE_SIZE + ENEMY_PATH_TILE_SIZE / 2);
    waypoints_y[i] = (int16_t)(ENEMY_PATH_ORIGIN_Y + path_y[ri] * ENEMY_PATH_TILE_SIZE + ENEMY_PATH_TILE_SIZE / 2);
  }

  *waypoint_count = path_len;
  return true;
}
