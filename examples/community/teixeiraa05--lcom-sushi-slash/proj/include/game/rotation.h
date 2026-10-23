#ifndef ROTATION_H
#define ROTATION_H

#include <math.h>

typedef struct {
    double x, y;
} point_t;

typedef struct {
    double cosine, sine;
} rotation_t;

/* Screen coordinates: positive angles turn clockwise. */
static inline rotation_t rotation_for_angle(int degrees) {
    double radians = degrees * 0.017453292519943295;
    return (rotation_t){cos(radians), sin(radians)};
}

static inline point_t rotation_apply(rotation_t rotation, point_t point) {
    return (point_t){rotation.cosine * point.x - rotation.sine * point.y,
                     rotation.sine * point.x + rotation.cosine * point.y};
}

static inline point_t rotation_inverse(rotation_t rotation, point_t point) {
    rotation.sine = -rotation.sine;
    return rotation_apply(rotation, point);
}

#endif
