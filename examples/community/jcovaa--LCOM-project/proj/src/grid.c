#include "grid.h"
#include "drivers/video/graphics.h"

extern unsigned bytes_per_pixel;
extern uint8_t *img_buffer;

static uint8_t *cell_template = NULL;
static uint32_t template_color = 0;

uint16_t GRID_ORIGIN_X;
uint16_t GRID_ORIGIN_Y;
uint32_t grid[GRID_ROWS][GRID_COLS];
uint32_t original_grid[GRID_ROWS][GRID_COLS];

void grid_init() {
  GRID_ORIGIN_X = (x_resolution - GRID_WIDTH) / 2;
  GRID_ORIGIN_Y = (y_resolution - GRID_HEIGHT) / 2;

  // Allocate memory for cell template
  if (!cell_template) {
    cell_template = malloc(CELL_SIZE * CELL_SIZE * bytes_per_pixel);
  }

  for (uint8_t row = 0; row < GRID_ROWS; row++) {
    for (uint8_t col = 0; col < GRID_COLS; col++) {
      grid[row][col] = 0xFFFFFF; 
    }
  }
}

void create_cell_template(uint32_t color) {
  if (template_color == color && cell_template) {
    return; // Already have this template
  }
  
  // Fill the template with the color
  for (int i = 0; i < CELL_SIZE * CELL_SIZE; i++) {
    memcpy(&cell_template[i * bytes_per_pixel], &color, bytes_per_pixel);
  }
  template_color = color;
}

void draw_cell_optimized(uint16_t x, uint16_t y, uint32_t color) {
  create_cell_template(color);
  
  // Copy the template row by row to the frame buffer
  for (int row = 0; row < CELL_SIZE; row++) {
    unsigned int dest_index = ((y + row) * x_resolution + x) * bytes_per_pixel;
    unsigned int src_index = row * CELL_SIZE * bytes_per_pixel;
    memcpy(&img_buffer[dest_index], &cell_template[src_index], CELL_SIZE * bytes_per_pixel);
  }
}

void grid_draw() {
  for (uint8_t row = 0; row < GRID_ROWS; row++) {
    for (uint8_t col = 0; col < GRID_COLS; col++) {
      uint16_t x = GRID_ORIGIN_X + col * CELL_SIZE;
      uint16_t y = GRID_ORIGIN_Y + row * CELL_SIZE;
      draw_cell_optimized(x, y, grid[row][col]);
    }
  }
  draw_grid_outline(GRID_ROWS, GRID_COLS, CELL_SIZE, CELL_SIZE, GRID_ORIGIN_X, GRID_ORIGIN_Y, 0x000000); 
}

void draw_original_drawing() {
  for (uint8_t row = 0; row < GRID_ROWS; row++) {
    for (uint8_t col = 0; col < GRID_COLS; col++) {
      uint16_t x = GRID_ORIGIN_X + col * CELL_SIZE;
      uint16_t y = GRID_ORIGIN_Y + row * CELL_SIZE;
      draw_cell_optimized(x, y, original_grid[row][col]);
    }
  }
}

void grid_clear() {
  for (uint8_t row = 0; row < GRID_ROWS; row++) {
    for (uint8_t col = 0; col < GRID_COLS; col++) {
      grid[row][col] = 0xFFFFFF; 
    }
  }

  for (uint8_t row = 0; row < GRID_ROWS; row++) {
    for (uint8_t col = 0; col < GRID_COLS; col++) {
      uint16_t x = GRID_ORIGIN_X + col * CELL_SIZE;
      uint16_t y = GRID_ORIGIN_Y + row * CELL_SIZE;
      vg_draw_rectangle(x, y, CELL_SIZE, CELL_SIZE, 0xFFFFFF);
    }
  }
}

int grid_paint_cell(uint8_t row, uint8_t col, uint32_t color) {
  if (row >= GRID_ROWS || col >= GRID_COLS) {
    return 1;
  }

  uint16_t x = GRID_ORIGIN_X + col * CELL_SIZE;
  uint16_t y = GRID_ORIGIN_Y + row * CELL_SIZE;

  draw_cell_optimized(x, y, color);
  grid[row][col] = color;

  return 0;
}

int get_cell(uint16_t x, uint16_t y, uint8_t *row, uint8_t *col) {
  if (x < GRID_ORIGIN_X || y < GRID_ORIGIN_Y || x >= GRID_ORIGIN_X + GRID_WIDTH || y >= GRID_ORIGIN_Y + GRID_HEIGHT) {
    *row = *col = UINT8_MAX;
    return 1;
  }

  *col = (x - GRID_ORIGIN_X) / CELL_SIZE;
  *row = (y - GRID_ORIGIN_Y) / CELL_SIZE;

  return 0;
}

bool compare_grid() {
  for (int row = 0; row < GRID_ROWS; row++) {
    for (int col = 0; col < GRID_COLS; col++) {
      if (grid[row][col] != original_grid[row][col]) {
        return false;
      }
    }
  }
  
  return true;
}

void grid_cleanup() {
  if (cell_template) {
    free(cell_template);
    cell_template = NULL;
  }
}
