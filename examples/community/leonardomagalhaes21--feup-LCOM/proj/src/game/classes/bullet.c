#include "bullet.h"
#include <math.h>

bullet *createBullet(int x,int y, int mouseX, int mouseY,int damage,Sprite *sp){
    bullet *p = (bullet *)malloc(sizeof(*p));
    if(p == NULL)
        return NULL;
    p->x = x;
    p->y = y;
    p->mouseX = mouseX;
    p->mouseY = mouseY;
    p->damage = damage;
    p->sprite = sp;
    p->dx = mouseX - x;
    p->dy = mouseY - y;
    
    float norm = sqrt(p->dx * p->dx + p->dy * p->dy);
    if (norm != 0) { 
        p->dx = p->dx / norm;
        p->dy = p->dy / norm;
    }

    return p;
}

void destroyBullets(bullet *p){
    if(p->x < 0 || p->x > 1024 || p->y < 0 || p->y > 768)
        free(p);
}

void moveBullet(bullet *p){
    int speed = 15;
    p->x += speed* p->dx;
    p->y += speed* p->dy;
}


