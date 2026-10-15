#include "cursor.h"
#include "mouse.h"

struct cursor_imp {
  uint16_t x, y; // position in the screen
  Sprite* img; // pointer to the sprite
  bool button_pressed[3]; // array of key pressed {left, middle, right}
  uint16_t screen_width; // width of the screen
  uint16_t screen_height; // height of the screen
};

uint16_t get_cursor_Xpos(Cursor *c) {
  if (c == NULL) {
    return 0;
  }
  return c->x;
}

uint16_t get_cursor_Ypos(Cursor *c) {
  if (c == NULL) {
    return 0;
  }
  return c->y;
}

bool get_cursor_button_pressed(Cursor *c, uint8_t button) {
  if (c == NULL || button > 2) {
    return false;
  }
  return c->button_pressed[button];
}

void reset_cursor_button_pressed(Cursor *c) {
  if (c == NULL) return;
  memset(c->button_pressed, 0, sizeof(c->button_pressed));
}

Cursor *create_cursor(Sprite *img, uint16_t screen_width, uint16_t screen_height) {
  Cursor *c = malloc(sizeof(Cursor));
  if (c == NULL) {
    return NULL;
  }
  c->x = screen_width / 2;
  c->y = screen_height / 2;
  c->img = img;
  c->screen_width = screen_width;
  c->screen_height = screen_height;
  memset(c->button_pressed, 0, sizeof(c->button_pressed));

  return c;
}

void destroy_cursor(Cursor *c) {
  if (c == NULL) return;

  free(c);
}

void update_cursor_with_mouse(Cursor *cursor, struct packet *mouse_packet) {
  if (cursor == NULL || mouse_packet == NULL) return;

  cursor->button_pressed[0] = mouse_packet->lb;
  cursor->button_pressed[1] = mouse_packet->mb;
  cursor->button_pressed[2] = mouse_packet->rb;

  // Ignore overflow
  if (mouse_packet->x_ov || mouse_packet->y_ov) return;

  int16_t new_x = cursor->x + mouse_packet->delta_x;
  int16_t new_y = cursor->y - mouse_packet->delta_y; 

  if (new_x < 0) {
    cursor->x = 0;
  } else if (new_x > cursor->screen_width - get_sprite_width(cursor->img) - 1) {
    cursor->x = cursor->screen_width - get_sprite_width(cursor->img) - 1;
  }else {
    cursor->x = new_x;
  }
  
  if (new_y < 0) {
    cursor->y = 0;
  } else if (new_y > cursor->screen_height - get_sprite_height(cursor->img) - 1) {
    cursor->y = cursor->screen_height - get_sprite_height(cursor->img) - 1;
  } else {
    cursor->y = new_y;
  }
}

void draw_cursor(Cursor *c) {
  if (c == NULL) return;

  draw_sprite(c->img, c->x, c->y);
}
