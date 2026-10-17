#include "../include/xpm.h"

xpm_t* 
xpm_new(xpm_map_t xpm, uint16_t x, uint16_t y) 
{
    xpm_t* new_xpm = (xpm_t*)malloc(sizeof(xpm_t));
    if (new_xpm == NULL) return NULL;

    enum xpm_image_type image_type = XPM_8_8_8;
    xpm_image_t img;
    new_xpm->map = xpm_load((xpm_map_t)xpm, image_type, &img);

    new_xpm->x = x;
    new_xpm->y = y;
    new_xpm->width = img.width;
    new_xpm->height = img.height;

    return new_xpm;
}

void 
xpm_delete(xpm_t* xpm)
{
    free(xpm->map);
    free(xpm);
}

void 
xpm_set_position(xpm_t* xpm, uint16_t x, uint16_t y)
{
    xpm->x = x;
    xpm->y = y;
}

uint16_t 
xpm_get_width(xpm_t* xpm)
{
    return xpm->width;
}

uint16_t 
xpm_get_height(xpm_t* xpm) 
{
    return xpm->height;
}


