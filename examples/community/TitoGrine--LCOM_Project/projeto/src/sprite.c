#include "sprite.h"

extern void *video_mem;
extern char previous[];
extern char buffer[];
extern uint16_t hres;
extern uint16_t vres;

Sprite *create_sprite(const char *pic[], int x, int y, int xspeed, int yspeed) {
  //allocate space for the "object"
  xpm_image_t xpm;

  Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
  if (sp == NULL)
    return NULL;

  sp->map = (char *) xpm_load(pic, XPM_8_8_8_8, &xpm);

  if (sp->map == NULL) {
    free(sp);
    return NULL;
  }
  sp->width = xpm.width;
  sp->height = xpm.height;
  sp->x = x;
  sp->y = y;
  sp->xspeed = xspeed;
  sp->yspeed = yspeed;
  return sp;
}

void destroy_sprite(Sprite *sp) {
  if (sp == NULL)
    return;
  if (sp->map)
    free(sp->map);
  free(sp);
  sp = NULL; // XXX: pointer is passed by value
  // should do this @ the caller
}

void move_xpm(Sprite *sp) {
  sp->x += sp->xspeed;
  sp->y += sp->yspeed;
}

void change_position(Sprite* sp, uint16_t x_pos, uint16_t y_pos) {
  sp->x = x_pos;
  sp->y = y_pos;
}

void draw_Sprite(Sprite *sp) {
  int x = sp->x;
  int y = sp->y;

  uint32_t *ptr = (uint32_t *) sp->map;

  for (int height = 0; height < sp->height; height++) {

    for (int width = 0; width < sp->width; width++) {
      // Checks if the pixel is meant to be painted, i.e is not transparent
      if (*ptr != TRANSPARENCY_COLOR_8_8_8_8) {
        paint_pixel(x + width, y + height, *ptr);
      }
      ptr++;
    }
  }
}

void paint_Sprite(Sprite* sp, int color){
  int x = sp->x;
  int y = sp->y;

  uint32_t *ptr = (uint32_t *) sp->map;

  for (int height = 0; height < sp->height; height++) {

    for (int width = 0; width < sp->width; width++) {
      // Checks if the pixel is meant to be painted, i.e is not transparent
      if (*ptr != TRANSPARENCY_COLOR_8_8_8_8) {
        // Does not paint the actual value in the sprite, but the color passed instead
        paint_pixel(x + width, y + height, color);
      }
      ptr++;
    }
  }
}

void erase_dynamic(Sprite* sp, xpm_string_t *wallpaper){
  int x = sp->x;
  int y = sp->y;
  xpm_image_t xpm;

  // get the pix map from the XPM
  uint32_t *map = (uint32_t *) xpm_load(wallpaper, XPM_8_8_8_8, &xpm) + (x + y * hres);

  for (int height = 0; height < sp->height; height++) {

    for (int width = 0; width < sp->width; width++) {
      paint_pixel(x + width, y + height, *map);
      map++;
    }

    map += (hres - sp->width);
  }
}


void erase_Sprite(Sprite *sp) {
  int x = sp->x;
  int y = sp->y;
  xpm_image_t xpm;

  // Get the pix map from the XPM
  uint32_t *map = (uint32_t *) xpm_load(background, XPM_8_8_8_8, &xpm) + (x + y * hres);

  for (int height = 0; height < sp->height; height++) {

    for (int width = 0; width < sp->width; width++) {
      paint_pixel(x + width, y + height, *map);
      map++;
    }

    map += (hres - sp->width);
  }
}

void move_cursor(Sprite *sp, int16_t move_x, int16_t move_y, int color) {

  erase_cursor(sp);

  sp->x += move_x;

  sp->y += move_y;

  draw_cursor(sp, color);
}

