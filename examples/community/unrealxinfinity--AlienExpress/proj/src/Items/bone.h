#ifndef BONE_H
#define BONE_H


#include "../macros/includes.h"
#include "../load/view.h"

/**
 * @brief Calls drawing function to draw bone and also calls animation function when bone is thrown
 */
void draw_bone();

/**
 * @brief Sets the current inventory position to a bone.
 * @param index Current inventory index position
 * 
 */
void get_bone(int index);

/**
 * @brief Animates the bone being thrown
 * @param item bone that is being thrown
 * @param was_thrown Boolean to know if bone has been thrown or not. Has value false if it hasnt yet and true if otherwise
 */

void throw_animation_bone(img_t *item, bool was_throwed);


#endif

