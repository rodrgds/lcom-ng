#ifndef TELEPORT_H
#define TELEPORT_H


#include "../macros/includes.h"
#include "../load/view.h"
#include <math.h>

/**
 * @brief Calls drawing function to draw the teleports and also calls the animation function to animate the score
 */
void draw_teleports();
/**
 * @brief Animates the score after delivering a package to a teleport
 * @param character Teleport that was delivered a package onto
 * @param type Type of coloring format of the score numbers. If 0 then it draws normally, if 1 it draws with green coloring, if 4 it draws with red coloring
 * @return Returns 0 if successfull
 */
int score_animation(img_t *character, int type);

/**
 * @brief Picks the type of coloring format to draw the score numbers. If 0 then it draws normally, if 1 it draws with green coloring, if 4 it draws with red coloring
 */
void pick_draw_type();

/**
 * @brief Calls number drawing function to draw the score numbers.
 * @param type Type of coloring format of the score numbers. If 0 then it draws normally, if 1 it draws with green coloring, if 4 it draws with red coloring
 */
void draw_score(int type);


#endif

