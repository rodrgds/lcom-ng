#include "plataforma.h"

int updatePlataforma(Plataforma *plataforma){

  if(plataforma->y+plataforma->h+plataforma->yspeed > v_res-plataforma->h-1){
    plataforma->y = plataforma->h;
    plataforma->x = rand() % (h_res-plataforma->w);
    //plataforma->x = h_res/2;
    plataforma->boost = 1;
    //plataforma->movable = 1;
    plataforma->movable = rand () % 4;
    if(plataforma->movable != 0)
      //plataforma->boost = 1;
     plataforma->boost = rand () % 10;

    plataforma->xspeed = rand() % 5 - 2;
  }
  if(plataforma->movable == 0 && plataforma->xspeed == 0)
    plataforma->xspeed = rand() % 5 - 2;

  if(plataforma->x+plataforma->w+plataforma->xspeed > h_res){
    plataforma->x = h_res-plataforma->w;
    plataforma->xspeed*=-1;
  }
  if(plataforma->x+plataforma->xspeed < 0){
    plataforma->x=0;
    plataforma->xspeed *= -1;
  }

  if(plataforma->movable == 0){
    plataforma->x += plataforma->xspeed;
    plataforma->color = rgb(0, 100, 255);
  }
  else if(plataforma->boost == 0) plataforma->color = rgb(0, 255, 100);
  else plataforma->color = rgb(255, 255, 0);

  plataforma->y += plataforma->yspeed;
  if(plataforma->movable){
    plataforma->x += plataforma->xspeed;
    plataforma->color = 11;
  }
  else plataforma->color = 6;
}
