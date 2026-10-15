#ifndef MAP_H
#define MAP_H

#include <lcom/lcf.h>
#include "../i8042.h"
#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <lcom/timer.h>

/** @file map.h
 *  @brief Header file for the Minesweeper game map module.
 *
 *  This file contains the definition of the Map and Cell structures, as well as
 *  the function prototypes for manipulating the game map, including creating,
 *  destroying, revealing cells, and flagging cells. It also includes functions
 *  for handling the deceiver power-up and ensuring the first move is safe.
 */

#define CELL_SIZE 30

/**
 * @brief Represents a single cell in the Minesweeper grid.
 */
typedef struct {
    bool is_bomb;               /**< True if the cell contains a bomb. */
    bool is_exploded;           /**< True if the bomb in the cell has exploded. */
    bool is_revealed;           /**< True if the cell has been revealed. */
    bool is_flagged;            /**< True if the cell is flagged by the player. */
    unsigned int adjacent_bombs; /**< Number of adjacent bombs (0-8). Ignored if the cell is a bomb. */
} Cell;

/**
 * @brief Represents the entire Minesweeper map.
 */
typedef struct {
    Cell **cells;               /**< 2D matrix of cells. */
    unsigned int rows;          /**< Number of rows in the map. */
    unsigned int cols;          /**< Number of columns in the map. */
    unsigned int total_bombs;   /**< Total number of bombs on the map. */
    unsigned int start_x;       /**< Starting X position (used for map positioning). */
    unsigned int start_y;       /**< Starting Y position (used for map positioning). */
    unsigned int flags_used;    /**< Number of flags currently placed by the player. */
    unsigned int revealed_cells;/**< Number of cells that have been revealed. */
    bool lamp_used;             /**< True if the lamp power-up has been used. */
} Map;

/**
 * @brief Global pointer to the current game map.
 */
extern Map* map;

/**
 * @brief Creates a new Minesweeper map with specified parameters.
 * 
 * @param rows Number of rows in the map.
 * @param cols Number of columns in the map.
 * @param bombs Number of bombs to place on the map.
 * @param start_x Starting X position (for display purposes).
 * @param start_y Starting Y position (for display purposes).
 * @return Pointer to the created Map structure.
 */
Map* (create_map)(unsigned int rows, unsigned int cols, unsigned int bombs, unsigned int start_x, unsigned int start_y);

/**
 * @brief Frees all memory allocated for the map.
 * 
 * @param map Pointer to the Map to be destroyed.
 */
void (destroy_map)(Map *map);

/**
 * @brief Reveals the cell at the specified position.
 * 
 * @param map Pointer to the Map.
 * @param row Row index of the cell.
 * @param col Column index of the cell.
 */
void (map_reveal_cell)(Map *map, unsigned int row, unsigned int col);

/**
 * @brief Reveals all bomb-containing cells on the map.
 * 
 * @param map Pointer to the Map.
 */
void (map_reveal_all_bombs)(Map *map);

/**
 * @brief Flags or unflags the cell at the specified position.
 * 
 * @param map Pointer to the Map.
 * @param row Row index of the cell.
 * @param col Column index of the cell.
 */
void (map_flag_cell)(Map *map, unsigned int row, unsigned int col);

/**
 * @brief Reveals a random unrevealed cell on the map.
 * 
 * @param map Pointer to the Map.
 */
void (reveal_random_cell)(Map *map);

/**
 * @brief Alters a random non-bomb cell to a bomb (for deceiver effect).
 * 
 * @param map Pointer to the Map.
 */
void (deceiver_change_cell)(Map *map);

/**
 * @brief Performs the first cell reveal of the game, ensuring the first move is safe.
 * 
 * @param map Pointer to the Map.
 * @param row Row index of the first revealed cell.
 * @param col Column index of the first revealed cell.
 */
void (map_first_reveal)(Map *map, unsigned int row, unsigned int col);

#endif

