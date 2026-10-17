#ifndef MOB_H
#define MOB_H

#include <lcom/lcf.h>
#include <math.h>
#include <stdlib.h>

#include "assets/xpm/level/mob/attacking.xpm"
#include "assets/xpm/level/mob/idle.xpm"
#include "config.h"
#include "drivers/video/gpu.h"
#include "model/utils/animated_sprite.h"
#include "model/utils/sprite.h"

#define MOB_RADIUS 250      /**< Radius of the mob's field of view */
#define MOB_MAX_SPEED 5     /**< Maximum speed of the mob */
#define MOB_HEALTH 1        /**< Health of the mob */
#define MOB_DEFAULT_SPEED 2 /**< Default speed of the mob */

/**
 * @brief Represents a single Mob instance.
 *
 * The Mob struct is opaque.
 */
typedef struct Mob Mob;

/**
 * @brief Possible states of the mob for animations and logic.
 */
typedef enum {
    MOB_IDLE,      /**< Mob is idle */
    MOB_ATTACKING, /**< Mob is attacking */
    MOB_DEAD       /**< Mob is dying */
} MobState;
/**
 * @brief Creates a new mob instance with the specified sprite.
 *
 * @param sprite Pointer to the Sprite structure used for the mob.
 * @return Pointer to the created Mob structure.
 */
Mob *create_mob(int16_t x, int16_t y);

/**
 * @brief Frees the memory associated with the mob.
 *
 * @param mob Pointer to the Mob structure to destroy.
 * @return 0 on success, 1 on failure.
 */
int destroy_mob(Mob *mob);

/**
 * @brief Returns the Sprite associated with the given mob.
 *
 * @param mob Pointer to the Mob structure.
 * @return Pointer to the Sprite structure.
 */
Sprite *mob_get_sprite(Mob *mob);

/**
 * @brief Sets the mob's sprite to a new one.
 *
 * @param mob Pointer to the Mob structure.
 * @param sprite Pointer to the new Sprite.
 */
void mob_set_sprite(Mob *mob, Sprite *sprite);

/**
 * @brief Returns the x-coordinate of the mob.
 *
 * @param mob Pointer to the Mob structure.
 * @return The mob's x-coordinate.
 */
int16_t mob_get_x(Mob *mob);

/**
 * @brief Sets the x of the mob.
 *
 * @param mob Pointer to the Mob structure.
 * @param x New x value.
 */
void mob_set_x(Mob *mob, int16_t x);

/**
 * @brief Returns the y-coordinate of the mob.
 *
 * @param mob Pointer to the Mob structure.
 * @return The mob's y-coordinate.
 */
int16_t mob_get_y(Mob *mob);

/**
 * @brief Sets the yx of the mob.
 *
 * @param mob Pointer to the Mob structure.
 * @param y New y value.
 */
void mob_set_y(Mob *mob, int16_t y);

/**
 * @brief Returns the x-speed of the mob.
 *
 * @param mob Pointer to the Mob structure.
 * @return The mob's x-speed.
 */
double mob_get_xspeed(Mob *mob);

/**
 * @brief Returns the y-speed of the mob.
 *
 * @param mob Pointer to the Mob structure.
 * @return The mob's y-speed.
 */
double mob_get_yspeed(Mob *mob);

/**
 * @brief Returns the current health of the mob.
 *
 * @param mob Pointer to the Mob structure.
 * @return The mob's health.
 */
unsigned char mob_get_health(Mob *mob);

/**
 * @brief Sets the current health of the mob.
 *
 * @param mob Pointer to the Mob structure.
 * @param health New health value.
 */
void mob_set_health(Mob *mob, unsigned char health);

/**
 * @brief Returns the maximum speed of the mob.
 *
 * @param mob Pointer to the Mob structure.
 * @return The mob's maximum speed.
 */
int mob_get_max_speed(Mob *mob);

/**
 * @brief Returns the current state of the mob.
 *
 * @param mob Pointer to the Mob structure.
 * @return Current mob state.
 */
MobState mob_get_state(Mob *mob);

/**
 * @brief Updates the state of the mob based on its context.
 *
 * @param mob Pointer to the Mob structure.
 * @param mob_cx X coordinate of the center of the mob.
 * @param mob_cy Y coordinate of the center of the mob.
 * @return 0 on success, 1 on failure.
 */
int mob_update_state(Mob *mob, int16_t mob_cx, int16_t mob_cy);

/**
 * @brief Renders the mob on the screen.
 *
 * @param mob Pointer to the Mob structure.
 * @param frame_buffer Pointer to the frame buffer.
 * @return 0 on success, 1 on failure.
 */
int draw_mob(Mob *mob, uint8_t *frame_buffer);

#endif /* MOB_H */
