#include "mouse.h"
#include "../../assets/mouse_pixmap.h"
#include "../../game/categories.h"
#include "../../game/game_state.h"
#include "../graphics/graphics.h"

int mouse_hook_id = 3;
struct packet mouse_packet;
uint8_t packet_byte_index = 0;
uint8_t mouse_bytes[3];
uint8_t curr_mouse_byte;
static xpm_image_t mouse_img;
uint8_t *mouse_pixmap;
extern char *mouse_pointer_xpm[];
extern uint16_t h_res;
extern uint16_t v_res;
int mouse_x = 150;                        // Initial mouse position X
int mouse_y = 150;                        // Initial mouse position Y
static uint32_t saved_background[32][32]; // Buffer to save the background under the mouse pointer

extern int menu_selected_option;

int(mouse_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) {
    return 1;
  }

  *bit_no = BIT(mouse_hook_id); // Save the bitmask for the interrupt

  return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

int(mouse_unsubscribe_int)() {
  return (sys_irqrmpolicy(&mouse_hook_id));
}

void(mouse_ih)() {
  uint8_t status;

  if (read_controller(STAT_REG, &status, 1) != 0) {
    return;
  }

  if ((status & OUT_BUF_FULL) && (status & AUX)) {
    if (read_controller(OUT_BUF, &curr_mouse_byte, 1) != 0) {
      return;
    }
  }
  else {
  }
}

void(buffer_mouse_bytes)() {
  if ((curr_mouse_byte & BIT(3)) && packet_byte_index == 0) {
    mouse_bytes[packet_byte_index++] = curr_mouse_byte;
  }
  else if (packet_byte_index > 0 && packet_byte_index < 3) {
    mouse_bytes[packet_byte_index++] = curr_mouse_byte;
  }

  if (packet_byte_index == 3) {
    process_mouse_bytes();
    update_mouse_position();
    process_mouse_clicks();
    packet_byte_index = 0;
  }
}

void process_mouse_bytes() {
  // Existing logic to process mouse bytes
  for (int i = 0; i < 3; i++) {
    mouse_packet.bytes[i] = mouse_bytes[i];
  }
  mouse_packet.lb = (mouse_bytes[0] & MOUSE_LB);
  mouse_packet.rb = (mouse_bytes[0] & MOUSE_RB);
  mouse_packet.mb = (mouse_bytes[0] & MOUSE_MB);
  mouse_packet.x_ov = (mouse_bytes[0] & MOUSE_X_OVF);
  mouse_packet.y_ov = (mouse_bytes[0] & MOUSE_Y_OVF);

  int16_t delta_x = mouse_bytes[1];
  if (mouse_bytes[0] & MOUSE_X_SIGN) {
    delta_x |= 0xFF00; // Sign extend if the sign bit is set
  }
  mouse_packet.delta_x = delta_x;

  int16_t delta_y = mouse_bytes[2];
  if (mouse_bytes[0] & MOUSE_Y_SIGN) {
    delta_y |= 0xFF00; // Sign extend if the sign bit is set
  }
  mouse_packet.delta_y = delta_y;
}

int(mouse_write_cmd)(uint8_t cmd) {
  uint8_t tries = MAX_MOUSE_TRIES;
  uint8_t response;

  while (tries--) {
    if (write_controller(KBC_WRITING, WRITE_BYTE_TO_MOUSE) != 0) {
      return 1;
    }
    if (write_controller(WRITE_CMD, cmd) != 0) {
      return 1;
    }
    if (util_sys_inb(OUT_BUF, &response) != 0) {
      return 1;
    }
    if (response == ACK) {
      return 0; // Command acknowledged
    }
    else if (response == ERROR)
      return 1;

    tickdelay(micros_to_ticks(20000)); // Delay before retrying
  }
  return 1; // Command not acknowledged after retries
}

int init_mouse_pointer() {
  // Load the mouse pointer XPM image
  mouse_pixmap = xpm_load((xpm_map_t) mouse_pointer_xpm, XPM_INDEXED, &mouse_img);
  if (mouse_pixmap == NULL) {
    return 1;
  }
  if (mouse_img.width > h_res || mouse_img.height > v_res) {
    return 1;
  }
  return 0;
}

void draw_mouse_pointer() {
  // Draw the mouse pointer at the current position
  for (int row = 0; row < mouse_img.height; row++) {
    for (int col = 0; col < mouse_img.width; col++) {
      uint32_t color = mouse_img.bytes[row * mouse_img.width + col];
      if (color != 0x0000) { // Skip transparent pixels
        vg_draw_pixel(mouse_x + col, mouse_y + row, color);
      }
    }
  }
}

void save_background(int x, int y) {
  for (int row = 0; row < mouse_img.height; row++) {
    for (int col = 0; col < mouse_img.width; col++) {
      saved_background[row][col] = vg_get_pixel(x + col, y + row); // Save the pixel color
    }
  }
}

void restore_background(int x, int y) {
  for (int row = 0; row < mouse_img.height; row++) {
    for (int col = 0; col < mouse_img.width; col++) {
      vg_draw_pixel(x + col, y + row, saved_background[row][col]); // Restore the pixel color
    }
  }
}

void process_mouse_clicks() {
  extern StateOptions state;
  extern int menu_selected_option;
  extern int categories_selected_option;

  if (mouse_packet.lb) {
    printf("Left button clicked at (%d, %d)\n", mouse_x, mouse_y);

    if (state == MENU || state == INSTRUCTIONS) {
      handle_menu_click(mouse_x, mouse_y, &menu_selected_option);
    }
    else if (state == CATEGORIES) {
      handle_categories_click(mouse_x, mouse_y, &categories_selected_option);
    }
    // You can add more states if needed (e.g., INSTRUCTIONS)
  }
}

void draw_mouse_pointer_to_back_buffer() {
  // Draw the mouse pointer to the back buffer
  for (int row = 0; row < mouse_img.height; row++) {
    for (int col = 0; col < mouse_img.width; col++) {
      uint32_t color = mouse_img.bytes[row * mouse_img.width + col];
      if (color != 0x0000) {                                // Skip transparent pixels
        vg_draw_pixel(mouse_x + col, mouse_y + row, color); // Use back buffer
      }
    }
  }
}

void update_mouse_position() {
  int speed = 2;

  // Update horizontal position
  int dx = mouse_packet.delta_x * speed;
  if (mouse_x + dx < 0 && !mouse_packet.x_ov) {
    mouse_x = 0;
  }
  else if (mouse_x + dx > SCREEN_WIDTH - mouse_img.width && !mouse_packet.x_ov) {
    mouse_x = SCREEN_WIDTH - mouse_img.width;
  }
  else if (!mouse_packet.x_ov) {
    mouse_x += dx;
  }

  // Update vertical position
  int dy = mouse_packet.delta_y * speed;
  if (mouse_y - dy < 0 && !mouse_packet.y_ov) {
    mouse_y = 0;
  }
  else if (mouse_y - dy > SCREEN_HEIGHT - mouse_img.height && !mouse_packet.y_ov) {
    mouse_y = SCREEN_HEIGHT - mouse_img.height;
  }
  else if (!mouse_packet.y_ov) {
    mouse_y -= dy; // Invertido para movimento correto
  }
}
