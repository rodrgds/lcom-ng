#include <lcom/lcf.h>

#include "controller/kbc/kbd.h"

// hook id used for subscribing and unsubscribing keyboard interrupts.
static int kbd_hook_id = 5;
// static global variable for communicating with our program
static uint8_t scancode = 0;

uint8_t(get_scancode)(void) {
  return scancode; // getter function to access the scancode
}

void(set_scancode)(uint8_t value) {
  scancode = value; // setter function to modify the scancode
}

int(kbd_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) {
    fprintf(stderr, "kbd_subscribe_int: bit_no pointer cannot be null.");
    return 1;
  }
  // saves the hook_id value so we can use it as a mask with the msg.m_notify.interrupts
  *bit_no = kbd_hook_id;

  /* this is very similar to the last lab, the only difference being that
   * the subscription should specify not only the IRQ_REENABLE policy but also the IRQ_EXCLUSIVE policy
   * this is done to avoid minix interrup handler to "steal" the scancodes */
  if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id) != 0) {
    perror("kbd_subscribe_int: failed to set the kbd interrupt subscription policy.");
    return 1;
  }

  return 0;
}

int(kbd_unsubscribe_int)(void) {
  if (sys_irqrmpolicy(&kbd_hook_id) != 0) { // unsubscribes the notification
    perror("kbd_unsubscribe_int: failed to remove the keyboard interrupt subscription policy.");
    return 1;
  }

  return 0;
}

int(kbd_enable_int)(void) {
  uint8_t cmd;
  // "tells" the KBC we want to read it's command byte
  if (kbc_write_cmd(KBC_IN, KBC_READ_CMD) != 0) {
    fprintf(stderr, "kbd_enable_int: failed to write the KBC_READ_CMD.");
    return 1;
  }
  if (kbc_read_data(&cmd, false) != 0) { // effectivelly reads the command byte
    fprintf(stderr, "kbd_enable_int: failed to read the kbc command.");
    return 1;
  }

  cmd |= KBC_INT; // masks the cmd with the kbd interrupt bit

  // "tells" the KBC we want to change it's command byte
  if (kbc_write_cmd(KBC_IN, KBC_WRITE_CMD) != 0) {
    fprintf(stderr, "kbd_enable_int: failed to write the KBC_WRITE_CMD.");
    return 1;
  }
  if (kbc_write_cmd(KBC_ARG, cmd) != 0) { // effectivelly writes the command byte
    fprintf(stderr, "kbd_enable_int: failed to pass the kbc argument.");
    return 1;
  }

  return 0;
}

void(kbd_ih)(void) {
  kbc_read_data(&scancode, false); // reads the value stored on the output buffer
}
