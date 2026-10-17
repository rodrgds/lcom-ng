#ifndef PLATAFORMA_H
#define PLATAFORMA_H

#include <stdlib.h>
#include <time.h>
#include "video_gr.h"

typedef struct{
  unsigned short x;
  unsigned short y;
  double yspeed;
  double xspeed;
  unsigned short h,w;
  int movable;
  int boost;
  unsigned short color;
}Plataforma;

int updatePlataforma(Plataforma *plataforma);
#endif
