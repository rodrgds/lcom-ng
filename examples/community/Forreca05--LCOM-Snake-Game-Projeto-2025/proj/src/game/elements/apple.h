/**
 * @file apple.h
 * @brief Defines the Apple struct and related functions for the snake game.
 *
 * This header provides the definition of the Apple struct, its types, and function prototypes
 * for creating and managing apples and their power-ups in the game.
 */
#ifndef APPLE_H
#define APPLE_H

#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdlib.h>
#include "../../controllers/gpu/sprites/sprite.h"
#include "freesnake.h"
#include "../UTILS.h"

// nao apaguem isto, tem de estar aqui por causa de dependencias circulares
struct FreeSnake;

/**
 * @enum AppleType
 * @brief Enumerates the different types of apples.
 */
typedef enum { NORMAL, GOLDEN, WHITE, DARK } AppleType;

/**
 * @struct Apple
 * @brief Represents an apple in the game, which may have special effects.
 */
typedef struct Apple Apple;

struct Apple {
    int x;                      /**< X position */
    int y;                      /**< Y position */
    Sprite* sprite;             /**< Pointer to the sprite of the apple */
    AppleType type;             /**< Type of the apple */
    int powerUpTimer;           /**< Timer for power-up effects */
    bool exists;                /**< Whether the apple currently exists in the game */

    /**
     * @brief Destroys the apple and frees its resources.
     * @param self Pointer to the Apple to destroy.
     */
    void (*destroy)(Apple* self);

    /**
     * @brief Applies the apple's power-up effect to the given snake.
     * @param self Pointer to the Apple.
     * @param snake Pointer to the FreeSnake.
     * @param option Game mode option.
     */
    void (*applyPowerUp)(Apple* self, struct FreeSnake* snake, int option);

    /**
     * @brief Disables the apple's power-up effect on the given snake.
     * @param self Pointer to the Apple.
     * @param snake Pointer to the FreeSnake.
     * @param option Game mode option.
     */
    void (*disablePowerUp)(Apple* self, struct FreeSnake* snake, int option);

    /**
     * @brief Checks and updates the power-up timer for the apple.
     * @param self Pointer to the Apple.
     * @param snake Pointer to the FreeSnake .
     * @param option Game mode option.
     */
    void (*checkPowerUpTimer)(Apple* self, struct FreeSnake* snake, int option);

    /**
     * @brief Draws the apple if it exists.
     * @param self Pointer to the Apple.
     */
    void (*drawIfExists)(Apple* self);
};

/**
 * @brief Creates a new apple of the given type.
 * @param type The type of apple(color) to create.
 * @param exists Whether the apple should exist initially.
 * @param isMultiplayer Whether the game is in multiplayer mode.
 * @return Pointer to the created Apple.
 */
Apple* create_apple(AppleType type, bool exists, bool isMultiplayer);

#endif
