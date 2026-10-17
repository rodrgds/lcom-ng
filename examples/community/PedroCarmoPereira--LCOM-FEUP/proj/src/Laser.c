#include "stdio.h"
#include <lcom/lcf.h>
#include "video.h"
#include "Laser.h"
#include <math.h>

void freeLasers(LaserVector *v){
  if (!(*v)) return;

  if (isOffScreen((*v)->curr.e)){
    LaserVector tmp = (*v);
    *v = (*v)->next;
    free(tmp);
    freeLasers(v);
  }

  else freeLasers(&(*v)->next);
}

void addLaser(LaserVector * v, Laser *l){
  LaserVector node = (LaserVector) malloc(sizeof(struct laserVector));
  node->curr = *l;
  node->next = NULL;

  if(*v == NULL){
    *v = node;
    return;
  }

  LaserVector iterator = *v;
  while(iterator->next) iterator = iterator->next;
  iterator->next = node;
}

Laser laser(int xi, int yi, int xf, int yf, int dmg, bool friendly, float angle){
  Laser l;

  if (friendly == true){
  l.e = entity(LASERF);
  l.e.p.y = yi - 30;
  float deltaX;
  float deltaY;
  l.e.xspeed = 20;
  deltaX = xf - xi;
  deltaY = yf - yi;
  l.angle = atan2(deltaY, deltaX);
  l.e.yspeed = 20;
  }

  else{
    l.e = entity(LASERE);
    l.e.p.y = yi + 30;
    l.e.xspeed = 10;
    l.dmg = dmg;
    l.angle = angle;
    l.e.yspeed = 10;
  }

  l.e.p.x = xi;
  l.friendly = friendly;
  return l;
}
