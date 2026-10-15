#include "draw.h"


//----------------draw--------------------------------------------------------------------------------------------------------------------


int drawBackground(Background *background) {
    if(drawSprite(background->sprite, background->x, background->y)!=0) {return 1;}
    return 0;
}

int drawPaddle(Paddle *paddle) {
    if(drawSprite(paddle->sprite, paddle->x, paddle->y)!=0) {return 1;}
    return 0;
}

int drawBrick(Brick *brick) {
    if(drawSprite(brick->sprite, brick->x, brick->y)!=0) {return 1;}
    return 0;
}

int drawBall(Ball *ball) {
    if(drawSprite(ball->sprite, ball->x, ball->y)!=0) {return 1;}
    return 0;
}

int drawProjectile(Projectile *projectile) {
    if(drawSprite(projectile->sprite, projectile->x, projectile->y)!=0) {return 1;}
    return 0;
}

int drawExplosion(Projectile *projectile) {
    if(drawSprite(projectile->explosion, projectile->x-12, projectile->y)!=0) {return 1;}
    return 0;
}

int drawMarker(Marker *marker) {
    if(drawSprite(marker->sprite, marker->x, 475)!=0) {return 1;}
    return 0;
}


//----------------erase--------------------------------------------------------------------------------------------------------------------


int erasePaddle(Paddle *paddle) {
    if(eraseSprite(paddle->sprite, paddle->oldx, paddle->y)!=0) {return 1;}
    return 0;
}

int eraseBall(Ball *ball) {
    if(eraseSprite(ball->sprite, ball->oldx, ball->oldy)!=0) {return 1;}
    return 0;
}

int eraseBrick(Brick *brick) {
    if(eraseSprite(brick->sprite, brick->x, brick->y)!=0) {return 1;}
    return 0;
}

int eraseProjectile(Projectile *projectile) {
    if(eraseSprite(projectile->sprite, projectile->x, projectile->oldy)!=0) {return 1;}
    return 0;
}

int eraseMarker(Marker *marker) {
    for (int i = 16; i < 768 ; i++) {
        if(eraseSprite(marker->sprite, i, 475)!=0) {return 1;}
    }
    return 0;
}

