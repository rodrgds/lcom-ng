#ifndef __BALL_H
#define __BALL_H

#include <lcom/lcf.h>
#include <stdint.h>
#include "images/letter_g.xpm"
#include "images/letter_o.xpm"
#include "images/letter_a.xpm"
#include "images/letter_l.xpm"
#include "images/number_zero.xpm"
#include "images/number_one.xpm"
#include "images/number_two.xpm"
#include "images/number_three.xpm"
#include "images/nada.xpm"

#include "piece.h"

/// @brief Handles the ball start when is goal or the game is starting
/// @return 0 if success, 1 if no success
int start_ball();

/// @brief Checks if the ball collided with piece 1 and its zone of collision
/// @return 0 if success, 1 if no success
int collision_ball_piece1();

/// @brief Checks if the ball collided with piece 2 and its zone of collision
/// @return 0 if success, 1 if no success
int collision_ball_piece2();

/// @brief Implement all the useful functions in this class to control the ball movement
/// @return 0 if success, 1 if no success
int ball_movement();

/// @brief Draws the ball in its position
/// @return 0 if success, 1 if no success
int draw_ball();

/// @brief Checks if left piece scored goal
/// @return 0 if success, 1 if no success
int goal_leftPiece();

/// @brief Checks if right piece scored goal
/// @return 0 if success, 1 if no success
int goal_rightPiece();

/// @brief Handles the score in the top of the field
/// @return 0 if success, 1 if no success
int score();

#endif /* _BALL_H_ */


