#ifndef POOL_H
#define POOL_H

#include "physics.h"
#include <lcom/lcf.h>

typedef enum {
  MAIN_MENU,
  ABOUT_MENU,
  AWAITING_PHYSICS,
  WAITING_FOR_HIT,
  PREPARING_HIT,
  POSITIONING_CUE,
  GAME_OVER
} game_state;

typedef struct {
  bool multiplayer;
  game_state state;
  bool myturn; 
  bool turn;
  vec_2d mouse_pos;
  ball* balls;
  ball* pockets;
  bool* in_pocket;
  uint16_t current_turn_time;
  int gameover_delay;
} pool_game;

pool_game game;

typedef enum {
  // player hit a pool ball
  PLAYER_HIT_BALL,
  // ball was pocketed
  BALL_POCKETED,
  // black ball was pocketed
  BALL8_POCKETED,
  // white ball was pocketed
  CUEBALL_POCKETED,
  // opponent ball hit first
  OPPONENT_BALL_HIT,
  FAILED_TO_HIT_BALL,
  FAILED_TO_HIT_RAIL,
  TOO_LONG_TO_HIT
} game_event;

/**
 * @brief Start a new 8 ball pool game
 * @param myturn whether player is p0 or p1
 * @return zero on success, non-0 otherwise
*/
int start_game(bool myturn);

/**
 * @brief Handle an event amidst a pool game
 * @param e event to be handled
*/
void handle_event(game_event e);

/**
 * @brief Set up the balls for a new game
 * @remark The white ball is the first one
 * @remark Balls are positioned in a triangle
*/
void setup_balls();

/**
 * @brief Set up the pockets for a new game
*/
void setup_pockets();

/**
 * @brief Update the physics of the game
 * @param mouse_pos position of the mouse
*/
void update_physics(vec_2d* mouse_pos);

/**
 * @brief Handle a ball being pocketed
 * @param c ball to be pocketed
*/
void handle_pocketing(ball *c, ball *pocket);

/**
 * @brief Get the padding of the cue stick from the cue ball based on the mouse position
 * @param cue_pos position of the cue ball
 * @param mouse_pos position of the mouse
 * @return the padding of the cue stick
*/
float get_cue_padding(vec_2d cue_pos, vec_2d mouse_pos);

bool is_pocket_empty();

bool is_pocket_full();

#endif
