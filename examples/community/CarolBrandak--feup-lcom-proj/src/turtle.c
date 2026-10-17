#include "turtle.h"
#include "macros.h"
#include "graphics_card.h"

extern int xRes;

void initTurtle(Turtle * turtle, int count){
  srand(time(NULL));
  xpm_image_t img;
  xpm_load(turtle_xpm,XPM_8_8_8_8,&img);
  xpm_image_t img2;
  xpm_load(turtle2_xpm,XPM_8_8_8_8,&img2);
  xpm_image_t img3;
  xpm_load(turtle3_xpm,XPM_8_8_8_8,&img3);
  switch (count)
  {
  case 0:
    turtle->x=rand() % ((xRes/2)-img.width) + 1;
    turtle->y=225;
    turtle->speed = STARTING_TURTLE_SPEED;
    break;
  case 1:
    turtle->x=rand() % ((xRes/2)-img.width) + (xRes/2);
    turtle->y=345;
    turtle->speed = STARTING_TURTLE_SPEED;
    break;                

  default:
    break;
  }
  turtle->img1=img;
  turtle->img2=img2;
  turtle->img3=img3;
  turtle->underwater=false;
  turtle->dayTurtle=true;
}
