#include "mouse_sprite.h"

static Sprite sprite;

void mouse_sprite_create(mouse_sprite* mouse) {
  mouse->x_pos = MOUSE_START;
  mouse->y_pos = MOUSE_START;
  mouse->clicked = 0;
  sprite = create_sprite(mouse_xpm);
}

void mouse_sprite_handle_event(mouse_sprite* mouse, Event event) {
  if (event.type != MOUSE) return;
  mouse->x_pos += event.info.mouse.x_delta;
  if (mouse->x_pos < 0) mouse->x_pos = 0;
  if (mouse->x_pos > video_get_h_res()) mouse->x_pos = video_get_h_res();
  mouse->y_pos -= event.info.mouse.y_delta;
  if (mouse->y_pos < 0) mouse->y_pos = 0;
  if (mouse->y_pos > video_get_v_res()) mouse->y_pos = video_get_v_res();
  mouse->clicked = event.info.mouse.clicked;
}

void mouse_sprite_draw(mouse_sprite* mouse) {
  if (draw_sprite(sprite, mouse->x_pos, mouse->y_pos))
    return;
}

void mouse_sprite_step(mouse_sprite* mouse, Event event) {
  if (event.type == MOUSE)
    mouse_sprite_handle_event(mouse, event);
  mouse_sprite_draw(mouse);
}
