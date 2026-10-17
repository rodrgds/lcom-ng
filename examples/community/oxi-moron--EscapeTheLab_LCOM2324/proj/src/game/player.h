#ifndef G6_PLAYER_H
#define G6_PLAYER_H

#include <lcom/lcf.h>
#include <math.h>

#include "game_macros.h"
#include "map.h"

/**
 * @brief Struct that defines a player, with his position and his angle.
 */
struct Player {
    struct point2D position;
    double angle;
};

/**
 * @brief Enumeration of the players possible moves.
 */
enum player_moves {UP, DOWN, LEFT, RIGHT, ROTATE_LEFT, ROTATE_RIGHT};

static struct Player player;

/**
 * @brief Initializes the player (sets his start position and angle).
 */
void player_construct();

/**
 * @brief Gets the players current position.
 * @return The position of the player.
 */
struct point2D player_get_position();

/**
 * @brief Gets the players current angle.
 * @return The angle of the player.
 */
double player_get_angle();

/**
 * @brief Sets the position of the player.
 * @param position Position to be set.
 * @return 0 if successful, non-zero otherwise.
 */
int player_set_position(struct point2D position);

/**
 * @brief Sets the angle of the player.
 * @param angle Angle to be set.
 * @return 0 if successful, non-zero otherwise.
 */
int player_set_angle(double angle);

/**
 * @brief Resets the players position to its initial position.
 */
void player_reset_position(int n);

/**
 * @brief Verifies if a player is walking through a door.
 * @return True if player is crossing the door, false otherwise.
 */

bool player_crossed_door();

/**
 * @brief Moves the player with a preset movement.
 * @param move Move to be executed.
 * @return 0 if successful, non-zero otherwise.
 */
int player_move(enum player_moves move);

/**
 * @brief Converts an angle in degrees to radians.
 * @param angle Angle to be converted.
 * @return Corresponding radians value.
 */
double (to_radians) (double angle);

#endif //G6_PLAYER_H
