#include "vector2.h"

#include <math.h>

vec_2d vec_neg(vec_2d v) {
  vec_2d res;
  res.x = -v.x;
  res.y = -v.y;
  return res;
}

vec_2d vec_add(vec_2d v1, vec_2d v2) {
  vec_2d res;
  res.x = v1.x + v2.x;
  res.y = v1.y + v2.y;
  return res;
}

vec_2d vec_add_scalar(vec_2d v, float scalar) {
  vec_2d res;
  res.x = v.x + scalar;
  res.y = v.y + scalar;
  return res;
}

vec_2d vec_mult(vec_2d v1, vec_2d v2) {
  vec_2d res;
  res.x = v1.x * v2.x;
  res.y = v1.y * v2.y;
  return res;
}

vec_2d vec_mult_scalar(vec_2d v, float scalar) {
  vec_2d res;
  res.x = v.x * scalar;
  res.y = v.y * scalar;
  return res;
}

vec_2d vec_sign(vec_2d v) {
  vec_2d res;
  res.x = (v.x == 0) ? 0 : (v.x < 0) ? -1 : 1;
  res.y = (v.y == 0) ? 0 : (v.y < 0) ? -1 : 1;
  return res;
}

vec_2d vec_normal(vec_2d v) {
  vec_2d res;
  res.x = -res.x;
  return res;
}

vec_2d vec_unit(vec_2d v) {
  float len = vec_len(v);
  if (len == 0) {
    return NULL_VEC;
  }
  vec_2d res;
  res.x = v.x / len;
  res.y = v.y / len;
  return res;
}

bool vec_equals(vec_2d v1, vec_2d v2) {
  return (v1.x == v2.x) && (v1.y == v2.y);
}

float vec_dot(vec_2d v1, vec_2d v2) {
  return (v1.x * v2.x) + (v1.y * v2.y);
}

float vec_len(vec_2d v) {
  return sqrtf((v.x * v.x) + (v.y * v.y));
}
