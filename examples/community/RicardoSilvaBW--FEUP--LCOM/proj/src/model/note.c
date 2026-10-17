#include "note.h"
#include <stdlib.h>
#include "../config.h"

void note_init(note *note, sprite *sp, double speed, note_lane lane) {
    if (note == NULL || sp == NULL)
        return ;

    note->sp = sp;
    note->speed = speed;
    note->y_position = sp->y;
    note->lane = lane;
    note->type = ARROW_NOTE;
    note->active = true;
    note->missed = false;
    note->hit = false;
    note->spawn_delay = 0;
    note->spawn_timer = 0;
}

bool acceptance_zone(const note *note) {
    if (note == NULL || note->sp == NULL)
        return false;
    int centre_y = note->sp->y + note->sp->height / 2;
    return (centre_y >= ACCEPTANCE_Y_TOP && centre_y <= ACCEPTANCE_Y_BOT);
}

void note_update(note *note) {
    if (note == NULL) return;
    if (!note->active) {
        if (note->spawn_timer > 0) {
            note->spawn_timer--;
        }
        return;
    }
    
    note->y_position -= note->speed;
    note->sp->y = (int) note->y_position;
    int centre_y = note->sp->y + note->sp->height / 2;
    if (!note->missed && !note->hit && centre_y < ACCEPTANCE_Y_TOP){
        note->missed = true;
        note->active = false;
    }
}

void note_reset_random(note *note, int spawn_y, int max_delay) {
    if (note == NULL || spawn_y <= 0)
        return ;
    note->sp->y = spawn_y;
    note->y_position = spawn_y;
    note->missed = false;
    note->hit = false;
    note->spawn_delay = (max_delay > 0) ? (rand() % max_delay) : 0;
    note->spawn_timer = note->spawn_delay;
    
    note->active = (note->spawn_delay == 0);
}

bool note_is_hittable(note* n) {
    if (!n->active) return false;

    return acceptance_zone(n);
}

bool (note_is_clickable)(note* n, sprite* heart, int mouse_x, int mouse_y) {
    if (n == NULL || n->sp == NULL || heart == NULL) return false;
    if (!n->active) return false;

    if (!acceptance_zone(n)) return false;

    int dist_to_left_corner_x = mouse_x - n->sp->x;
    int dist_to_left_corner_y = mouse_y - n->sp->y;

    if (dist_to_left_corner_x < 0 || dist_to_left_corner_x >= heart->width || dist_to_left_corner_y < 0 || dist_to_left_corner_y >= heart->height) return false;

    int scale = (heart->img.width > 0) ? (heart->width / heart->img.width) : 1;
    
    if (scale < 1) scale = 1;
    
    int src_x = dist_to_left_corner_x / scale;
    int src_y = dist_to_left_corner_y / scale;
    
    int pixel_index = (src_y * heart->img.width + src_x) * 4;
    uint8_t alpha = heart->colors[pixel_index + 3];
    return (alpha == 0);
}
