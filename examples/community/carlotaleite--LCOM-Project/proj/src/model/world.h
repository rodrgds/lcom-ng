#pragma once

#include "snake.h" // Assuming you have your Snake in a separate C file
#include "controller/gpu.h"
#include <lcom/lcf.h>
#include <stdio.h> // For input/output potentially

typedef struct Coord {
  uint8_t x;
  uint8_t y;
} Coord;

typedef struct Wall {
  Coord coords;
  int width;
  int height;
} Wall;

typedef struct World {
  Wall bounds[4];
  Coord appleCoords;
  int blockSize;
} World;

World *(world_create)();

void (world_respawn_apple)(World *world);
void (world_update)(World *world, Snake *player);
void (world_render)(World *world);
