#include "../include/fruit_view.h"

int 
fruit_draw(fruit_t* fruit)
{
  if (vg_draw_rectangle(fruit->position->x * 20, fruit->position->y * 20, 20, 20, 0xff0000)) return 1;
  return 0;
}

