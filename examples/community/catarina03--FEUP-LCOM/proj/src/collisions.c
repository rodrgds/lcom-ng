#include <lcom/lcf.h>
#include "collisions.h"


Area_Collision* newAreaCollision(int x0,int x1,int y0,int y1){
    Area_Collision* a=(Area_Collision*)malloc(sizeof(Area_Collision));
    a->x0=x0;
    a->x1=x1;
    a->y0=y0;
    a->y1=y1;
    return a;
}


void deleteAreaCollision(Area_Collision* ac){
    free(ac);
}


int check_collision(Area_Collision *a1,Area_Collision *a2){
    return !(a2->x0>a1->x1||a2->x1<a1->x0||a2->y0>a1->y1||a2->y1<a1->y0);
}

