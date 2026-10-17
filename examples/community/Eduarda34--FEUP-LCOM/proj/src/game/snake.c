#include <lcom/lcf.h>
#include "snake.h"
#include "../controller/video/colors.h"
Point *snake_body = NULL; // Snake body segments
int snake_length = INITIAL_SNAKE_LENGTH;

Direction current_direction = UP;
Direction next_direction = UP;

extern vbe_mode_info_t mode_info;


void snake_init(uint16_t screen_width, uint16_t screen_height) {
    // Initialize the snake's body and position
    snake_length = INITIAL_SNAKE_LENGTH;
    snake_body = malloc(snake_length * sizeof(Point));
    if (snake_body == NULL) {
        printf("Failed to allocate memory for snake\n");
        return;
    }

    // Calculate initial snake position to be grid-aligned
    uint16_t start_grid_x = (screen_width / 2) / CELL_SIZE;
    uint16_t start_grid_y = (screen_height / 2) / CELL_SIZE;

    uint16_t start_x = start_grid_x * CELL_SIZE; // Align to grid
    uint16_t start_y = start_grid_y * CELL_SIZE; // Start at the vertical center

    // Initialize the snake's body segments
    for (int i = 0; i < snake_length; i++) {
        snake_body[i].x = start_x;
        snake_body[i].y = start_y - i * CELL_SIZE; // Add spacing
    }

    current_direction = UP;
    next_direction = UP;
}

void snake_draw() {
    // Draw the snake's body segments
    for (int i = 0; i < snake_length; i++) {
        if (vg_draw_rectangle(snake_body[i].x, snake_body[i].y, SEGMENT_SIZE, SEGMENT_SIZE, COLOR_GREEN) != 0) {
            printf("Failed to draw snake segment\n");
            return;
        }
    }
}

void snake_update() {
    for (int i = snake_length - 1; i > 0; i--) {
        snake_body[i] = snake_body[i - 1];
    }

    current_direction = next_direction;

    switch (current_direction) {
        case UP:
            snake_body[0].y = (snake_body[0].y == 0) 
                ? mode_info.YResolution - CELL_SIZE 
                : snake_body[0].y - CELL_SIZE;
            break;
        case DOWN:
            snake_body[0].y = (snake_body[0].y + CELL_SIZE >= mode_info.YResolution) 
                ? 0 
                : snake_body[0].y + CELL_SIZE;
            break;
        case LEFT:
            snake_body[0].x = (snake_body[0].x == 0) 
                ? mode_info.XResolution - CELL_SIZE 
                : snake_body[0].x - CELL_SIZE;
            break;
        case RIGHT:
            snake_body[0].x = (snake_body[0].x + CELL_SIZE >= mode_info.XResolution) 
            ? 0 
            : snake_body[0].x + CELL_SIZE;
            break;
    }
}

void snake_change_direction(Direction dir) {
    next_direction = dir;
}

bool snake_check_collision_with_self() {
    if (snake_length >= 4) {
        for (int i = 1; i < snake_length; i++) {
            if (snake_body[i].x == snake_body[0].x && snake_body[i].y == snake_body[0].y) {
                return true; // Collision with itself
            }
        }
        return false;
    }
    return false; // No collision if snake length is less than 4
}

bool snake_check_collision_with_apple(Point apple) {
    return (snake_body[0].x == apple.x && snake_body[0].y == apple.y);
}

Point snake_get_head_position() {
    return snake_body[0];
}

void snake_grow() {
    Point *new_body = realloc(snake_body, (snake_length + 1) * sizeof(Point));
    if (new_body) {
        snake_body = new_body;
        snake_body[snake_length] = snake_body[snake_length - 1]; // Duplicate last segment
        snake_length++;
    } else {
        printf("Failed to grow snake\n");
        return;
    }
}

void snake_free() {
    // Free the allocated memory for the snake's body
    if (snake_body != NULL) {
        free(snake_body);
        snake_body = NULL;
    }
    snake_length = 0;
}

void snake_handle_mouse_movement(int16_t delta_x, int16_t delta_y) {
// Apply a threshold to avoid noise
  int threshold = 5;

  if (abs(delta_x) > abs(delta_y)) {
    if (delta_x > threshold && current_direction != LEFT)
      snake_change_direction(RIGHT);
    else if (delta_x < -threshold && current_direction != RIGHT)
      snake_change_direction(LEFT);
  } else {
    if (delta_y < -threshold && current_direction != UP)
      snake_change_direction(DOWN);
    else if (delta_y > threshold && current_direction != DOWN)
      snake_change_direction(UP);
  }
}
