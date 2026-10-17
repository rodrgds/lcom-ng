#ifndef PHYSICS_H
#define PHYSICS_H

#include "vector2.h"

#define EARTH_ACC 9.81

#define TIME_STEP 0.01667f

#define X_MIN 62
#define X_MAX 755
#define Y_MIN 170
#define Y_MAX 549

typedef struct {
  vec_2d pos;
  vec_2d vel;
  vec_2d acc;
  vec_2d force;
  float mass;
  float radius;
  float friction_coef;
} ball;

/**
 * @brief Update the state of a body over a time interval
 * @param body physics body to be updated
*/
void update(ball *body);

/**
 * @brief Check if two circles are colliding
 * @return true if they are colliding
*/
bool are_colliding(ball *c1, ball *c2);

/**
 * @brief Handle a collision between two circles
 * @remark Provide pointers to circles in their previous
 * state, i.e. during frame right before the collision
*/
void handle_collision(ball *c1, ball *c2);

/**
 * Get time until the two circles collide
 * @param r displacement vector (r2 - r1)
 * @param v difference in velocity of the 2 circles (v2 - v1)
*/
float get_timeof_collision(ball *c1, ball *c2, vec_2d r, vec_2d v);

/**
 * @brief Handle a wall collision
 * @param c ball to be collided
 * @param hit_wall indicates whether there is a wall collision
*/
void handle_wallcollision(ball *b, bool* hit_wall);

/**
 * @brief Get the angle of the cue stick
 * @param c1 the cue ball
 * @param mouse_pos the position of the mouse
 * @return the angle of the cue stick
*/
double get_cue_angle(ball *c1, vec_2d mouse_pos);

/**
 * @brief Get the force of the shot
 * @param c1 the cue ball
 * @param mouse_pos the position of the mouse
 * @return the force of the shot
*/
vec_2d get_shot_force_dir(ball *c1, vec_2d mouse_pos);


#endif
