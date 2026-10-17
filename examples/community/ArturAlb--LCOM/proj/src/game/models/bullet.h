/**
 * @file bullet.h
 *
 */

#ifndef _LCOM_BULLET_H_
#define _LCOM_BULLET_H_

#include <stdint.h>

#include "object.h"

/**
 * @brief bullet is anextension of type object
 * 
 * @param object the base of the bullet
 * 
 * @param owner player who fired the bullet
 * 
 * @param velocityX velocity of the bullet on the x axis
 * 
 * @param velocityY velocity of the bullet on the Y axis
 * 
 * @param bounces number of times the bullet has bounced
 * 
 * @param bounced tells if in a given frame it has already bounced
 * 
 */
typedef struct {
    Object *object;
    Object *owner;
    float velocityX;
    float velocityY;
    uint8_t bounces;
    bool bounced;
} Bullet;

/**
 * @brief it frees the memory allocated to the bullet passed in the parameter
 * 
 * @param obj Pointer to struct type bullet
 */
void freeBullet(Bullet* obj);

/**
 * @brief alters the trajectory of the bullet
 * 
 * @param bullet bullet that has gone against a wall and is supposed to bounce
 * @param obj wall against which the bullet has gone against
 */
void bullet_bounce(Bullet* bullet, Object* obj);

#endif
