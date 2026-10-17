#include "world.h"
#include "sprite.h"

//extern Sprite *apple_sprite; 
extern Sprite *mini_apple_sprite;
extern vbe_mode_info_t modeInfo;

World* (world_create)() {
  World *world = malloc(sizeof(World));

  // Initialize from modeInfo (assuming you've set it up)
  world->blockSize = modeInfo.XResolution / 20;

  world->bounds[0].coords.x = 0;
  world->bounds[0].coords.y = 0;
  world->bounds[0].width = modeInfo.XResolution / world->blockSize;
  world->bounds[0].height = 1;

  world->bounds[1].coords.x = 0;
  world->bounds[1].coords.y = modeInfo.YResolution / world->blockSize - 1; // Bottom wall
  world->bounds[1].width = modeInfo.XResolution / world->blockSize;
  world->bounds[1].height = 1;

  world->bounds[2].coords.x = 0;
  world->bounds[2].coords.y = 0;
  world->bounds[2].width = 1;
  world->bounds[2].height = modeInfo.YResolution / world->blockSize;

  world->bounds[3].coords.x = modeInfo.XResolution / world->blockSize - 1; // Right wall
  world->bounds[3].coords.y = 0;
  world->bounds[3].width = 1;
  world->bounds[3].height = modeInfo.YResolution / world->blockSize;

  // Apple Initialization
  srand(time(NULL));

  return world;
}

void (world_respawn_apple)(World *world) {
  int maxX = modeInfo.XResolution / world->blockSize - 2;
  int maxY = modeInfo.YResolution / world->blockSize - 2;

  world->appleCoords.x = rand() % maxX + 1;
  world->appleCoords.y = rand() % maxY + 1;
}

void (world_update)(World *world, Snake *snake) {
  if (snake->snakeBody.segments[0].x == world->appleCoords.x &&
      snake->snakeBody.segments[0].y == world->appleCoords.y) {
    snake_extend(snake);
    snake_increase_score(snake);
    world_respawn_apple(world);
  }

  int gridSizeX = modeInfo.XResolution / world->blockSize;
  int gridSizeY = modeInfo.YResolution / world->blockSize;

  if (snake->snakeBody.segments[0].x <= 0 || snake->snakeBody.segments[0].y <= 0 ||
      snake->snakeBody.segments[0].x >= gridSizeX - 1  || snake->snakeBody.segments[0].y >= gridSizeY - 1) {
    snake->hasLost = true;
  }
}

void (world_render)(World* world) {

  // TODO STEP 1 IS TO BE REMOVED AND REPLACED BY XPM BACKGROUND 
  // 1. Draw background
    uint32_t color1 = 0xaad751; // Light green
    uint32_t color2 = 0xa2d149; // Dark green
    
    int numBlocksX = modeInfo.XResolution / world->blockSize;
    int numBlocksY = modeInfo.YResolution / world->blockSize;

    for (int x = 0; x < numBlocksX; x++) {
        for (int y = 0; y < numBlocksY; y++) {
            uint32_t color = ((x + y) % 2 == 0) ? color1 : color2;
            vg_draw_rectangle(x * world->blockSize, y * world->blockSize, 
                              world->blockSize, world->blockSize, color);
        }
    }

  // 2. Render Walls
  for (int i = 0; i < 4; i++) {
    vg_draw_rectangle(
      world->bounds[i].coords.x * world->blockSize,
      world->bounds[i].coords.y * world->blockSize,
      world->bounds[i].width * world->blockSize,
      world->bounds[i].height * world->blockSize,
      0x578a34);
  }

  // 3. Render Apple

  create_sprites();
  draw_sprite(mini_apple_sprite, world->appleCoords.x * world->blockSize, world->appleCoords.y * world->blockSize);
  //render_apple(world->appleCoords.x * world->blockSize, world->appleCoords.y * world->blockSize);  
  /*
  vg_draw_rectangle(
    world->appleCoords.x * world->blockSize,
    world->appleCoords.y * world->blockSize,
    world->blockSize, world->blockSize,
    0x00FFFF);
    */
}
