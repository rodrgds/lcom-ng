#include "../include/sprite.h"

sprite_t* 
sprite_new(const char* const* xpm, int x, int y) 
{
    sprite_t* new_sprite = (sprite_t*)malloc(sizeof(sprite_t));
    if (new_sprite == NULL) return NULL;

    enum xpm_image_type image_type = XPM_8_8_8;
    xpm_image_t* img = (xpm_image_t*)malloc(sizeof(xpm_image_t));
    if(img == NULL) return NULL; 
    img->bytes = xpm_load((xpm_map_t)xpm, image_type, img);

    new_sprite->x = x; 
    new_sprite->y = y; 
    new_sprite->width = img->width; 
    new_sprite->height = img->height; 
    new_sprite->img = img; 

    return new_sprite;
}

void
sprite_delete(sprite_t* sprite)
{
  free(sprite->img); 
  free(sprite); 
}

int
sprite_get_width(sprite_t* sprite)
{
  return sprite->width; 
}

int
sprite_get_height(sprite_t* sprite)
{
  return sprite->height; 
}

int 
sprite_get_xspeed(sprite_t* sprite)
{
  return sprite->xspeed; 
}

int 
sprite_get_yspeed(sprite_t* sprite)
{
  return sprite->yspeed; 
}

int 
sprite_get_x(sprite_t* sprite)
{
  return sprite->x; 
}

int 
sprite_get_y(sprite_t* sprite)
{
  return sprite->y; 
}

xpm_image_t*
sprite_get_image(sprite_t* sprite)
{
  return sprite->img; 
}

void
sprite_draw(sprite_t* sprite){
  vg_xpm_img_draw(sprite->img, sprite->x, sprite->y); 
}
