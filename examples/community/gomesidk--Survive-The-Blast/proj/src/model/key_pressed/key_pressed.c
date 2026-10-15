#include "key_pressed.h"

struct key_pressed_imp {
  bool keys[5]; // UP, DOWN, LEFT, RIGHT, ESC
};

KeyPressed *key_pressed_create() {
  KeyPressed *kp = (KeyPressed *) malloc(sizeof(KeyPressed));
  if (kp == NULL) {
    return NULL;
  }
  for (int i = 0; i < 5; i++) kp->keys[i] = false;
  return kp;
}

void key_pressed_destroy(KeyPressed *kp) {
  if (kp != NULL) {
    free(kp);
  }
}

bool is_up_pressed(KeyPressed *kp) {
  if (kp == NULL) {
    return false;
  }
  return kp->keys[0];
}

bool is_down_pressed(KeyPressed *kp) {
  if (kp == NULL) {
    return false;
  }
  return kp->keys[1];
}

bool is_left_pressed(KeyPressed *kp) {
  if (kp == NULL) {
    return false;
  }
  return kp->keys[2];
}

bool is_right_pressed(KeyPressed *kp) {
  if (kp == NULL) {
    return false;
  }
  return kp->keys[3];
}

bool is_esc_pressed(KeyPressed *kp) {
  if (kp == NULL) {
    return false;
  }
  return kp->keys[4];
}

void key_pressed_update(KeyPressed *kp, uint8_t keyboard_code) {
  if (kp == NULL) {
    return;
  }

  switch (keyboard_code) {
    case ESC_MAKE_CODE: kp->keys[4] = true; break;
    case W_MAKE_CODE:   kp->keys[0] = true; break;
    case S_MAKE_CODE:   kp->keys[1] = true; break;
    case A_MAKE_CODE:   kp->keys[2] = true; break;
    case D_MAKE_CODE:   kp->keys[3] = true; break;
    case ESC_BREAK_CODE: kp->keys[4] = false; break;
    case W_BREAK_CODE:   kp->keys[0] = false; break;
    case S_BREAK_CODE:   kp->keys[1] = false; break;
    case A_BREAK_CODE:   kp->keys[2] = false; break;
    case D_BREAK_CODE:   kp->keys[3] = false; break;
    default: break;
  }
}
