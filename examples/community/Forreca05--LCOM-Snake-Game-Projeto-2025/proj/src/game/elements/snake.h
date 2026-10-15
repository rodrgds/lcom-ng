/**
 * @file snake.h
 * @brief Defines the Snake struct and related functions for the classic snake game.
 *
 * This header provides the definition of the Snake struct and function prototypes
 * for creating, moving, drawing, and destroying classic snakes in the game.
 */

#ifndef SNAKE_H
#define SNAKE_H

#include <stdlib.h>
#include <stdbool.h>
#include "../../controllers/gpu/sprites/sprite.h"
#include "../UTILS.h"

#define MAX_SNAKE_LEN 100

/**
 * @struct Snake
 * @brief Represents a classic snake in the game.
 */
typedef struct Snake Snake;

struct Snake {
    int x, y, xspeed, yspeed;           /**< Head position and speed of the snake */
    Sprite* sprite;                     /**< Pointer to the head sprite */
    Sprite* snakebody[MAX_SNAKE_LEN];   /**< Array of pointers to body sprites */
    int length;                         /**< Number of body segments */
    Direction direction;                /**< Current direction (enum) */

    /**
     * @brief Draws the snake on the screen.
     * @param self Pointer to the Snake.
     */
    void (*draw)(Snake* self);

    /**
     * @brief Destroys the snake and frees its resources.
     * @param self Pointer to the Snake to destroy.
     */
    void (*destroy)(Snake* self);

    /**
     * @brief Moves the snake and handles apple collision.
     * @param self Pointer to the Snake.
     * @param apple Pointer to the apple sprite.
     * @return 0 if collision with itself, 1 if apple eaten, 2 otherwise.
     */
    int (*move)(Snake* self, Sprite* apple);
};

/**
 * @brief Creates a new Snake at the given position and speed.
 * @param x Initial x position.
 * @param y Initial y position.
 * @param xspeed Initial x speed.
 * @param yspeed Initial y speed.
 * @return Pointer to the created Snake.
 */
Snake* create_snake(int x, int y, int xspeed, int yspeed);

#endif
