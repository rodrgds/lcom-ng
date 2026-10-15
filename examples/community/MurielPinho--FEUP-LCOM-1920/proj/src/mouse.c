#include <lcom/lcf.h>

#include "graphics.h"
#include "sprite.h"
#include "mouse.h"
#include "macros.h"

Mouse *create_mouse(uint16_t x, uint16_t y, Sprite *mouse_sprite)
{

  Mouse *mouse = (Mouse *)malloc(sizeof(Mouse));
  if (mouse == NULL)
    return NULL;
  mouse->sprite = mouse_sprite;
  mouse->sprite->x = x;
  mouse->sprite->y = y;
  mouse->lb_status = 0;
  return mouse;
}

void delete_mouse(Mouse *mouse)
{

  if (mouse == NULL)
    return;
  destroy_sprite(mouse->sprite);
  free(mouse);
  mouse = NULL;
}

void draw_mouse(Mouse *mouse)
{
  draw_sprite(mouse->sprite);
}

void setMouseX(Mouse *mouse, uint16_t x)
{
  if (x >= 0 && x <= HEIGHT)
  {
    mouse->sprite->x = x;
  }
}

void setMouseY(Mouse *mouse, uint16_t y)
{
  if (y >= 0 && y <= WIDTH)
  {
    mouse->sprite->y = y;
  }
}


