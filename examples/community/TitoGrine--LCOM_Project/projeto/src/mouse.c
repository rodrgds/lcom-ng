#include <lcom/lcf.h>

#include "keyboard.h"
#include "mouse.h"
#include "MACROS.h"

int hook_id_mouse = 3;
uint8_t data;
bool error_flag_mouse;

static bool right_button = false;
static bool left_button = false;
static bool middle_button = false;

bool x_sign = false;
bool y_sign = false;

// Stores mouse event information
struct packet packet_data;
uint8_t packet_size = 0;

int(mouse_subscribe_int)(uint8_t *bit_no) {

  *bit_no = hook_id_mouse;
  sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse);

  return 0;
}

int(mouse_unsubscribe_int)() {

  sys_irqrmpolicy(&hook_id_mouse);

  return 0;
}

void(mouse_ih)() {

  uint32_t status_register;

  sys_inb(STAT_REG, &status_register);

  if ((status_register & ST_PARITY) || (status_register & ST_TIMEOUT))
    error_flag_mouse = true;
  else
    error_flag_mouse = false;


  if (status_register & ST_OBF) {
    uint32_t output;

    sys_inb(OUT_BUF, &output);

    data = (uint8_t) output;
  }
  
  if (!error_flag_mouse) {

    if (packet_size == 0 && (data & BIT(3))) {

      packet_data.bytes[packet_size] = data;

      packet_data.y_ov = data & Y_OVF;
      packet_data.x_ov = data & X_OVF;

      if (data & Y_SIGNAL)
        y_sign = true;
      else
        y_sign = false;
      if (data & X_SIGNAL)
        x_sign = true;
      else
        x_sign = false;

      packet_data.mb = data & MMB;
      middle_button = packet_data.mb;
      packet_data.rb = data & RMB;
      right_button = packet_data.rb;
      packet_data.lb = data & LMB;
      left_button = packet_data.lb;

      packet_size++;
    }
    else if (packet_size == 1) {
      packet_data.bytes[packet_size] = data;

      if (x_sign)
        packet_data.delta_x = 0xFF00 | data;
      else
        packet_data.delta_x = data;
      packet_size++;
    }
    else if (packet_size == 2) {
      packet_data.bytes[packet_size] = data;
      if (y_sign)
        packet_data.delta_y = 0xFF00 | data;
      else
        packet_data.delta_y = data;

      packet_size = 0;
    }
  }
}

int(mouse_cmd)(uint8_t command) {

  uint32_t status;

  do {

    kbc_cmd(WRITE_BYTE_MOUSE, &command, false);

    tickdelay(micros_to_ticks(DELAY));

    while (1) {
      sys_inb(STAT_REG, &status);

      if (status & ST_OBF) {
        sys_inb(OUT_BUF, &status);
        break;
      }
      tickdelay(micros_to_ticks(DELAY));
    }

  } while (status != ACK);

  return 0;
}


