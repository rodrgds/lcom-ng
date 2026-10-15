/**
 * @file objects.h
 * @brief Header file containing the declarations of various game objects.
 */

#ifndef _LCOM_OBJECTS_H_
#define _LCOM_OBJECTS_H_

#include <stdio.h>
#include "sprite/sprite.h"

/**
 * @struct Background
 * @brief Structure representing the background object.
 */
typedef struct {
    int x;              /**< x-coordinate of the background */
    int y;              /**< y-coordinate of the background */
    Sprite *sprite;     /**< pointer to the background sprite */
} Background;

/**
 * @struct Ball
 * @brief Structure representing the ball object.
 */
typedef struct {
    int x;              /**< x-coordinate of the ball */
    int y;              /**< y-coordinate of the ball */
    int oldx;           /**< x-coordinate of the ball in the previous frame */
    int oldy;           /**< y-coordinate of the ball in the previous frame */
    int dx;             /**< x-speed of the ball */
    int dy;             /**< y-speed of the ball */
    int base;           /**< flag to indicate if the ball is the base */
    Sprite *sprite;     /**< pointer to the ball sprite */
} Ball;

/**
 * @struct Brick
 * @brief Structure representing the brick object.
 */
typedef struct {
    int x;              /**< x-coordinate of the brick */
    int y;              /**< y-coordinate of the brick */
    int destroyed;      /**< flag to indicate if the brick is destroyed */
    Sprite *sprite;     /**< pointer to the brick sprite */
} Brick;

/**
 * @struct Paddle
 * @brief Structure representing the paddle object.
 */
typedef struct {
    int x;              /**< x-coordinate of the paddle */
    int y;              /**< y-coordinate of the paddle */
    int oldx;           /**< old x-coordinate of the paddle */
    Sprite *sprite;     /**< pointer to the paddle sprite */
} Paddle;

/**
 * @struct Projectile
 * @brief Structure representing the projectile object.
 */
typedef struct {
    int x;              /**< x-coordinate of the projectile */
    int y;              /**< y-coordinate of the projectile */
    int oldy;           /**< old y-coordinate of the projectile */
    Sprite *sprite;     /**< pointer to the projectile sprite */
    Sprite *explosion;  /**< pointer to the explosion sprite */
} Projectile;

/**
 * @struct Marker
 * @brief Structure representing the marker object.
 */
typedef struct {
    int x;              /**< x-coordinate of the marker */
    int oldx;           /**< old x-coordinate of the marker */
    Sprite *sprite;     /**< pointer to the marker sprite */
} Marker;

/**
 * @brief Initializes the background object.
 * 
 * @param background Pointer to the background object.
 * @param x The x-coordinate of the background.
 * @param y The y-coordinate of the background.
 * @param sprite Pointer to the background sprite.
 */
void initBackground(Background* background, int x, int y, Sprite *sprite);

/**
 * @brief Initializes the ball object.
 * 
 * @param ball Pointer to the ball object.
 * @param x The x-coordinate of the ball.
 * @param y The y-coordinate of the ball.
 * @param sprite Pointer to the ball sprite.
 */
void initBall(Ball* ball, int x, int y, Sprite *sprite);

/**
 * @brief Initializes the brick object.
 * 
 * @param brick Pointer to the brick object.
 * @param x The x-coordinate of the brick.
 * @param y The y-coordinate of the brick.
 * @param sprite Pointer to the brick sprite.
 */
void initBrick(Brick* brick, int x, int y, Sprite *sprite);

/**
 * @brief Initializes the paddle object.
 * 
 * @param paddle Pointer to the paddle object.
 * @param x The x-coordinate of the paddle.
 * @param y The y-coordinate of the paddle.
 * @param sprite Pointer to the paddle sprite.
 */
void initPaddle(Paddle* paddle, int x, int y, Sprite *sprite);

/**
 * @brief Initializes the projectile object.
 * 
 * @param projectile Pointer to the projectile object.
 * @param paddle The paddle object.
 * @param sprite Pointer to the projectile sprite.
 * @param explosion Pointer to the explosion sprite.
 */
void initProjectile(Projectile* projectile, Paddle paddle, Sprite *sprite, Sprite *explosion);

/**
 * @brief Initializes the marker object.
 * 
 * @param marker Pointer to the marker object.
 * @param x The x-coordinate of the marker.
 * @param sprite Pointer to the marker sprite.
 */
void initMarker(Marker* marker, int x, Sprite *sprite);

#endif
