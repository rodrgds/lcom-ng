#include "sun.h"
#include "constants.h"

void init_sun(Sun *sun, int start_x, int start_y, int target_y) {
    sun->x = start_x;
    sun->y = start_y;
    sun->target_y = target_y;
    sun->target_x = 0;     /* not flying yet */
    sun->speed = 1; // Falls slowly
    sun->is_active = true;
    sun->is_flying = false;
    sun->ground_ticks = 0;
    sun->frame = 0;
    sun->anim_counter = 0;
    sun->anim_speed = SUN_ANIM_SPEED;
}

void update_sun_animation(Sun *sun) {
    sun->anim_counter++;

    if (sun->anim_counter >= sun->anim_speed) {
        sun->frame++;
        sun->anim_counter = 0;
    }
}

/* Moves the sun towards the HUD sun-counter position.
 * Called every tick while is_flying == true.
 * Returns true when the sun has reached the destination. */
void update_sun_fly(Sun *sun) {
    int dx = SUN_HUD_TARGET_X - sun->x;
    int dy = SUN_HUD_TARGET_Y - sun->y;

    /* Move at SUN_FLY_SPEED pixels per tick along each axis */
    if (dx > SUN_FLY_SPEED)       sun->x += SUN_FLY_SPEED;
    else if (dx < -SUN_FLY_SPEED) sun->x -= SUN_FLY_SPEED;
    else                           sun->x  = SUN_HUD_TARGET_X;

    if (dy > SUN_FLY_SPEED)       sun->y += SUN_FLY_SPEED;
    else if (dy < -SUN_FLY_SPEED) sun->y -= SUN_FLY_SPEED;
    else                           sun->y  = SUN_HUD_TARGET_Y;
}
