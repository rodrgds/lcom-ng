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
  // Rendering uses a copy so edge targets retain their original hit coordinates.
  Sprite cursor = *mouse->sprite;
  int min_x = cursor.width / 2, min_y = cursor.height / 2;
  int max_x = HEIGHT - (cursor.width - min_x);
  int max_y = WIDTH - (cursor.height - min_y);
  cursor.x = cursor.x < min_x ? min_x : cursor.x > max_x ? max_x : cursor.x;
  cursor.y = cursor.y < min_y ? min_y : cursor.y > max_y ? max_y : cursor.y;
  draw_sprite(&cursor);
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


