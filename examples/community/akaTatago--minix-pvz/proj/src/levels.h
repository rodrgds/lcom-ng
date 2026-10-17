/**
 * @file levels.h
 * @brief Level configurations, wave details, and spawning logic.
 */

#ifndef LEVELS_H
#define LEVELS_H

#include <stdint.h>

#define MAX_WAVES 10
#define MAX_ZOMBIES_PER_WAVE 20

/**
 * @brief Defines the spawn parameters for a single zombie.
 */
typedef struct {
    uint8_t type; /**< @brief The type of zombie. */
    uint8_t lane; /**< @brief The grid lane for the spawn. */
} ZombieSpawnConfig;

/**
 * @brief Defines a wave composed of multiple zombies.
 */
typedef struct {
    int num_zombies;                                 /**< @brief Total zombies in this wave. */
    ZombieSpawnConfig spawns[MAX_ZOMBIES_PER_WAVE];  /**< @brief Configuration array for the spawns. */
    int time_to_next_wave;                           /**< @brief Time delay until the next wave begins. */
} Wave;

/**
 * @brief Defines the overall structure and parameters of a level.
 */
typedef struct {
    int initial_delay;           /**< @brief Delay before the first wave begins. */
    int total_waves;             /**< @brief Total number of waves in the level. */
    Wave waves[MAX_WAVES];       /**< @brief Array holding the wave configurations. */
} LevelConfig;

/**
 * @brief Constant array defining all levels in the game.
 */
