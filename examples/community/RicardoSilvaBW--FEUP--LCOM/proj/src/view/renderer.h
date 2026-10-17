#ifndef RENDERER_H
#define RENDERER_H

#include "../model/note.h"
#include "../model/sprite.h"

int (draw_note)(note *n);
int (draw_sprite)(sprite *sp);

void (draw_acceptance_zone)(const int lane_x[4], sprite *targets[4]);
void (draw_time)(sprite *clock, int remaining_secs, int x, int y);
void (draw_score)(sprite *star_sp, int score, int x, int y);

#endif
