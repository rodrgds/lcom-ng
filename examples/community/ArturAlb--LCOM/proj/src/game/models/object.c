#include <lcom/lcf.h>

#include "object.h"

void moveUp(Object* obj, uint16_t speed){
    obj->y -= speed;
}

void moveDown(Object* obj, uint16_t speed){
    obj->y += speed;
}

void moveLeft(Object* obj, uint16_t speed){
    obj->x -= speed;
}

void moveRight(Object* obj, uint16_t speed){
    obj->x += speed;
}

void freeObject(Object* obj) {
    free(obj->image->colors);
    free(obj->image);
    free(obj);
}

bool pointInObject(Object *obj, uint16_t x, uint16_t y) {
    return (obj->x <= x) && (obj->x + obj->width >= x) && (obj->y <= y) && (obj->y + obj->height >= y); 
}

bool checkCollisions(Object *obj1, Object* obj2) {
    return pointInObject(obj2, obj1->x, obj1->y) || 
        pointInObject(obj2, obj1->x + obj1->width, obj1->y) ||
        pointInObject(obj2, obj1->x, obj1->y + obj1->height) || 
        pointInObject(obj2, obj1->x + obj1->width, obj1->y + obj1->height) || 
        pointInObject(obj1, obj2->x, obj2->y) || 
        pointInObject(obj1, obj2->x + obj2->width, obj2->y) ||
        pointInObject(obj1, obj2->x, obj2->y + obj2->height) || 
        pointInObject(obj1, obj2->x + obj2->width, obj2->y + obj2->height);
}
