#include "assets/assets.h"
#include "assets/writtenAssets.h"
#include "Entity.h"
#include "video.h"

Entity entity(EntityType t){
  Entity e;
  setPosition(&e, 0, 0);
  e.xspeed = 0;
  e.yspeed = 0;
  e.img.type = XPM_8_8_8_8;
  e.type = t;
  switch(t){
    case 0:
      if(xpm_load(playerS, XPM_8_8_8_8, &e.img) == NULL){
        puts("Error loading xpm");
        break;
      }
      e.xspeed = 20;
      e.yspeed = 20;
      e.p.x = getHRes() / 2 - e.img.width / 2;
      e.p.y = getVRes() - e.img.height - 10;
      break;
    case 1:
      if(xpm_load(crosshair, XPM_8_8_8_8, &e.img) == NULL){
        puts("Error loading xpm");
        break;
      }
      setPosition(&e, getHRes()/2 - e.img.width/2, getVRes()/2 - e.img.height/2);
      break;
    case 2:
      if (xpm_load(Enemy1, XPM_8_8_8_8, &e.img) == NULL) puts("Error loading xpm");
      break;
    case 3:
      if (xpm_load(Enemy2, XPM_8_8_8_8, &e.img) == NULL) puts("Error loading xpm");
      break;
    case 4:
      if (xpm_load(Boss, XPM_8_8_8_8, &e.img) == NULL) puts("Error loading xpm");
      break;
    case 5:
      if (xpm_load(asteroid, XPM_8_8_8_8, &e.img) == NULL){
        puts("Error loading xpm");
        break;
      }
      e.xspeed = 5;
      e.yspeed = 0;
      break;
    case 6:
      if(xpm_load(laserP, XPM_8_8_8_8, &e.img) == NULL) puts("Error loading xpm");
      break;
    case 7:
      if(xpm_load(laserE, XPM_8_8_8_8, &e.img) == NULL) puts("Error loading xpm");
      break;
    case 8:
      if(xpm_load(play_xpm, XPM_8_8_8_8, &e.img) == NULL) puts("Error loading xpm");
      setPosition(&e, getHRes()/2 - e.img.width/2, getVRes()/2 - e.img.height);
      break;
    case 9:
      if(xpm_load(highscores_xpm, XPM_8_8_8_8, &e.img) == NULL) puts("Error loading xpm");
      setPosition(&e, getHRes()/2 - e.img.width/2, getVRes()/2 - e.img.height/10 + 20);
      break;
    case 10:
      if(xpm_load(quit_xpm, XPM_8_8_8_8, &e.img) == NULL) puts("Error loading xpm");
      setPosition(&e, getHRes()/2- e.img.width/2, getVRes()/2 + 100 + e.img.height * 3/2);
      break;
    case 11:
      if(xpm_load(mainmenu_xpm, XPM_8_8_8_8, &e.img) == NULL) puts("Error loading xpm");
      setPosition(&e, getHRes()/2 - e.img.width/2, getVRes()/2 + 100 - e.img.height/10 + 20);
      break;
    default:
        break;
  }
  return e;
}

void setX(Entity *e, int nx){
  if (nx > getHRes() - e->img.width) nx = getHRes() - e->img.width;
  if (nx < 0) nx = 0;
  e->p.x = nx;

}

void setY(Entity *e, int ny){
  if (ny > getVRes() - e->img.height) ny = getVRes() - e->img.height;
  if (ny < 0) ny = e->img.height;
  e->p.y = ny;
}

void setPosition(Entity *e, int nx, int ny){
  setX(e, nx);
  setY(e, ny);
}

void setSpeed(Entity *e, int8_t xs, int8_t ys){e->xspeed = xs; e->yspeed = ys;}

void drawEntity(Entity *e){
  draw_sproot(e->img, e->p.x, e->p.y);
}


bool colision(const Entity *e1, const Entity *e2){

  if(e1->p.x < e2->p.x + e2->img.width &&
     e1->p.x + e1->img.width > e2->p.x &&
     e1->p.y < e2->p.y + e2->img.height &&
     e1->p.y + e1->img.height > e2->p.y) return true;

  return false;
}


bool isOffScreen(Entity e){
  if (e.p.y + e.img.height >= getVRes() || e.p.y <= 25) return true;
  int rx = e.p.x + e.img.width;
  int lx = e.p.x;
  if (lx == 0 || rx >= getHRes()) return true;
  return false;
}