void draw_cursor(Sprite *sp, int color) {
  int x = sp->x;
  int y = sp->y;

  uint32_t *ptr = (uint32_t *) sp->map;

  for (int height = 0; height < sp->height; height++) {

    for (int width = 0; width < sp->width; width++) {
      // Checks if color is transparent and if the coordenates are within limits
      if (*ptr != TRANSPARENCY_COLOR_8_8_8_8 && x + width < hres && x + width > 0 && y + height < vres && y + height > 0) {
        // Checks if the color is 0xFFFFFE, if so it paints the color passed as argument, otherwise it paints the sprite color
        if(*ptr == 0xFFFFFE)
          paint_pixel_cursor(x + width, y + height, color);
        else
          paint_pixel_cursor(x + width, y + height, *ptr);
      }

      ptr++;
    }
  }
}

void erase_cursor(Sprite *sp) {
  int x = sp->x;
  int y = sp->y;
  uint32_t *ptr = (uint32_t *) previous + (x + y * hres);

  for (int height = 0; height < sp->height; height++) {

    for (int width = 0; width < sp->width; width++) {
      paint_pixel(x + width, y + height, *ptr);
      ptr++;
    }

    ptr += (hres - sp->width);
  }
}

void fill_displacement(Sprite *sp, int16_t delta_x, int16_t delta_y, int color) {

  //If the displacement is to short it still draws the sprite in the initial position
  paint_Sprite(sp, color);

  //Calculating the number of iterations
  unsigned long iterations = (abs(delta_x) > abs(delta_y)) ? abs(delta_x) : abs(delta_y);

  //How much each axis increases per iteration
  double x_increase = (double) delta_x / iterations;
  double y_increase = (double) delta_y / iterations;

  //Using decimal variables to be able to increase with decimal steps
  double x = sp->x;
  double y = sp->y;

  for (unsigned long i = 0; i < iterations; ++i) {

    //Incrementing or decrementing x and y
    x += x_increase;
    y += y_increase;

    sp->x = (int) trunc(x);
    sp->y = (int) trunc(y);

    paint_Sprite(sp, color);
  }
}

void spray_area(uint16_t x_pos, uint16_t y_pos, int16_t delta_x, int16_t delta_y, int color) {

  random_fill(x_pos, y_pos, color);

  //Calculating the number of iterations
  unsigned long iterations = (abs(delta_x) > abs(delta_y)) ? abs(delta_x) : abs(delta_y);

  //How much each axis increases per iteration
  double x_increase = (double) delta_x / iterations;
  double y_increase = (double) delta_y / iterations;

  //Using decimal variables to be able to increase with decimal steps
  double x = x_pos;
  double y = y_pos;

  for (unsigned long i = 0; i < iterations; ++i) {

    //Incrementing or decrementing x and y
    x += x_increase;
    y += y_increase;

    x_pos = (int) trunc(x);
    y_pos = (int) trunc(y);

    random_fill(x_pos, y_pos, color);
  }
}

void random_fill(uint16_t x_pos, uint16_t y_pos, int color) {
  xpm_image_t xpm;
  uint array_color = 0;
  xpm_load(circle, XPM_8_8_8_8, &xpm);

  for (int y = 0; y < 17; y++) {

    for (int x = 0; x < 17; x++) {

      for (int i = 3; i >= 0; i--)
      // Checks what color is in the circle xmp in current position
        array_color = (array_color << 8) | (uint8_t)(xpm.bytes[(x + y * 17) * 4 + i]);

      // If the random number is 0 it paints the pixel, otherwise it doesn't do anything
      if (array_color != TRANSPARENCY_COLOR_8_8_8_8 && !(rand() % 50)) {
        paint_pixel(x_pos + x, y_pos + y, color);
      }
    }
  }
}

