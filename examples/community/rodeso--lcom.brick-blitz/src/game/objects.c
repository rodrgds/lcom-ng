#include "objects.h"


// Function to initialize a background
void initBackground(Background* background, int x, int y, Sprite *background_sprite) {
    background->x = x;
    background->y = y;
    background->sprite = background_sprite;
}

// Function to initialize a ball
void initBall(Ball* ball, int x, int y, Sprite *ball_sprite) {
    ball->x = x;
    ball->y = y;
    ball->base = true;
    ball->dx = 4;
    ball->dy = -4;
    ball->oldx = 0;
    ball->oldy = 0;
    ball->sprite = ball_sprite;
}

// Function to initialize a brick
void initBrick(Brick* brick, int x, int y, Sprite *brick_sprite) {
    brick->x = x;
    brick->y = y;
    brick->destroyed = false;
    brick->sprite = brick_sprite;
}

// Function to initialize the paddle
void initPaddle(Paddle* paddle, int x, int y, Sprite *paddle_sprite) {
    paddle->x = x;
    paddle->y = y;
    paddle->oldx = 20;
    paddle->sprite = paddle_sprite;
}

// Function to initialize the projectile
void initProjectile(Projectile* projectile, Paddle paddle, Sprite *projectile_sprite, Sprite *explosion_sprite) {
    projectile->x = paddle.x + paddle.sprite->width/2;
    projectile->y = paddle.y;
    projectile->sprite = projectile_sprite;
    projectile->explosion = explosion_sprite;
}

// Function to initialize the marker
void initMarker(Marker* marker, int x, Sprite *marker_sprite) {
    marker->x = x;
    marker->oldx = 0;
    marker->sprite = marker_sprite;
}


