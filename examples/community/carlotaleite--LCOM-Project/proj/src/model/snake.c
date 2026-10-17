#include "snake.h"
#include <stdint.h>

extern vbe_mode_info_t modeInfo;

Snake *(snake_create) () {
  Snake *snake = malloc(sizeof(Snake));
  snake->blockSize = modeInfo.XResolution / 20;
  if (!snake) {
    fprintf(stderr, "Memory allocation error!\n");
    return NULL;
  }

  snake->snakeBody.capacity = 10;
  snake->snakeBody.size = 3; // Initial capacity
  snake->snakeBody.segments = malloc(snake->snakeBody.size * sizeof(SnakeSegment));
  if (!snake->snakeBody.segments) {
    fprintf(stderr, "Memory allocation error!\n");
    free(snake);
    return NULL;
  }

  snake_reset(snake);
  return snake;
}

void(destroy_snake)(Snake *snake) {
  if (snake) { // Check if the snake pointer is valid
    if (snake->snakeBody.segments) {
      free(snake->snakeBody.segments); // Free the segments array
    }
    free(snake); // Free the Snake struct itself
  }
}

void(snake_reset)(Snake *snake) {

  if (snake->snakeBody.segments) {
    free(snake->snakeBody.segments);
    snake->snakeBody.segments = NULL;
    snake->snakeBody.size = 0;
    snake->snakeBody.capacity = 0;
  }

  snake->snakeBody.capacity = 10;
  snake->snakeBody.segments = malloc(snake->snakeBody.capacity * sizeof(SnakeSegment));
  if (!snake->snakeBody.segments) {
    fprintf(stderr, "Memory allocation error!\n");
    return;
  }
  snake->snakeBody.size = 3; // Set the number of segments

  snake->snakeBody.segments[0].x = 5;
  snake->snakeBody.segments[0].y = 7;
  snake->snakeBody.segments[1].x = 5;
  snake->snakeBody.segments[1].y = 6;
  snake->snakeBody.segments[2].x = 5;
  snake->snakeBody.segments[2].y = 5;

  // Reset other Snake properties
  snake->dir = None;
  snake->speed = 5;
  snake->lives = 3;
  snake->score = 0;
  snake->hasLost = false;
}

Direction(snake_get_physical_direction)(Snake *snake) {
  if (snake->snakeBody.size <= 1) {
    return None; // No meaningful direction if the snake has only one segment
  }

  // Access the head and tail of the snake
  SnakeSegment head = snake->snakeBody.segments[0];
  SnakeSegment tail = snake->snakeBody.segments[1];

  // Determine direction based on head and tail positions
  if (head.x == tail.x) {
    return head.y > tail.y ? Down : Up;
  }
  else if (head.y == tail.y) {
    return head.x > tail.x ? Right : Left;
  }
  else {
    return None;
  }
}

void(snake_increase_score)(Snake *snake) {
  snake->score += 10;
}

void(snake_extend)(Snake *snake) {
  // Check if we need to reallocate for more capacity
  if (snake->snakeBody.size == snake->snakeBody.capacity) {
    snake->snakeBody.capacity += 20; // Double the capacity
    snake->snakeBody.segments = realloc(snake->snakeBody.segments,
                                        snake->snakeBody.capacity * sizeof(SnakeSegment));
    if (!snake->snakeBody.segments) {
      fprintf(stderr, "Memory allocation error!\n");
      return;
    }
  }

  // Add the new segment at the tail.
  SnakeSegment *tail = &snake->snakeBody.segments[snake->snakeBody.size - 1];
  SnakeSegment *newTail = &snake->snakeBody.segments[snake->snakeBody.size]; // Next free slot

  if (snake->dir == Up) {
    newTail->x = tail->x;
    newTail->y = tail->y + 1;
  }
  else if (snake->dir == Down) {
    newTail->x = tail->x;
    newTail->y = tail->y - 1;
  }
  else if (snake->dir == Left) {
    newTail->x = tail->x + 1;
    newTail->y = tail->y;
  }
  else if (snake->dir == Right) {
    newTail->x = tail->x - 1;
    newTail->y = tail->y;
  }

  snake->snakeBody.size++; // Increment the number of segments
}

void(snake_move)(Snake *snake) {
  // Move existing segments (tail to head)
  for (int i = snake->snakeBody.size - 1; i > 0; i--) {
    snake->snakeBody.segments[i] = snake->snakeBody.segments[i - 1];
  }

  // Move the head based on direction
  if (snake->dir == Up) {
    snake->snakeBody.segments[0].y--;
  }
  else if (snake->dir == Down) {
    snake->snakeBody.segments[0].y++;
  }
  else if (snake->dir == Left) {
    snake->snakeBody.segments[0].x--;
  }
  else if (snake->dir == Right) {
    snake->snakeBody.segments[0].x++;
  }
}

void(snake_tick)(Snake *snake) {
  if (snake->dir == None || snake->hasLost) {
    return;
  }

  snake_move(snake);
  snake_check_collision(snake);
}

void(snake_cut)(Snake *snake, int segmentsToRemove) {
  if (segmentsToRemove >= snake->snakeBody.size)
    return;

  snake->snakeBody.size -= segmentsToRemove;

  if (--snake->lives == 0) {
    snake->hasLost = true;
  }
}

void(snake_check_collision)(Snake *snake) {
  if (snake->snakeBody.size < 5)
    return;

  SnakeSegment head = snake->snakeBody.segments[0];

  for (int i = 1; i < snake->snakeBody.size; i++) {
    if (head.x == snake->snakeBody.segments[i].x &&
        head.y == snake->snakeBody.segments[i].y) {
      int segmentsToRemove = snake->snakeBody.size - i;
      snake_cut(snake, segmentsToRemove);
      break;
    }
  }
}

void(snake_render)(Snake *snake) {
  uint32_t headColor = 0x1c469e; // Green
  uint32_t bodyColor = 0x4371e5; // Yellow

  for (int i = 0; i < snake->snakeBody.size; i++) {
    SnakeSegment *segment = &snake->snakeBody.segments[i];
    uint32_t color = (i == 0) ? headColor : bodyColor;

    int smol = 2;
    vg_draw_rectangle(segment->x * snake->blockSize + smol, segment->y * snake->blockSize + smol, snake->blockSize - 2 * smol, snake->blockSize - 2 * smol, color);
  }
}
