#ifndef MAZE_H
#define MAZE_H

#include <lcom/lcf.h>
#include <stdlib.h>

#include "assets/xpm/level/key.xpm"
#include "assets/xpm/level/wall.xpm"
#include "config.h"
#include "drivers/video/gpu.h"
#include "model/utils/animated_sprite.h"
#include "model/utils/sprite.h"

typedef enum {
    EMPTY, /**< Represents an empty cell in the maze. */
    WALL,  /**< Represents a wall cell in the maze. */
    MOB,   /**< Represents a mob cell in the maze. */
    WIN,   /**< Represents the winning cell of the maze. */
} maze_entity;

#define MAZE_OPENING_PERCENTAGE 20 /**< Maze generated does not look like an arena so we generate it normally then remove random walls until we removed this percentage. */
#define NO_MONSTER_SQUARE 2        /**< No mobs spawn at this manhattan distance from the player spawn. */
#define MOB_MULTIPLIER 4           /**< Scales the number of mobs in the maze. */
#define MAX_ATTEMPTS 1000          /**< max attmpts for loops with random. */

extern uint8_t *secondary_frame_buffer; /**< Pointer to the secondary framebuffer */
extern uint32_t frame_size;             /**< Size of the framebuffer */
extern uint16_t x_res;                  /**< X resolution of the screen */
extern uint16_t y_res;                  /**< Y resolution of the screen */

/**
 * @brief Represents a single Maze instance.
 *
 * The Maze struct is opaque.
 */
typedef struct Maze Maze;

/**
 * @brief Represents a single Point instance.
 */
typedef struct Point {
    int x; /**< X-coordinate of the point */
    int y; /**< Y-coordinate of the point */
} Point;

/**
 * @brief Creates and initializes a new maze with the given dimensions.
 *
 * @param width Width of the maze in cells.
 * @param height Height of the maze in cells.
 * @param mob_count Number of mobs in the maze.
 * @return Pointer to the created Maze structure.
 */
Maze *create_maze(uint8_t width, uint8_t height, uint8_t mob_count);

/**
 * @brief Frees the memory associated with the maze.
 *
 * @param maze Pointer to the Maze structure to free.
 * @return 0 for success, 1 for failure.
 */
int destroy_maze(Maze *maze);

/**
 * @brief Returns the width of the maze in cells.
 *
 * @param maze Pointer to the Maze structure.
 * @return Width of the maze.
 */
uint8_t get_width(Maze *maze);

/**
 * @brief Returns the height of the maze in cells.
 *
 * @param maze Pointer to the Maze structure.
 * @return Height of the maze.
 */
uint8_t get_height(Maze *maze);

/**
 * @brief Returns the number of lines in the maze (used for rendering and collision).
 *
 * @param maze Pointer to the Maze structure.
 * @return Number of lines in the maze.
 */
int get_line_count(Maze *maze);

/**
 * @brief Returns the number of mobs in the maze.
 *
 * @param maze Pointer to the Maze structure.
 * @return Number of mobs in the maze.
 */
uint8_t get_mob_count(Maze *maze);

/**
 * @brief Sets the number of mobs in the maze.
 *
 * @param maze Pointer to the Maze structure.
 */
void set_mob_count(Maze *maze, uint8_t mob_count);

/**
 * @brief Returns the positions in the maze where there are mobs.
 *
 * @param maze Pointer to the Maze structure.
 * @return Pointer to array of mob Position pointers;
 */
Point **get_mob_positions(Maze *maze);

/**
 * @brief Returns the sprite of the key.
 *
 * @param maze Pointer to the Maze structure.
 * @return Pointer to the Sprite structure representing the key.;
 */
Sprite *get_key_sprite(Maze *maze);

/**
 * @brief Checks for collision between two rectangles.
 *
 * @param x1 First rectangle top left x.
 * @param y1 First rectangle top left y.
 * @param width1 First rectangle width.
 * @param height1 First rectangle height.
 * @param x2 Second rectangle top left x.
 * @param y2 Second rectangle top left y.
 * @param width1 Second rectangle width.
 * @param height1 Second rectangle height.
 * @return true if there is a collision, false otherwise.
 */
bool check_rectangle_collision(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2);

/**
 * @brief Checks for collision between two sprites.
 *
 * @param a Sprite a.
 * @param b Sprite b.
 * @return true if there is a collision, false otherwise.
 */
bool check_sprite_collision(Sprite *a, Sprite *b);

/**
 * @brief Checks for collision between a sprite and the maze walls.
 *
 * @param maze Pointer to the Maze structure.
 * @param x X-coordinate of the top-left corner of the rectangle.
 * @param y Y-coordinate of the top-left corner of the rectangle.
 * @param sprite Pointer to the Sprite structure.
 * @return true if there is a collision, false otherwise.
 */
bool check_wall_collision(Maze *maze, Sprite *sprite);

/**
 * @brief Draws the maze to the provided frame buffer.
 *
 * @param maze Pointer to the Maze structure to draw.
 * @param frame_buffer Pointer to the frame buffer.
 * @return 0 on success, non-zero on failure.
 */
int draw_maze(Maze *maze, uint8_t *frame_buffer);

/**
 * @brief Draws the not maze part of the screen to the provided frame buffer.
 *
 * @param maze Pointer to the Maze structure.
 * @param frame_buffer Pointer to the frame buffer.
 * @return 0 on success, non-zero on failure.
 */
int draw_maze_outer(Maze *maze, uint8_t *frame_buffer);

#endif /* MAZE_H */
