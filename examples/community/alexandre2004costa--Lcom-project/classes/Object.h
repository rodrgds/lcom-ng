#ifndef __Object_h
#define __Object_h



#include "Sprite.h"

typedef enum{
  MENU,
  SCORE,
  MULTI,
  PLAY,
  OVER,
  CLOSE
}GameState;

typedef struct {
    int xspeed, yspeed; // current speed
    Sprite *sprite;
} Object;

void drawObject(Object *Object);

Object *create_object(xpm_map_t xpm, int x, int y);


#endif
