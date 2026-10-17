#include <lcom/lcf.h>
#include <stdint.h>

#include "mouse.h"

int packet_counter = 1;
int mouse_hook_id = MOUSE_IRQ;

uint8_t mouse_scancode;

Cursor *cursor;

extern uint32_t selected_color;
extern uint32_t background_color;
extern xpm_image_t frame;
extern bool eraser_on;
extern char selected_thickness;

xpm_image_t old_img;

void(mouse_ih)() {
  while (1) {
    uint8_t status;

    if (util_sys_inb(KBC_ST_REG, &status) != OK)
      return;

    if (status & KBC_OBFULL) {
      if ((status & KBC_CHECK_ERR) != OK)
        return;
      else if (util_sys_inb(KBC_OUT_BUF, &mouse_scancode) != OK) {
        printf("Error while reading from the output buffer\n");
        mouse_scancode = 0x00;
      }
      break;
    }
  }
}

int(mouse_disable_data_reporting)() {
  uint8_t ret;
  sys_outb(0x64, 0xD4);

  int terminate = 0;

  while (!terminate) {
    util_sys_inb(0x64, &ret);
    if (ret & BIT(1)) {
      tickdelay(micros_to_ticks(10000));
      continue;
    }
    else {
      sys_outb(0x60, 0xF5);
      terminate = 1;
    }
  }

  util_sys_inb(0x60, &ret);
  if (ret == 0xFA)
    return 0;
  else
    return -1;

  return 0;
}

int(mouse_subscribe_int)(uint8_t *bit_no) {
  *bit_no = mouse_hook_id;

  if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) != OK) {
    printf("Error while subscribing the mouse interruptions.\n");
    return 1;
  }

  return 0;
}

int(mouse_unsubscribe_int)() {
  if (sys_irqrmpolicy(&mouse_hook_id) != OK) {
    printf("Error while unsubscribing the mouse interruptions.\n");
    return 1;
  }

  return 0;
}

int(mouse_read_out_buf)(uint8_t *byte) {
  if (util_sys_inb(KBC_OUT_BUF, byte) != OK) {
    printf("Error while reading from the output buffer\n");
    return 1;
  }
  return 0;
}

int(mouse_parse_packet)(struct packet *pp) {
  pp->rb = pp->bytes[0] & RIGHT_BUTTON;
  pp->lb = pp->bytes[0] & LEFT_BUTTON;
  pp->mb = pp->bytes[0] & MIDDLE_BUTTON;

  pp->x_ov = pp->bytes[0] & X_OVF;
  pp->y_ov = pp->bytes[0] & Y_OVF;

  if (pp->bytes[0] & MSB_DELTA_X)
    pp->delta_x = pp->bytes[1] | 0xFF00;

  else
    pp->delta_x = pp->bytes[1];

  if (pp->bytes[0] & MSB_DELTA_Y)
    pp->delta_y = pp->bytes[2] | 0xFF00;

  else
    pp->delta_y = pp->bytes[2];

  return 0;
}

void(new_cursor)(uint16_t x, uint16_t y) {
  cursor = (Cursor *) malloc(sizeof(Cursor));
  if (cursor == NULL)
    return;

  cursor->x = x;
  cursor->y = y;

  cursor->isOverOption = false;

  xpm_load(cursor_xpm, XPM_8_8_8_8, &(cursor->img));
  xpm_load(s_eraser_cursor_xpm, XPM_8_8_8_8, &(cursor->eraser_img));
  xpm_load(select_cursor_xpm, XPM_8_8_8_8, &(cursor->select_img));

  old_img = cursor->img;
}

void(draw_cursor)() {
  xpm_image_t new_img = (eraser_on) ? cursor->eraser_img : cursor->img;
  new_img = (cursor->isOverOption) ? cursor->select_img : new_img;

  draw_pixmap(cursor->x, cursor->y, &new_img, false);

  old_img = new_img;
}

void(delete_prev_frame)() {
  uint32_t *prev_frame = (uint32_t *) frame.bytes;

  for (uint16_t i = 0; i < old_img.width; i++) {
    for (uint16_t j = 0; j < old_img.height; j++) {
      if (*(prev_frame + i + j * frame.width) != TRANSPARENCY)
        vg_draw_pixel(cursor->x + i, cursor->y + j, *(prev_frame + (cursor->x + i) + (cursor->y + j) * frame.width), true);
    }
  }
}

void(mouse_draw_pixel)(uint32_t selected_color, char width, bool addToBackgroundFrame) {
  if (cursor->x < 336)
    return;

  int expanding_factor = 3;

  if (width == 'M')
    expanding_factor = 6;
  else if (width == 'L')
    expanding_factor = 9;

  if (eraser_on)
    expanding_factor *= 3;

  my_vg_draw_rectangle(cursor->x, cursor->y, expanding_factor, expanding_factor, selected_color, addToBackgroundFrame);
}

void(update_cursor)(struct packet *pp) {
  if (eraser_on) {
    switch (selected_thickness) {
      case 'S': {
        xpm_load(s_eraser_cursor_xpm, XPM_8_8_8_8, &(cursor->eraser_img));
        break;
      }
      case 'M': {
        xpm_load(m_eraser_cursor_xpm, XPM_8_8_8_8, &(cursor->eraser_img));
        break;
      }
      case 'L': {
        xpm_load(l_eraser_cursor_xpm, XPM_8_8_8_8, &(cursor->eraser_img));
        break;
      }
      default:
        break;
    }
  }
  delete_prev_frame();
  update_cursor_position(pp);
  draw_cursor();
}

