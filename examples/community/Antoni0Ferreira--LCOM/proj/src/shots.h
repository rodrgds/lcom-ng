#ifndef __SHOTS_H
#define __SHOTS_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#include "video.h"
#include "xpm/shot1.xpm"
#include "xpm/shot2.xpm"
#include "xpm/shot3.xpm"
#include "xpm/shot4.xpm"
#include "xpm/shot5.xpm"
#include "xpm/shot6.xpm"
#include "xpm/shot_asset.xpm"
#include "xpm/shot_exp1.xpm"
#include "xpm/shot_exp2.xpm"
#include "xpm/shot_exp3.xpm"
#include "xpm/shot_exp4.xpm"
#include "xpm/shot_exp5.xpm"

//sprites from https://craftpix.net/freebies/free-pixel-art-enemy-spaceship-2d-sprites/

/**
 * @defgroup shots shots
 * @{
 * 
 * Functions regarding the initialization and drawing of the missile shots
 */

/**
 * @brief Struct type for storing and organizing information relative to a shot
 * 
 */
struct shot {
    uint16_t x, y;
    int8_t speed;
    int animation;
};

/**
 * @brief Struct type for storing and organizing information relative to a shot animation
 * 
 */
struct shot_animation{
    xpm_image_t img;
    uint8_t *sprite;
};

/**
 * @brief Function responsible for initializing each shot and shot animations of the arrays with
 * their respective sprite, xpm image and initial positions. In this case, these positions will be
 * hidden from the player, as to only appear when they click on the left button of the mouse
 * 
 * @param shots Array of shots
 * @param shot_animations Array of animation of the shots
 * @return Return 0 upon success and non-zero otherwise 
 */
int (initialize_shots)(struct shot shots[], struct shot_animation shot_animations[]);

/**
 * @}
 */

#endif /* __SHOTS_H */
