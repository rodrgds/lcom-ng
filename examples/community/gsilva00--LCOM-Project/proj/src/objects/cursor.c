#include "cursor.h"

Cursor *create_cursor(xpm_map_t pic) {
  Cursor *cr = (Cursor *) malloc (sizeof(Cursor));
  if (cr == NULL) return NULL;

  xpm_image_t img;
  uint8_t *map = xpm_load(pic, XPM_8_8_8, &img);
  
  if (map == NULL) {
    free(cr);
    return NULL;
  }

  cr->img = img;
  cr->x = 400;
  cr->y = 300;
  
  return cr;
}

void destroy_cursor(Cursor *cr) {
  if (cr == NULL) return;
  free(cr);
  cr = NULL;
}

void set_coordinates(Cursor *cr, uint16_t x, uint16_t y, bool negative_x , bool negative_y) {
  if (cr == NULL) return;
  int32_t new_x = (int32_t)cr->x + (negative_x ? -(int32_t)x : (int32_t)x);
  int32_t new_y = (int32_t)cr->y + (negative_y ? -(int32_t)y : (int32_t)y);

  if (new_x < 0) new_x = 0;
  if (new_y < 0) new_y = 0;
  if (new_x > 799) new_x = 799;
  if (new_y > 599) new_y = 599;
  cr->x = (uint16_t)new_x;
  cr->y = (uint16_t)new_y;
}
