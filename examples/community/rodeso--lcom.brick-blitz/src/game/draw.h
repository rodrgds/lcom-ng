/**
 * @file draw.h
 * @brief Header file containing the declaration of the draw functions.
 */

#ifndef _LCOM_DRAW_H_
#define _LCOM_DRAW_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "objects.h"

/**
 * @brief Draws the background on the screen.
 *
 * @param background Pointer to the Background object.
 * @return 0 on success, non-zero otherwise.
 */
int drawBackground(Background *background);

/**
 * @brief Draws the paddle on the screen.
 *
 * @param paddle Pointer to the Paddle object.
 * @return 0 on success, non-zero otherwise.
 */
int drawPaddle(Paddle *paddle);

/**
 * @brief Draws a brick on the screen.
 *
 * @param brick Pointer to the Brick object.
 * @return 0 on success, non-zero otherwise.
 */
int drawBrick(Brick *brick);

/**
 * @brief Draws the ball on the screen.
 *
 * @param ball Pointer to the Ball object.
 * @return 0 on success, non-zero otherwise.
 */
int drawBall(Ball *ball);

/**
 * @brief Draws a projectile on the screen.
 *
 * @param projectile Pointer to the Projectile object.
 * @return 0 on success, non-zero otherwise.
 */
int drawProjectile(Projectile *projectile);

/**
 * @brief Draws an explosion animation on the screen.
 *
 * @param projectile Pointer to the Projectile object representing the explosion.
 * @return 0 on success, non-zero otherwise.
 */
int drawExplosion(Projectile *projectile);

/**
 * @brief Draws a marker on the screen.
 *
 * @param marker Pointer to the Marker object.
 * @return 0 on success, non-zero otherwise.
 */
int drawMarker(Marker *marker);

/**
 * @brief Erases the paddle from the screen.
 *
 * @param paddle Pointer to the Paddle object.
 * @return 0 on success, non-zero otherwise.
 */
int erasePaddle(Paddle *paddle);

/**
 * @brief Erases the ball from the screen.
 *
 * @param ball Pointer to the Ball object.
 * @return 0 on success, non-zero otherwise.
 */
int eraseBall(Ball *ball);

/**
 * @brief Erases a brick from the screen.
 *
 * @param brick Pointer to the Brick object.
 * @return 0 on success, non-zero otherwise.
 */
int eraseBrick(Brick *brick);

/**
 * @brief Erases a projectile from the screen.
 *
 * @param projectile Pointer to the Projectile object.
 * @return 0 on success, non-zero otherwise.
 */
int eraseProjectile(Projectile *projectile);

/**
 * @brief Erases a marker from the screen.
 *
 * @param marker Pointer to the Marker object.
 * @return 0 on success, non-zero otherwise.
 */
int eraseMarker(Marker *marker);

#endif