void fill_area(uint16_t x_pos, uint16_t y_pos, int color) {

  // Gets the initial background's pixel color
  int fl_color = get_color(x_pos, y_pos);

  // Checks if color to be painted is not already the initial pixel color, avoiding infinite loop
  if (color != fl_color) {

    queue *pixels = new_queue(1);
    pair temp = {x_pos, y_pos};
    pair pix = {x_pos, y_pos};

    push(pixels, temp);

    while (!is_empty(pixels)) {
      pop(pixels, &temp);

    // Checks if current pixel as a different color then the one to be painted
      if (check_pixel(temp.x, temp.y, fl_color)) {
        paint_pixel(temp.x, temp.y, color);

        // Checks pixel to the left
        if (check_pixel(temp.x - 1, temp.y, fl_color)) {
          pix.x = temp.x - 1;
          pix.y = temp.y;

          push(pixels, pix);
        }

        // Checks upper pixel
        if (check_pixel(temp.x, temp.y - 1, fl_color)) {
          pix.x = temp.x;
          pix.y = temp.y - 1;

          push(pixels, pix);
        }

        // Checks pixel to the right
        if (check_pixel(temp.x + 1, temp.y, fl_color)) {
          pix.x = temp.x + 1;
          pix.y = temp.y;

          push(pixels, pix);
        }

        // Checks bottom pixel
        if (check_pixel(temp.x, temp.y + 1, fl_color)) {
          pix.x = temp.x;
          pix.y = temp.y + 1;

          push(pixels, pix);
        }
      }
    }
    delete_queue(pixels);

    double_buffer();
  }
}

bool check_pixel(uint16_t x_pos, uint16_t y_pos, int fl_color) {
  if (x_pos > 0 && x_pos < hres && y_pos > 0 && y_pos < vres && get_color(x_pos, y_pos) == fl_color) {
    return true;
  }

  return false;
}

bool check_limits(Sprite *sp, uint16_t x_pos, uint16_t y_pos) {
  if (x_pos + sp->width < hres && x_pos > 0 && y_pos + sp->height < vres && y_pos > 0) {
    return true;
  }

  return false;
}

bool button_hovering(Sprite *button, uint16_t x_pos, uint16_t y_pos) {

  int x_inf = button->x;
  int x_sup = button->x + button->width;

  int y_inf = button->y;
  int y_sup = button->y + button->height;

  return (x_pos > x_inf && x_pos < x_sup && y_pos > y_inf && y_pos < y_sup);
}

bool pallete_hovering(Sprite *pallete, uint16_t x_pos, uint16_t y_pos){
  // Borders are 10 pixels wide, so it needs to be accounted for
  int border = 10;

  int x_inf = pallete->x + border;
  int x_sup = pallete->x + pallete->width - border;

  int y_inf = pallete->y + border;
  int y_sup = pallete->y + pallete->height - border;

  return (x_pos > x_inf && x_pos < x_sup && y_pos > y_inf && y_pos < y_sup);
}

bool in_canvas(Sprite *canvas, Sprite *brush, uint16_t x_pos, uint16_t y_pos) {
  // Borders are 10 pixels wide, so it needs to be accounted for
  int border = 10;

  int x_inf = canvas->x + border;
  int x_sup = canvas->x + canvas->width - brush->width - border;

  int y_inf = canvas->y + border;
  int y_sup = canvas->y + canvas->height - brush->height - border;

  return (x_pos > x_inf && x_pos < x_sup && y_pos > y_inf && y_pos < y_sup);
}

int16_t calc_speed_x(Sprite *sp, int16_t x_speed) {
  if (sp->x + sp->width + x_speed < hres && sp->x + x_speed > 0)
    return x_speed;
  else if (sp->x + x_speed <= 0) // Left side limit trespassed
    return -sp->x;
  else                           // Right side limit trespassed
    return hres - sp->x - sp->width;
}

int16_t calc_speed_y(Sprite *sp, int16_t y_speed) {
  if (sp->y + sp->height + y_speed < vres && sp->y + y_speed > 0)
    return y_speed;
  else if (sp->y + y_speed <= 0) // Upper side limit trespassed
    return -sp->y;
  else                           // Bottom side limit trespassed
    return vres - sp->y - sp->height;
}

