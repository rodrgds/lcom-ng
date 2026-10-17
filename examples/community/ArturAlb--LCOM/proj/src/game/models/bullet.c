#include <lcom/lcf.h>

#include <math.h>
#include "bullet.h"

void freeBullet(Bullet* obj) {
    free(obj->object);
    free(obj);
}

void bullet_bounce(Bullet* bullet, Object* obj){
    // revert the position
    bullet->object->x -= bullet->velocityX;
    bullet->object->y -= bullet->velocityY;

    if (pointInObject(obj, bullet->object->x + bullet->velocityX, bullet->object->y)){
        bullet->velocityX *= -1;
        --bullet->bounces;
        bullet->bounced = true;
    }

    if (pointInObject(obj, bullet->object->x, bullet->object->y + bullet->velocityY)){
        bullet->velocityY *= -1;
        bullet->bounces -= !bullet->bounced;
        bullet->bounced = true;
    }
}
