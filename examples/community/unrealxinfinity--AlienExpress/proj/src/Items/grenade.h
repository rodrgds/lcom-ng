#ifndef GRENADE_H
#define GRENADE_H



#include "../macros/includes.h"
#include "../load/view.h"
#include "../Entities/explosion.h"

/**
 * @brief Calls drawing function to draw grenade and also calls animations functions to do throwing animation
 * @param index Index position of grenade to draw
 */
void draw_grenade(int index);


/**
 * @brief Sets the current inventory position to a grenade.
 * @param index Current inventory index position
 * 
 */
void get_grenade(int index);

/**
 * @brief The animation when you throw a grenade
 * @param item grenade being thrown
 * @param was_throwed boolean to keep track if grenade was thrown yet or not. Has value false when grenade has yet not been thrown and true if otherwise
 */
void throw_animation_grenade(img_t *item, bool was_throwed);

/**
 * @brief Eliminates all monsters in a radius cause by grenade explosion
 * @param x left horizontal coordinate of grenade explosion radius
 * @param y up vertical coordinate of grenade explosion radius
 * @param max_x right horizontal coordinate of grenade explosion radius
 * @param max_y down vertical coordinate of grenade explosion radius
 */
void kill_monster_radius(int x, int y, int max_x, int max_y);

#endif

