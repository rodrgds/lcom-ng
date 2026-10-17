#include "pool.h"

#include <math.h>
#include <float.h>
#include "./io/rtc/rtc.h"
#include <lcom/lcf.h>

bool first_hit = true;
bool hit_wall = false;
bool keep_turn = false;
bool pocketed = false;
uint8_t ball_index = -1;

int start_game(bool myturn) {

  memset(&game, 0, sizeof(game));

  game.turn = 0;
  game.myturn = myturn;

  game.state = MAIN_MENU;

  game.balls = (ball*) malloc(16*sizeof(ball));
  game.pockets = (ball*) malloc(6*sizeof(ball));
  game.in_pocket = (bool*) malloc(16*sizeof(bool));

  for(int i = 0; i < 16; i++) {
    game.in_pocket[i] = false;
  }

  if (game.balls == NULL) {
    return 1;
  }

  if(game.pockets == NULL) {
    return 1;
  }

  update_datetime();
  game.current_turn_time = 0;
  return 0;
}

bool only_8ball_remaining() {
  int count = 0;
  int offset = game.myturn ? 9 : 1;
  for(int i = offset; i < 7 + offset; i++) {
    if(!game.in_pocket[i]) {
      count++;
    }
  }

  return count == 0;
}

float get_cue_padding(vec_2d cue_pos, vec_2d mouse_pos) {
  vec_2d aux = vec_add(cue_pos, vec_neg(mouse_pos));
  return fmin(vec_len(aux) * 0.25, 50);
}

bool is_pocket_empty() {
  bool flag = false;

  for(int i = 0; i < 16; i++) {
    if(game.in_pocket[i]) flag = true;
  }

  return flag;
}

void handle_event(game_event e) {
  switch (e) {
    case BALL_POCKETED:
      pocketed = true;
      if ((game.myturn && ball_index > 8) || (!game.myturn && ball_index < 8)) {
        keep_turn = true;
        ball_index = -1;
      } else {
        keep_turn = false;
        ball_index = -1;
      }
      game.in_pocket[ball_index] = true;
      break;
    case BALL8_POCKETED:
      if (!only_8ball_remaining()) {
        game.turn = !game.turn;
      }
      game.state = GAME_OVER;
      game.gameover_delay = 0;
      break;
    case CUEBALL_POCKETED:
      keep_turn = false;
      game.balls[0].pos.x = 150;
      game.balls[0].pos.y = 300;
      game.balls[0].vel.x = 0;
      game.balls[0].vel.y = 0;
      game.in_pocket[0] = true;
      break;
    case OPPONENT_BALL_HIT:
      keep_turn = false;
      first_hit = false;
      break;
    case FAILED_TO_HIT_RAIL:
      keep_turn = false;
      break;
    case TOO_LONG_TO_HIT:
      game.current_turn_time = 0;
      game.turn = !game.turn;
      break;
    default:
      break;
  }
}

void setup_balls() {
  float ball_radius = 21;
  float row_spacing = sqrt(3) * ball_radius;
  vec_2d start_pos = {300, 300}; 

  int index = 1;

  for (int row = 0; row < 5; row++) {
    for (int i = 0; i <= row; i++) {
      game.balls[index].pos.y = start_pos.y + (i - row / 2.0) * 2 * ball_radius;
      game.balls[index].pos.x = start_pos.x + row * row_spacing;
      game.balls[index].acc = NULL_VEC;
      game.balls[index].vel = NULL_VEC;
      game.balls[index].friction_coef = 8;
      game.balls[index].mass = 5;
      game.balls[index].radius= 20;
      game.balls[index].force = NULL_VEC;
      index++;
    }
  }

  game.balls[0].pos = (vec_2d){150, 300};
  game.balls[0].force = (vec_2d){0,0};
  game.balls[0].acc = NULL_VEC;
  game.balls[0].vel = NULL_VEC;
  game.balls[0].friction_coef = 8;
  game.balls[0].mass = 5;
  game.balls[0].radius= 20;
}

void setup_pockets() {
  for(int i = 0; i < 6; i++) {
    game.pockets[i].radius = 6;
    game.pockets[i].mass = FLT_MAX;
    game.pockets[i].friction_coef = 0;
    game.pockets[i].force = NULL_VEC;
    game.pockets[i].acc = NULL_VEC;
    game.pockets[i].vel = NULL_VEC;
  }

  game.pockets[0].pos = (vec_2d){45, 154};
  game.pockets[1].pos = (vec_2d){45, 552};
  game.pockets[2].pos = (vec_2d){399, 154};
  game.pockets[3].pos = (vec_2d){399, 552};
  game.pockets[4].pos = (vec_2d){758, 154};
  game.pockets[5].pos = (vec_2d){758, 552};

}

void update_physics(vec_2d* mouse_pos) {
  bool are_balls_moving = false;
  // update physics for all balls
  for (int i = 0; i < 16; i++) {
    if(game.in_pocket[i]) continue;
    update(&game.balls[i]);
    if(vec_len(game.balls[i].vel) != 0) are_balls_moving = true;
  }

  // check for pocketing
  for(int i = 0; i < 6; i++) {
    for(int j = 0; j < 16; j++) {
      if(!game.in_pocket[j] && are_colliding(&game.balls[j], &game.pockets[i])) {
        handle_pocketing(&game.balls[j], &game.pockets[i]);
        if(is_pocket_empty()) {
          if(j > 8) {
            game.myturn = true; // myturn 1 -> balls with id > 8 || myturn 0 -> solid balls
          }
        }
        game.in_pocket[j] = true;
        if(j != 8 && j != 0) ball_index = j;
        handle_event(j == 8 ? BALL8_POCKETED : (j == 0 ? CUEBALL_POCKETED : BALL_POCKETED));
      }
    }
  }

  // check for collisions with other balls and walls
  for (int i = 0; i < 15; i++) {
    for (int j = i + 1; j < 16; j++) {
      if(game.in_pocket[i] || game.in_pocket[j]) continue;
      if (i != j && are_colliding(&game.balls[i], &game.balls[j])) {
        handle_collision(&game.balls[i], &game.balls[j]);
        if(first_hit && i == 0 && ((j > 8 && game.myturn) || (j < 8 && !game.myturn)) && !is_pocket_empty()) {
          handle_event(OPPONENT_BALL_HIT);
        }
      }
    }
  }

  for (int i = 0; i < 16; i++) {
    handle_wallcollision(&game.balls[i], &hit_wall);
  }

  if (game.state != GAME_OVER) {
    if (are_balls_moving) 
    game.state = AWAITING_PHYSICS;
    else {
      if(game.in_pocket[0]) game.in_pocket[0] = false;
      mouse_pos->x = game.balls[0].pos.x;
      mouse_pos->y = game.balls[0].pos.y;
      if(!hit_wall && !pocketed) handle_event(FAILED_TO_HIT_RAIL);
      if(first_hit) handle_event(FAILED_TO_HIT_BALL);
      game.state = WAITING_FOR_HIT;
      game.turn = keep_turn ? game.turn : !game.turn;
      first_hit = true;
      hit_wall = false;
      keep_turn = false;
  }
  }

}

void handle_pocketing(ball *c, ball *pocket) {
  c->pos = pocket->pos;
  c->vel = NULL_VEC;
  c->acc = NULL_VEC;
  c->force = NULL_VEC;
}


