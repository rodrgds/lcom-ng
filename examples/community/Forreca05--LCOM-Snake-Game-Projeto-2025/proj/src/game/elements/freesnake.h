/**
 * @file freesnake.h
 * @brief Defines the FreeSnake struct and related functions for the snake game.
 *
 * This header provides the definition of the FreeSnake struct and function prototypes
 * for creating, moving, drawing, and handling collisions for free-moving snakes in the game.
 */
#ifndef FREESNAKE_H
#define FREESNAKE_H

#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdlib.h>
#include <math.h>
#include "../../controllers/gpu/gpu.h"
#include "../../controllers/gpu/sprites/sprite.h"
#include "apple.h"
#include "../UTILS.h"

// nao apaguem isto, tem de estar aqui por causa de dependencias circulares
struct Apple;

/**
 * @struct FreeSnake
 * @brief Represents a free-moving snake in the game.
 */
typedef struct FreeSnake FreeSnake;


struct FreeSnake {
    int x, y;                  /**< Head position of the snake */
    int vx, vy;                /**< Velocity components */
    double angle;              /**< Current movement angle (radians) */
    double speed;              /**< Current speed */
    Direction direction;       /**< Current direction (enum) */
    Sprite* sprite;            /**< Pointer to the head sprite */
    Sprite* body[1000];        /**< Array of pointers to body sprites */
    int bodyLength;            /**< Number of body segments */
    double r;                  /**< Radius or distance between segments */

    /**
     * @brief Destroys the snake and frees its resources.
     * @param self Pointer to the FreeSnake to destroy.
     */
    void (*destroy)(FreeSnake* self);

    /**
     * @brief Moves the snake according to the current direction and speed.
     * @param self Pointer to the FreeSnake.
     * @param option Game mode option.
     * @return true if the move was successful, false otherwise.
     */
    bool (*move)(FreeSnake* self, int option);

    /**
     * @brief Checks for collision with an apple.
     * @param self Pointer to the FreeSnake.
     * @param apple Pointer to the Apple.
     * @return true if collision occurred, false otherwise.
     */
    bool (*checkAppleCollision)(FreeSnake* self, struct Apple* apple);

    /**
     * @brief Checks for collision with another snake.
     * @param self Pointer to the FreeSnake.
     * @param other Pointer to the other FreeSnake.
     * @return true if collision occurred, false otherwise.
     */
    bool (*checkSnakeCollision)(FreeSnake* self, FreeSnake* other);

    /**
     * @brief Draws the snake on the screen.
     * @param self Pointer to the FreeSnake.
     */
    void (*draw)(FreeSnake* self);
};

/**
 * @brief Creates a new FreeSnake at the given position.
 * @param x Initial x position.
 * @param y Initial y position.
 * @return Pointer to the created FreeSnake.
 */
FreeSnake* createFreeSnake(int x, int y);

#endif

