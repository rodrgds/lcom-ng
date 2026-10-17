#include <lcom/lcf.h>
#include "apple.h"
#include "controller/video/colors.h"
extern vbe_mode_info_t mode_info;

Point apple_position;
bool apple_eaten = true;

#define CELL_SIZE (SEGMENT_SIZE + SEGMENT_SPACING)

// Generate a random apple position not overlapping with the snake
void generate_apple_position() {
    int grid_width = mode_info.XResolution / CELL_SIZE;
    int grid_height = mode_info.YResolution / CELL_SIZE;

    while (true) {
        int gx = rand() % grid_width;
        int gy = rand() % grid_height;

        apple_position.x = gx * CELL_SIZE;
        apple_position.y = gy * CELL_SIZE;

        bool overlap = false;
        for (int i = 0; i < snake_length; i++) {
            if (snake_body[i].x == apple_position.x &&
                snake_body[i].y == apple_position.y) {
                overlap = true;
                break;
            }
        }

        if (!overlap)
            break;
    }
}

void draw_apple() {
    vg_draw_xpm(apple, apple_position.x, apple_position.y);
}

void clear_apple() {
    vg_draw_rectangle(apple_position.x, apple_position.y, SEGMENT_SIZE, SEGMENT_SIZE, COLOR_BLACK);
}
