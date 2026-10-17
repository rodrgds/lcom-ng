/**
 * @file grid.h
 * @brief This file contains the function prototypes and constants for the grid module. It includes functions to initialize, draw, paint cells, and clear the grid. It also includes functions to get cell information and compare the grid state.
 */

#ifndef _GRID_H_
#define _GRID_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <lcom/lcf.h>

#define GRID_ROWS 10
#define GRID_COLS 10
#define CELL_SIZE 32
#define GRID_WIDTH (GRID_COLS * CELL_SIZE)
#define GRID_HEIGHT (GRID_ROWS * CELL_SIZE)

extern uint16_t x_resolution;
extern uint16_t y_resolution;
extern uint16_t GRID_ORIGIN_X;
extern uint16_t GRID_ORIGIN_Y;

extern uint32_t original_grid[GRID_ROWS][GRID_COLS];

/**
 * @brief Initializes the grid by setting the origin coordinates.
 */
void grid_init();

/**
 * @brief Creates a cell template filled with the specified color.
 * 
 * @param color The color to fill the cell template with.
 */
void create_cell_template(uint32_t color);

/**
 * @brief Draws a cell at the specified coordinates with a given color using an optimized method.
 * 
 * @param x The x-coordinate of the cell.
 * @param y The y-coordinate of the cell.
 * @param color The color to fill the cell with.
 */
void draw_cell_optimized(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws the grid outline on the screen.
 */
void grid_draw();

/**
 * @brief Draws the original drawing on the grid.
 */
void draw_original_drawing();

/**
 * @brief Clears the grid by removing the drawing.
 */
void grid_clear();

/**
 * @brief Paints a cell in the grid with the specified color.
 * 
 * @param row The row index of the cell to be painted.
 * @param col The column index of the cell to be painted.
 * @param color The color to paint the cell with.
 * @return 0 on success, non-zero otherwise.
 */
int grid_paint_cell(uint8_t row, uint8_t col, uint32_t color);

/**
 * @brief Gets the cell coordinates, row and column, based on the x and y screen coordinates.
 * 
 * @param x The x-coordinate on the screen.
 * @param y The y-coordinate on the screen.
 * @param row Pointer to store the row index.
 * @param col Pointer to store the column index.
 * @return 0 on success, non-zero otherwise. 
 */
int get_cell(uint16_t x, uint16_t y, uint8_t *row, uint8_t *col);

/**
 * @brief Compares the current grid state with the original grid state.
 * 
 * @return true if the grids are the same, false otherwise.
 */
bool compare_grid();

/**
 * @brief Frees memory allocated for grid cell templates.
 */
void grid_cleanup();

#endif /* _GRID_H_ */
