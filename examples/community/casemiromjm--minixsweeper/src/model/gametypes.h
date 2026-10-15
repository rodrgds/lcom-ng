/**
 * @file gametypes.h
 * @brief Header file for defining game types and enums.
 */

#ifndef GAMETYPES_H
#define GAMETYPES_H

/**
 * @enum GameDifficulty
 * @brief Defines difficulty levels for the game.
 */
typedef enum {
    EASY,    ///< Easy difficulty level
    MEDIUM,  ///< Medium difficulty level
    HARD     ///< Hard difficulty level
} GameDifficulty;

/**
 * @enum GameMode
 * @brief Defines different game modes.
 */
typedef enum {
    NORMAL,   ///< Normal game mode
    DECEIVER  ///< Deceiver game mode (special mode, details depend on implementation)
} GameMode;

#endif

