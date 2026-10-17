#ifndef TARGET_H
#define TARGET_H

#include <lcom/lcf.h>
#include "position.h"

#define TARGET_RADIUS 54
#define TARGET_RADIUS_2 2916  //TARGET_RADIUS²
#define TARGET_RADIUS_2_CENTER 400
#define TARGET_RADIUS_2_MIDDLE 1600


/**
 * @struct Target
 * @brief Represents a target in the game.
 * 
 * The Target struct contains information about the position, activity status, fall counter, and direction of a target.
 */
typedef struct {
    Position pos;           /** The position of the target */
    bool active;            /** The activity status of the target */
    int16_t fallCounter;    /** The fall counter of the target (Used for the fall animation) */
    Direction dir;          /** The direction of the target */
} Target;

/**
 * @brief Creates a new target with the specified position and direction.
 * 
 * @param x The x-coordinate of the target's position
 * @param y The y-coordinate of the target's position
 * @param dir The direction of the target
 * @return A pointer to the created target
 */
Target* createTarget(int x, int y, Direction dir);

/**
 * @brief Gets the x-coordinate of the target's position.
 * 
 * @param target A pointer to the target
 * @return The x-coordinate of the target's position
 */
int16_t getTargetX(Target* target);

/**
 * @brief Gets the y-coordinate of the target's position.
 * 
 * @param target A pointer to the target
 * @return The y-coordinate of the target's position
 */
int16_t getTargetY(Target* target);

/**
 * @brief Destroys a target and frees the associated memory.
 * 
 * @param target A pointer to the target to be destroyed
 */
void destroyTarget(Target* target);

#endif
