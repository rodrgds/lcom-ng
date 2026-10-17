#include "physics.h"

#include <math.h>
#include <float.h>

#include <lcom/lcf.h>

void update(ball *c) {

  // stabilize velocity
  if (fabs(c->vel.x) < 1) {
    c->vel.x = 0.0f;
  }
  if (fabs(c->vel.y) < 1) {
    c->vel.y = 0.0f;
  }

  vec_2d friction = NULL_VEC;

  float friction_len = EARTH_ACC * c->mass * c->friction_coef;

  // compute friction force
  if (!vec_equals(c->vel, NULL_VEC)) {
    vec_2d vel_unit = vec_unit(c->vel);
    friction = vec_mult_scalar(vel_unit, -friction_len);
  }

  // update acceleration (F = ma)
  vec_2d net_force = vec_add(c->force, friction);
  c->acc = vec_mult_scalar(net_force, 1.0/c->mass);

  // update velocity
  c->vel = vec_add(c->vel, vec_mult_scalar(c->acc, TIME_STEP));

  // update position
  c->pos = vec_add(c->pos, vec_mult_scalar(c->vel, TIME_STEP));

  c->force = NULL_VEC;
}

bool are_colliding(ball *c1, ball *c2) {
  vec_2d tmp = vec_add(c2->pos, vec_neg(c1->pos));
  float len = (tmp.x * tmp.x) + (tmp.y * tmp.y);
  float rad_sqr = (c1->radius + c2->radius) * (c1->radius + c2->radius);
  return len < rad_sqr;
}

float get_timeof_collision(ball *c1, ball *c2, vec_2d r, vec_2d v) {

  // (rad1 + rad2) ^2
  float rad_sum_sqr = (c1->radius + c2->radius) * (c1->radius + c2->radius);

  // compute a, b, c of quadratic formula
  float a = vec_dot(v, v);
  float b = vec_dot(r, v);
  float c = vec_dot(r, r) - rad_sum_sqr;

  return (-b - sqrtf((b*b) - (4*a*c))) / (2*a);
}

void handle_collision(ball *c1, ball *c2) {

  // displacement vector
  vec_2d r = vec_add(c2->pos, vec_neg(c1->pos));

  // relative velocity
  vec_2d v = vec_add(c2->vel, vec_neg(c1->vel));
  
  float t = get_timeof_collision(c1, c2, r, v);

  // move circles to collision point 
  c1->pos = vec_add(c1->pos, vec_mult_scalar(c1->vel, t));
  c2->pos = vec_add(c2->pos, vec_mult_scalar(c2->vel, t));

  // recalculate r and v with respect to the collision point
  r = vec_add(c2->pos, vec_neg(c1->pos));
  v = vec_add(c2->vel, vec_neg(c1->vel));

  // normal vector  
  vec_2d n = vec_unit(r);

  // relative velocity in normal direction
  float v_dot_n = vec_dot(v, n);


  float impulse = 2 * v_dot_n / (c1->mass + c2->mass);

  // update velocities
  c1->vel = vec_add(c1->vel, vec_mult_scalar(n, impulse * c2->mass));
  c2->vel = vec_add(c2->vel, vec_mult_scalar(n, -impulse * c1->mass));
}

double get_cue_angle(ball *c1, vec_2d mouse_pos) {
  vec_2d cue_dir = vec_add(mouse_pos, vec_neg(c1->pos));
  return atan2(cue_dir.y , cue_dir.x) + M_PI / 2;;
}

vec_2d get_shot_force_dir(ball *c1, vec_2d mouse_pos) {
  vec_2d direction = vec_add(mouse_pos, vec_neg(c1->pos));

    vec_2d normalized_direction = vec_unit(direction);

    vec_2d scaled_force = vec_mult_scalar(normalized_direction, 100000);

    double max_force = 200000;
    float force_magnitude = vec_len(scaled_force);
    if (force_magnitude > max_force) {
        scaled_force = vec_mult_scalar(normalized_direction, max_force);
    }

    return scaled_force;
}

void handle_wallcollision(ball *b, bool* hit_wall) {

  float tx = FLT_MAX;
  float ty = FLT_MAX;

  // x-axis collision
  if(b->vel.x == 0) {
    if(b->pos.x < X_MIN) {
      b->pos.x = X_MIN;
    }
    if(b->pos.x > X_MAX - b->radius) {
      b->pos.x = X_MAX - b->radius;
    }
  }

  if (b->pos.x < X_MIN) {
    tx = (X_MIN - b->pos.x) / b->vel.x;
  }
  else if ((b->pos.x + b->radius) > X_MAX) {
    tx = (X_MAX - (b->radius) - b->pos.x) / b->vel.x;
  }

  if (tx != FLT_MAX) {
    *hit_wall = true;
    b->pos = vec_add(b->pos, vec_mult_scalar(b->vel, tx));
    b->vel.x = -b->vel.x;
  }

  // y-axis collision

  if(b->vel.y == 0) {
    if(b->pos.y < Y_MIN) {
      b->pos.y = Y_MIN;
    }
    if(b->pos.y > Y_MAX - b->radius) {
      b->pos.y = Y_MAX - b->radius;
    }
  }

  if (b->pos.y < Y_MIN) {
    ty = (Y_MIN - b->pos.y) / b->vel.y;
  }
  else if ((b->pos.y + b->radius) > Y_MAX) {
    ty = (Y_MAX - (b->radius) - b->pos.y) / b->vel.y;
  }

  if (ty != FLT_MAX) {
    *hit_wall = true;
    b->pos = vec_add(b->pos, vec_mult_scalar(b->vel, ty));
    b->vel.y = -b->vel.y;
  }
}
