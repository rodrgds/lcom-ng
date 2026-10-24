#include <lcom/lcf.h>

#include "input.h"
#include "../config.h"

#define ESC_MAKE       0x01
#define ESC_BREAK      0x81

#define W_MAKE         0x11
#define W_BREAK        0x91

#define S_MAKE         0x1F
#define S_BREAK        0x9F

#define A_MAKE         0x1E
#define A_BREAK        0x9E

#define D_MAKE         0x20
#define D_BREAK        0xA0

#define SPACE_MAKE     0x39
#define SPACE_BREAK    0xB9

#define P_MAKE         0x19
#define P_BREAK        0x99

#define TWO_BYTE_CODE  0xE0

#define UP_MAKE        0x48
#define UP_BREAK       0xC8
#define DOWN_MAKE      0x50
#define DOWN_BREAK     0xD0
#define LEFT_MAKE      0x4B
#define LEFT_BREAK     0xCB
#define RIGHT_MAKE     0x4D
#define RIGHT_BREAK    0xCD

#define MOUSE_LB       BIT(0)
#define MOUSE_X_SIGN   BIT(4)
#define MOUSE_Y_SIGN   BIT(5)
#define MOUSE_X_OVF    BIT(6)
#define MOUSE_Y_OVF    BIT(7)

static bool extended_scancode = false;

void input_init(InputState *input) {
  if (input == NULL) {
    return;
  }

  input->up = false;
  input->down = false;
  input->left = false;
  input->right = false;

  input->shoot = false;
  input->shoot_clicked = false;

  input->pause = false;
  input->pause_clicked = false;

  input->escape = false;
  input->escape_clicked = false;

  input->mouse_x = SCREEN_WIDTH / 2;
  input->mouse_y = SCREEN_HEIGHT / 2;
  input->mouse_left_pressed = false;
  input->mouse_left_clicked = false;

  extended_scancode = false;
}

void input_handle_keyboard(InputState *input, uint8_t scancode) {
  if (input == NULL) {
    return;
  }

  if (scancode == TWO_BYTE_CODE) {
    extended_scancode = true;
    return;
  }

  if (extended_scancode) {
    switch (scancode) {
      case UP_MAKE:
        input->up = true;
        break;

      case UP_BREAK:
        input->up = false;
        break;

      case DOWN_MAKE:
        input->down = true;
        break;

      case DOWN_BREAK:
        input->down = false;
        break;

      case LEFT_MAKE:
        input->left = true;
        break;

      case LEFT_BREAK:
        input->left = false;
        break;

      case RIGHT_MAKE:
        input->right = true;
        break;

      case RIGHT_BREAK:
        input->right = false;
        break;

      default:
        break;
    }

    extended_scancode = false;
    return;
  }

  switch (scancode) {
    case W_MAKE:
      input->up = true;
      break;

    case W_BREAK:
      input->up = false;
      break;

    case S_MAKE:
      input->down = true;
      break;

    case S_BREAK:
      input->down = false;
      break;

    case A_MAKE:
      input->left = true;
      break;

    case A_BREAK:
      input->left = false;
      break;

    case D_MAKE:
      input->right = true;
      break;

    case D_BREAK:
      input->right = false;
      break;

    case SPACE_MAKE:
      if (!input->shoot) {
        input->shoot_clicked = true;
      }
      input->shoot = true;
      break;

    case SPACE_BREAK:
      input->shoot = false;
      break;

    case P_MAKE:
      if (!input->pause) {
        input->pause_clicked = true;
      }
      input->pause = true;
      break;

    case P_BREAK:
      input->pause = false;
      break;

    case ESC_MAKE:
      if (!input->escape) {
        input->escape_clicked = true;
      }
      input->escape = true;
      break;

    case ESC_BREAK:
      input->escape = false;
      break;

    default:
      break;
  }
}

void input_handle_mouse(InputState *input, uint8_t *packet) {
  if (input == NULL || packet == NULL) {
    return;
  }

  int16_t dx = packet[1];
  int16_t dy = packet[2];

  if (packet[0] & MOUSE_X_SIGN) {
    dx |= 0xFF00;
  }

  if (packet[0] & MOUSE_Y_SIGN) {
    dy |= 0xFF00;
  }

  /* 
   * Apply deltas. We do not discard movement on overflow, as that causes
   * fast mouse flicks to be completely ignored.
   * Speed is balanced with a 1.5x multiplier.
   */
  input->mouse_x += dx + (dx / 2);
  input->mouse_y -= dy + (dy / 2);

  if (input->mouse_x < 0) {
    input->mouse_x = 0;
  }

  if (input->mouse_x >= SCREEN_WIDTH) {
    input->mouse_x = SCREEN_WIDTH - 1;
  }

  if (input->mouse_y < 0) {
    input->mouse_y = 0;
  }

  if (input->mouse_y >= SCREEN_HEIGHT) {
    input->mouse_y = SCREEN_HEIGHT - 1;
  }

  bool was_pressed = input->mouse_left_pressed;
  input->mouse_left_pressed = packet[0] & MOUSE_LB;

  if (!was_pressed && input->mouse_left_pressed) {
    input->mouse_left_clicked = true;
  }
}

void input_end_frame(InputState *input) {
  if (input == NULL) {
    return;
  }

  input->shoot_clicked = false;
  input->pause_clicked = false;
  input->mouse_left_clicked = false;
}
