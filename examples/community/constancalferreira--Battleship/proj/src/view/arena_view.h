/**
 * @file arena_view.h
 * @brief This file contains functions responsible for drawing and rendering elements
 * related to the game arena, including the background, grid, ships, and markers.
 */

#ifndef ARENA_VIEW_H
#define ARENA_VIEW_H

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "controller/graphics.h"
#include "model/game_macro.h"
#include "model/game.h"
#include "model/sprite.h"
#include "model/arena.h"
#include "view/sprite_view.h"

/**
 * @brief Draws the background of the arena.
 * @param buffer Pointer to the graphics buffer where the background will be drawn.
 */
void draw_arena_background(uint8_t *buffer);

/**
 * @brief Draws the entire arena, including the background and grid.
 */
void draw_arena();

/**
 * @brief Draws the layout of the arena, such as borders or static elements.
 */
void draw_layout();

/**
 * @brief Draws the game grid.
 * @param grid Pointer to the Grid structure containing the grid data.
 * @param hide_ships A boolean indicating whether ships on the grid should be hidden (e.g., for opponent's grid).
 */
void draw_grid(Grid *grid, bool hide_ships);

/**
 * @brief Draws a single cell of the grid.
 * @param grid Pointer to the Grid structure.
 * @param x X-coordinate (pixel) of the cell's top-left corner.
 * @param y Y-coordinate (pixel) of the cell's top-left corner.
 * @param cell_row Row index of the cell in the grid.
 * @param cell_col Column index of the cell in the grid.
 * @param hovered_ship_id The ID of the ship currently being hovered over, or -1 if none.
 * @param hide_ships A boolean indicating whether ships in the cell should be hidden.
 */
void draw_cell(Grid *grid, int x, int y, int cell_row, int cell_col, int hovered_ship_id, bool hide_ships);

/**
 * @brief Draws the status of ships (e.g., hit points, destroyed status).
 * @param grid Pointer to the Grid structure.
 * @param start_x X-coordinate (pixel) where ship status drawing should begin.
 * @param start_y Y-coordinate (pixel) where ship status drawing should begin.
 */
void draw_ship_status(Grid *grid, int start_x, int start_y);

/**
 * @brief Draws "guide" ships, often used for placement previews.
 */
void draw_guide_ships();

/**
 * @brief Draws a ship sprite.
 * @param x X-coordinate (pixel) for the ship's top-left corner.
 * @param y Y-coordinate (pixel) for the ship's top-left corner.
 * @param type The type of the ship to draw.
 * @param orientation The orientation of the ship (e.g., horizontal, vertical).
 * @param hoover A boolean indicating if the ship is currently being hovered over (for visual feedback).
 */
void draw_ship_sprite(int x, int y,  ShipType type, int orientation, bool hoover);

/**
 * @brief Draws a marker indicating a successful hit.
 * @param x X-coordinate (pixel) for the marker.
 * @param y Y-coordinate (pixel) for the marker.
 */
void draw_hit_marker(int x, int y);

/**
 * @brief Draws a marker indicating a missed shot.
 * @param x X-coordinate (pixel) for the marker.
 * @param y Y-coordinate (pixel) for the marker.
 */
void draw_miss_marker(int x, int y);

/**
 * @brief Draws visual elements indicating the current player's turn.
 * @param player The PlayerTurn enum value indicating whose turn it is.
 */
void draw_player(PlayerTurn player);

#endif
