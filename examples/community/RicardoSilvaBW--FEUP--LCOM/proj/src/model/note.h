#ifndef NOTE_H
#define NOTE_H

#include <lcom/lcf.h>
#include "sprite.h"

typedef enum {
    LANE_NONE = -1,
    LANE_UP,
    LANE_DOWN,
    LANE_LEFT,
    LANE_RIGHT
} note_lane;

typedef enum {
    ARROW_NOTE,
    HEART_NOTE
} note_type;

typedef struct {
    sprite *sp;
    double speed;
    double y_position;
    bool active;
    note_lane lane;
    note_type type;
    bool missed;
    bool hit;
    int spawn_delay;
    int spawn_timer;
} note;

void note_init(note *note, sprite *sp, double speed, note_lane lane);
void note_update(note *note);
void note_reset_random(note *note, int spawn_y, int max_delay);

bool (note_acceptance_zone)(const note *note);
bool (note_is_hittable)(note* n);
bool (note_is_clickable)(note* n, sprite* heart, int mouse_x, int mouse_y);

#endif