static const LevelConfig ALL_LEVELS[] = {
    
    // LEVEL 1
    {
        .initial_delay = 15, 
        .total_waves = 2,
        .waves = {
            { .num_zombies = 2, .spawns = {{1, 1}, {1, 2}}, .time_to_next_wave = 10 },
            
            { .num_zombies = 1, .spawns = {{1, 2}}, .time_to_next_wave = 3 }
        }
    },
    
 // LEVEL 2 - more waves
    {
        .initial_delay = 12,
        .total_waves = 4,
        .waves = {
            { .num_zombies = 1, .spawns = {{1, 3}}, .time_to_next_wave = 18 },
            { .num_zombies = 2, .spawns = {{1, 0}, {1, 4}}, .time_to_next_wave = 18 },
            { .num_zombies = 3, .spawns = {{1, 1}, {1, 2}, {1, 3}}, .time_to_next_wave = 7 },
            { .num_zombies = 2, .spawns = {{1, 0}, {1, 4}}, .time_to_next_wave = 0 }
        }
    },
 
    // LEVEL 3 - more zombies per wave
    {
        .initial_delay = 12,
        .total_waves = 4,
        .waves = {
            { .num_zombies = 1, .spawns = {{1, 2}}, .time_to_next_wave = 18 },
            { .num_zombies = 3, .spawns = {{1, 0}, {1, 2}, {1, 4}}, .time_to_next_wave = 25 },
            { .num_zombies = 4, .spawns = {{1, 0}, {1, 1}, {1, 3}, {1, 4}}, .time_to_next_wave = 10 },
            { .num_zombies = 3, .spawns = {{1, 1}, {1, 2}, {1, 3}}, .time_to_next_wave = 0 }
        }
    },
 
    // LEVEL 4 - faster waves
    {
        .initial_delay = 15,
        .total_waves = 5,
        .waves = {
            { .num_zombies = 1, .spawns = {{1, 2}}, .time_to_next_wave = 25 },
            { .num_zombies = 2, .spawns = {{1, 2}, {1, 4}}, .time_to_next_wave = 10 },
            { .num_zombies = 3, .spawns = {{1, 1}, {1, 2}, {1, 3}}, .time_to_next_wave = 10 },
            { .num_zombies = 4, .spawns = {{1, 0}, {1, 1}, {1, 3}, {1, 4}}, .time_to_next_wave = 6 },
            { .num_zombies = 2, .spawns = {{1, 0}, {1, 4}}, .time_to_next_wave = 0 }
        }
    },
 
    // LEVEL 5 - all lanes at once
    {
        .initial_delay = 15,
        .total_waves = 5,
        .waves = {
            { .num_zombies = 1, .spawns = {{1, 3}}, .time_to_next_wave = 18 },
            { .num_zombies = 3, .spawns = {{1,0},{1,2},{1,4}}, .time_to_next_wave = 8 },
            { .num_zombies = 5, .spawns = {{1,0},{1,0},{1,2},{1,3},{1,4}}, .time_to_next_wave = 10 },
            { .num_zombies = 5, .spawns = {{1,0},{1,1},{1,2},{1,3},{1,4}}, .time_to_next_wave = 8 },
            { .num_zombies = 3, .spawns = {{1,1},{1,2},{1,3}}, .time_to_next_wave = 0 }
        }
    },
 
    // LEVEL 6 - bigger waves, shorter breaks
    {
        .initial_delay = 8,
        .total_waves = 7,
        .waves = {
            { .num_zombies = 1, .spawns = {{1, 2}}, .time_to_next_wave = 20 },
            { .num_zombies = 1, .spawns = {{1, 3}}, .time_to_next_wave = 18 },
            { .num_zombies = 4, .spawns = {{1,0},{1,1},{1,3},{1,4}}, .time_to_next_wave = 8 },
            { .num_zombies = 5, .spawns = {{1,0},{1,1},{1,2},{1,3},{1,4}}, .time_to_next_wave = 6 },
            { .num_zombies = 3, .spawns = {{1,0},{1,2},{1,4}}, .time_to_next_wave = 8 },
            { .num_zombies = 5, .spawns = {{1,0},{1,1},{1,2},{1,3},{1,4}}, .time_to_next_wave = 6 },
            { .num_zombies = 4, .spawns = {{1,0},{1,1},{1,3},{1,4}}, .time_to_next_wave = 0 }
        }
    },
 
    // LEVEL 7 - double zombies in lanes
    {
        .initial_delay = 8,
        .total_waves = 7,
        .waves = {
            { .num_zombies = 1, .spawns = {{1, 2}}, .time_to_next_wave = 18 },
            { .num_zombies = 1, .spawns = {{1, 3}}, .time_to_next_wave = 18 },
            { .num_zombies = 4, .spawns = {{1,0},{1,0},{1,2},{1,4}}, .time_to_next_wave = 8 },
            { .num_zombies = 6, .spawns = {{1,0},{1,1},{1,1},{1,2},{1,3},{1,4}}, .time_to_next_wave = 6 },
            { .num_zombies = 4, .spawns = {{1,1},{1,2},{1,3},{1,3}}, .time_to_next_wave = 8 },
            { .num_zombies = 6, .spawns = {{1,0},{1,0},{1,2},{1,2},{1,4},{1,4}}, .time_to_next_wave = 6 },
            { .num_zombies = 5, .spawns = {{1,0},{1,1},{1,2},{1,3},{1,4}}, .time_to_next_wave = 0 }
        }
    },
 
    // LEVEL 8 - heavy assault
    {
        .initial_delay = 7,
        .total_waves = 8,
        .waves = {
            { .num_zombies = 1, .spawns = {{1, 1}}, .time_to_next_wave = 18 },
            { .num_zombies = 1, .spawns = {{1, 3}}, .time_to_next_wave = 18 },
            { .num_zombies = 5, .spawns = {{1,0},{1,1},{1,2},{1,3},{1,4}}, .time_to_next_wave = 6 },
            { .num_zombies = 5, .spawns = {{1,0},{1,1},{1,2},{1,3},{1,4}}, .time_to_next_wave = 6 },
            { .num_zombies = 4, .spawns = {{1,0},{1,0},{1,4},{1,4}}, .time_to_next_wave = 6 },
            { .num_zombies = 5, .spawns = {{1,0},{1,1},{1,2},{1,3},{1,4}}, .time_to_next_wave = 6 },
            { .num_zombies = 6, .spawns = {{1,0},{1,1},{1,1},{1,3},{1,3},{1,4}}, .time_to_next_wave = 4 },
            { .num_zombies = 5, .spawns = {{1,0},{1,1},{1,2},{1,3},{1,4}}, .time_to_next_wave = 0 }
        }
    },
 
    // LEVEL 9 - fast and relentless
    {
        .initial_delay = 6,
        .total_waves = 9,
        .waves = {
            { .num_zombies = 1, .spawns = {{1, 4}}, .time_to_next_wave = 18 },
            { .num_zombies = 1, .spawns = {{1, 3}}, .time_to_next_wave = 18 },
            { .num_zombies = 5, .spawns = {{1,0},{1,1},{1,2},{1,3},{1,4}}, .time_to_next_wave = 6 },
            { .num_zombies = 6, .spawns = {{1,0},{1,0},{1,1},{1,2},{1,3},{1,4}}, .time_to_next_wave = 4 },
            { .num_zombies = 5, .spawns = {{1,0},{1,1},{1,2},{1,3},{1,4}}, .time_to_next_wave = 6 },
            { .num_zombies = 6, .spawns = {{1,0},{1,1},{1,2},{1,3},{1,4},{1,4}}, .time_to_next_wave = 4 },
            { .num_zombies = 5, .spawns = {{1,0},{1,1},{1,2},{1,3},{1,4}}, .time_to_next_wave = 6 },
            { .num_zombies = 7, .spawns = {{1,0},{1,0},{1,1},{1,2},{1,3},{1,4},{1,4}}, .time_to_next_wave = 4 },
            { .num_zombies = 5, .spawns = {{1,0},{1,1},{1,2},{1,3},{1,4}}, .time_to_next_wave = 0 }
        }
    },
 
    // LEVEL 10 - final level, maximum difficulty
    {
        .initial_delay = 5,
        .total_waves = 10,
        .waves = {
            { .num_zombies = 1, .spawns = {{1, 2}}, .time_to_next_wave = 18 },
            { .num_zombies = 1, .spawns = {{1, 3}}, .time_to_next_wave = 18 },
            { .num_zombies = 5, .spawns = {{1,0},{1,1},{1,2},{1,3},{1,4}}, .time_to_next_wave = 6 },
            { .num_zombies = 6, .spawns = {{1,0},{1,0},{1,1},{1,2},{1,3},{1,4}}, .time_to_next_wave = 4 },
            { .num_zombies = 7, .spawns = {{1,0},{1,0},{1,1},{1,2},{1,3},{1,4},{1,4}}, .time_to_next_wave = 4 },
            { .num_zombies = 5, .spawns = {{1,0},{1,1},{1,2},{1,3},{1,4}}, .time_to_next_wave = 6 },
            { .num_zombies = 8, .spawns = {{1,0},{1,0},{1,1},{1,1},{1,2},{1,3},{1,4},{1,4}}, .time_to_next_wave = 4 },
            { .num_zombies = 5, .spawns = {{1,0},{1,1},{1,2},{1,3},{1,4}}, .time_to_next_wave = 4 },
            { .num_zombies = 8, .spawns = {{1,0},{1,0},{1,1},{1,2},{1,2},{1,3},{1,4},{1,4}}, .time_to_next_wave = 4 },
            { .num_zombies = 10, .spawns = {{1,0},{1,0},{1,1},{1,1},{1,2},{1,2},{1,3},{1,3},{1,4},{1,4}}, .time_to_next_wave = 0 }
        }
    }


};

#endif // LEVELS_H
