/** 
 * @file animations.h
 * @brief Header file for @ref animations.c
 * 
 * This file declares all the functions to be used for the animations, as well as the animation global variables themselves
 */


#ifndef _ANIMATIONS_PROJ_H_
#define _ANIMATIONS_PROJ_H_

#include <lcom/lcf.h>

#include "../model/entity.h"
#include "../model/structs.h"

/**
 * @enum animation_direction
 * @brief Represents index of the animation to choose
 */
enum animation_direction {
  LEFT, 
  RIGHT,
  IDLE_
};

/**
 * @brief Gets the direction of the entity to choose which the animation
 * 
 * The entity can be facing left, right or be idle.
 * 
 * @param entity The entity to get the direction from
 * @return The animation_direction of the entity
 */
enum animation_direction (entity_get_direction)(entity* entity);

/**
 * @brief Loads all the animations to the gloabl variables
 * 
 * Calls the functions to load the specific animations.
 * 
 * @return 0 in case of success, 1 otherwise
 */
int (animations_load)();

/**
 * @brief Frees all the dynamically allocated space of the animations
 * 
 * Frees every animation on the animation arrays and then the animations themselves.
 * 
 * @return 0 in case of success, 1 otherwise
 */
int (animations_clean)();

/**
 * @brief Destroys an animation
 * 
 * Frees every sprite in the animation and then the animation itself.
 * 
 * @return 0 in case of success, 1 otherwise
 */
int (animation_destroy)(animation animation);


//the animations themselves
extern animation* pharaoh_animations;            ///< The animations for the pharaoh
extern animation* enemy1_animations;             ///< The animations for the carequinha 1
extern animation* enemy2_animations;             ///< The animations for the carequinha 2
extern animation* mummy_animations;              ///< The animations for the mummy
extern animation* secret_animations;             ///< It's a secret shhhhhhh


extern animation khopesh_attack_right_animation; ///< The animation for the auto attack right
extern animation khopesh_attack_left_animation;  ///< The animation for the auto attack left
extern animation lightning_attack_animation;     ///< The animation for the lightning attack
extern animation heart_animation;                ///< The animation for the heart


#endif /* _ANIMATIONS_PROJ_H_ */
