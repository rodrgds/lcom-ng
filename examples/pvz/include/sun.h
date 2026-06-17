#ifndef SUN_H
#define SUN_H

#include <stdbool.h>

typedef struct {
    int x;
    int y;
    int target_y;
    int target_x;       /* HUD destination X (used when flying) */
    int speed;
    bool is_active;
    bool is_flying;     /* true after the player clicks the sun */
    int ground_ticks;
    int frame;
    int anim_counter;
    int anim_speed;
} Sun;

void init_sun(Sun *sun, int start_x, int start_y, int target_y);
void update_sun_animation(Sun *sun);
void update_sun_fly(Sun *sun);

#endif
