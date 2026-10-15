#include <lcom/lcf.h>
#include <lcom/xpm.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "../assets/xpm.h"
#include "sprite.h"
#include "graphics.h"

Sprite *create_sprite(xpm_map_t xpm, int x, int y)
{
  Sprite *sp = (Sprite *)malloc(sizeof(Sprite));
  xpm_image_t img;
  if (sp == NULL)
    return NULL;
  sp->map = xpm_load(xpm, XPM_8_8_8, &img);
  sp->x = x;
  sp->y = y;
  sp->height = img.height;
  sp->width = img.width;
  sp->xspeed = 0;
  sp->yspeed = 0;
  sp->angle = 0;

  if (sp->map == NULL)
  {
    free(sp);
    return NULL;
  }

  return sp;
}

int draw_sprite(Sprite *sp)
{
  uint8_t *colorPTR, *basePTR;
  int x, y, xf, yf;
  uint32_t color;
  colorPTR = sp->map;
  basePTR = sp->map;
  x = sp->x - (sp->width / 2);
  y = sp->y - (sp->height / 2);
  xf = sp->x + (sp->width / 2);
  yf = sp->y + (sp->height / 2);

  for (; y < yf; y++)
  {

    for (; x < xf; x++)
    {

      for (uint8_t i = 0; i < 3; i++)
      {
        color |= *(colorPTR) << (8 * i);
        colorPTR++;
      }
      if (color != CHROMA_KEY_GREEN_888)
      {
        if (x > 0 && y > 0 && x < 800 && y < 600)
        {
          fillPixel(x, y, color);
        }
      }
      color = 0;
    }
    x -= sp->width;
  }
  return 0;
}

void destroy_sprite(Sprite *sp)
{
  clear_sprite(sp);
  if (sp == NULL)
    return;

  if (sp->map)
    free(sp->map);

  free(sp);
  sp = NULL;
}
void clear_sprite(Sprite *sp)
{
  int x, y, xf, yf;

  x = sp->x - (sp->width / 2);
  y = sp->y - (sp->height / 2);
  xf = sp->x + (sp->width / 2);
  yf = sp->y + (sp->height / 2);
  for (; y < yf; y++)
  {
    for (; x < xf; x++)
    {
      fillPixel(x, y, 0x0);
    }
    x -= sp->width;
  }
}
void set_sprite_speed(Sprite *sprite, int xi, int yi, int xf, int yf, int speed)
{

  double pi = 3.14159265;
  int dx, dy, angle;
  dx = xf - xi;
  dy = yf - yi;
  angle = abs(atan2(dy, dx) * (180 / pi));
  sprite->xspeed = (abs(cos(atan2(dy, dx))) * speed);
  sprite->yspeed = (abs(sin(atan2(dy, dx))) * speed);
  sprite->angle = angle;
}

int update_sprite(Sprite *sprite, uint16_t xf, uint16_t yf)
{
  int intPerFrame = 2;
  if (interruptCounter % intPerFrame == 0)
  {
    if (sprite->x != xf || sprite->y != yf)
    {
      clear_sprite(sprite);
      sprite->x += sprite->xspeed;
      sprite->y += sprite->yspeed;

      if (sprite->xspeed > 0 && sprite->x > xf)
      {
        sprite->x = xf;
      }
      else if (sprite->xspeed < 0 && sprite->x < xf)
      {
        sprite->x = xf;
      }
      if (sprite->yspeed > 0 && sprite->y > xf)
      {
        sprite->y = yf;
      }
      else if (sprite->yspeed < 0 && sprite->y < xf)
      {
        sprite->y = yf;
      }

      draw_sprite(sprite);
    }
  }
  if (sprite->x == xf && sprite->y == yf)
  {
    return 0;
  }

  return 1;
}