void(update_cursor_position)(struct packet *pp) {
  int screen_width, screen_height;
  get_screen_dimensions(&screen_width, &screen_height);

  xpm_image_t cursor_img = (eraser_on) ? cursor->eraser_img : cursor->img;
  cursor_img = (cursor->isOverOption) ? cursor->select_img : cursor_img;

  uint16_t prev_x = cursor->x;
  uint16_t prev_y = cursor->y;

  /* Update coordinates */
  cursor->x += pp->delta_x;
  cursor->y -= pp->delta_y;

  if (prev_x + pp->delta_x < 0)
    cursor->x = 0;

  else if (cursor->x + cursor_img.width > screen_width)
    cursor->x = screen_width - cursor_img.width;

  if (prev_y - pp->delta_y < 0)
    cursor->y = 0;

  // The same thing for the lower limit
  else if (cursor->y + cursor_img.height > screen_height)
    cursor->y = screen_height - cursor_img.height;
}

Collision(cursor_collisions_in_word_selection)() {
  int width = 410, height = 44;

  if (cursor->x > 499 && cursor->x < 499 + width && cursor->y > 466 && cursor->y < 474 + height){
    cursor->isOverOption = true;
    return OVER_WORD_1;
  }
  else if (cursor->x > 499 && cursor->x < 499 + width && cursor->y > 555 && cursor->y < 555 + height){
    cursor->isOverOption = true;
    return OVER_WORD_2;
  }
  else if (cursor->x > 499 && cursor->x < 499 + width && cursor->y > 644 && cursor->y < 644 + height){
    cursor->isOverOption = true;
    return OVER_WORD_3;
  }

  cursor->isOverOption = false;
  return NO_COLLISION;

}


Collision(cursor_collisions_in_game)() {
  /* Color black */
  int width = 60, height = 60, box_size = 20;

  if ((cursor->x > 25) && (cursor->x < 25 + width) && (cursor->y > 640) && (cursor->y < 640 + height)){
    cursor->isOverOption = true;
    return OVER_COLOR_1;
  }

  else if ((cursor->x > 100) && (cursor->x < 100 + width) && (cursor->y > 640) && (cursor->y < 640 + height)){
    cursor->isOverOption = true;
    return OVER_COLOR_2;
  }

  else if ((cursor->x > 175) && (cursor->x < 175 + width) && (cursor->y > 640) && (cursor->y < 640 + height)){
    cursor->isOverOption = true;
    return OVER_COLOR_3;
  }

  else if ((cursor->x > 250) && (cursor->x < 250 + width) && (cursor->y > 640) && (cursor->y < 640 + height)){
    cursor->isOverOption = true;
    return OVER_COLOR_4;
  }

  else if ((cursor->x > 57) && (cursor->x < 57 + width) && (cursor->y > 716) && (cursor->y < 716 + height)){
    cursor->isOverOption = true;
    return OVER_COLOR_5;
  }

  else if ((cursor->x > 132) && (cursor->x < 132 + width) && (cursor->y > 716) && (cursor->y < 716 + height)){
    cursor->isOverOption = true;
    return OVER_COLOR_6;
  }

  else if ((cursor->x > 206) && (cursor->x < 206 + width) && (cursor->y > 716) && (cursor->y < 716 + height)){
    cursor->isOverOption = true;
    return OVER_COLOR_7;
  }

  else if ((cursor->x > 200) && (cursor->x < 300) && (cursor->y > 872) && (cursor->y < 972)){
    cursor->isOverOption = true;
    return OVER_ERASER;
  }

  else if ((cursor->x > 40) && (cursor->x < 40 + box_size) && (cursor->y > 860) && (cursor->y < 860 + box_size)){
    cursor->isOverOption = true;
    return OVER_S;
  }

  else if ((cursor->x > 40) && (cursor->x < 40 + box_size) && (cursor->y > 900) && (cursor->y < 900 + box_size)){
    cursor->isOverOption = true;
    return OVER_M;
  }

  else if ((cursor->x > 40) && (cursor->x < 40 + box_size) && (cursor->y > 940) && (cursor->y < 940 + box_size)){
    cursor->isOverOption = true;
    return OVER_L;
  }

  cursor->isOverOption = false;
  return NO_COLLISION;
}

Collision(cursor_collisions_option_selection)() {
  int height = 48, width = 200;

  if ((cursor->x > 606) && (cursor->x < 606 + width) && (cursor->y > 485) && (cursor->y < 485 + height)){
    cursor->isOverOption = true;
    return OVER_PLAY_AGAIN;
  }
  else if ((cursor->x > 606) && (cursor->x < 606 + width) && (cursor->y > 557) && (cursor->y < 557 + height)){
    cursor->isOverOption = true;
    return OVER_PLAY_SWITCH;
  }

  else if ((cursor->x > 606) && (cursor->x < 606 + width) && (cursor->y > 629) && (cursor->y < 629 + height)){
    cursor->isOverOption = true;
    return OVER_EXIT_GAME;
  }

  cursor->isOverOption = false;
  return NO_COLLISION;
}

void(handle_mouse_collision_with_colors)(uint32_t *new_color, Collision collision) {

  switch (collision) {
    case OVER_COLOR_1:
      *new_color = get_color_rgb(collision);
      break;

    case OVER_COLOR_2:
      *new_color = get_color_rgb(collision);
      break;

    case OVER_COLOR_3:
      *new_color = get_color_rgb(collision);
      break;

    case OVER_COLOR_4:
      *new_color = get_color_rgb(collision);
      break;

    case OVER_COLOR_5:
      *new_color = get_color_rgb(collision);
      break;

    case OVER_COLOR_6:
      *new_color = get_color_rgb(collision);
      break;

    case OVER_COLOR_7:
      *new_color = get_color_rgb(collision);
      break;

    case OVER_ERASER:
      *new_color = background_color;
      break;

    case NO_COLLISION:
      *new_color = selected_color;
      break;

    default:
      break;
  }
}
