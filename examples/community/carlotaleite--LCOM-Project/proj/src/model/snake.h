#pragma once
#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdio.h>

// #define SNAKE_POS_EQUALS(s1, s2) (s1.x == s2.x && s1.y == s2.y)

typedef struct SnakeSegment {
  uint8_t x;
  uint8_t y;
} SnakeSegment;

typedef enum Direction {
  None,
  Up,
  Down,
  Left,
  Right
} Direction;

typedef struct SnakeContainer {
  SnakeSegment *segments;
  int size;
  int capacity;
} SnakeContainer;

typedef struct Snake {
  SnakeContainer snakeBody;
  int blockSize;
  Direction dir;
  int speed;
  int lives;
  int score;
  bool hasLost;
} Snake;

// Function Prototypes
Snake *(snake_create)();
void (destroy_snake)(Snake *snake);

Direction (snake_get_physical_direction)(Snake *snake);
void (snake_increase_score)(Snake *snake);

void (snake_extend)(Snake *snake);
void (snake_reset)(Snake *snake);
void (snake_move)(Snake *snake);
void (snake_tick)(Snake *snake);
void (snake_cut)(Snake *snake, int segments);
void (snake_check_collision)(Snake *snake);

void (snake_render)(Snake *snake);
