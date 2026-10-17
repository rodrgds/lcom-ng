#ifndef PROJ_ENTITY_H
#define PROJ_ENTITY_H

#include <stdbool.h>
#include <lcom/lcf.h>

/**
 * @brief Represents a player in the game.
 */
typedef struct {
    unsigned int x_pos;    /**< The x position of the player (constant). */
    unsigned int y_pos;          /**< The y position of the player. */
    unsigned int width;          /**< The width of the player. */
    unsigned int height;         /**< The height of the player. */
    unsigned char number;  /**< The player's number (constant). */
} Player;

/**
 * @brief Represents a ball in the game.
 */
typedef struct {
    unsigned int radius;   /**< The radius of the ball (constant). */
    unsigned int width;    /**< The width of the ball (constant). */
    unsigned int height;   /**< The height of the ball (constant). */
    unsigned int x_pos;          /**< The x position of the ball. */
    unsigned int y_pos;          /**< The y position of the ball. */
    int vX;
    int vY;       /**< The velocity of the ball. */
    float angle;                 /**< The angle of the ball's movement. */
    uint8_t *sprite;             /**< Pointer to the sprite representing the ball. */
} Ball;

/**
 * @brief Represents a wall in the game.
 */
typedef struct {
    unsigned int x_pos;    /**< The x position of the wall (constant). */
    unsigned int y_pos;    /**< The y position of the wall (constant). */
    unsigned int width;    /**< The width of the wall (constant). */
    unsigned int height;   /**< The height of the wall (constant). */
    bool top;              /**< Indicates if the wall is the top wall (constant). */
} Wall;

/**
 * @brief Enumerates the types of collision entities.
 */
typedef enum {
    PLAYER,   /**< Collision with a player. */
    WALL,     /**< Collision with a wall. */
} CollisionType;

/**
 * @brief Union representing a collision entity, which can be either a player or a wall.
 */
typedef union {
    Player *player;   /**< The player involved in the collision. */
    Wall *wall;       /**< The wall involved in the collision. */
} CollisionEntity;

/**
 * @brief Handles the collision between a ball and a collision entity.
 *
 * This function processes the collision between a ball and another entity 
 * (either a player or a wall). It adjusts the ball's position based on
 * the type of collision.
 *
 * @param ball Pointer to the ball involved in the collision.
 * @param entity Collision entity (either a player or a wall).
 * @param type The type of the collision entity (PLAYER or WALL).
 * @return Return true if a collision occurred, false otherwise.
 */
bool collision_handler(Ball *ball, CollisionEntity entity, CollisionType type);

/**
 * @brief Creates a ball with the given sprite.
 *
 * This function initializes a new ball object with the specified sprite. 
 * The ball's initial position, velocity, and angle are set to default values.
 *
 * @param xpm The sprite map for the ball.
 * @return Pointer to the newly created ball object.
 */

bool update_ball(Ball *ball);

/**
 * @brief Creates a ball object.
 *
 * This function creates a ball object with the specified XPM map representing its sprite.
 *
 * @param xpm The XPM map representing the sprite of the ball.
 * @return Pointer to the created ball object.
 */
Ball *create_ball(xpm_map_t xpm);

/**
 * @brief Creates a wall object.
 *
 * This function creates a wall object with the specified orientation.
 *
 * @param top Boolean value indicating whether the wall is positioned at the top of the screen or at the bottom.
 * @return Pointer to the created wall object.
 */
Wall *create_wall(bool top);

Player *create_player(unsigned char number);


#endif
