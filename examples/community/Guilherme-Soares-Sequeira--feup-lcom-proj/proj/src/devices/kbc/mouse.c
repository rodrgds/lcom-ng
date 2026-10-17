#include <lcom/lcf.h>

#include <lcom/lab4.h>
#include <lcom/utils.h>

#include <stdbool.h>

#include "i8042.h"
#include "mouse.h"

int32_t mouse_global_hook_id; /**< @brief the hook id that is used to unsubscribe interrupts from the mouse */

struct packet mouse_packet; /**< @brief the packet that contains information about the last movement and the state of the various buttons of the mouse */

uint8_t packet_counter = 0; /**< @brief the amount of bytes that have been read so far for the current packet */

bool mouse_ready = false; /**< @brief indicates whether the mouse_packet contains all the necessary information or not */

/* declared in LCF */

struct packet(get_mouse_packet)() {
  return mouse_packet;
}

int(mouse_subscribe_int)(uint8_t *bit_no) {
  int hook_id = MOUSE_HOOK_ID;

  (*bit_no) = (uint8_t) hook_id;

  int res = sys_irqsetpolicy(KBC_MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);

  mouse_global_hook_id = hook_id;

  return res;
}

int(mouse_unsubscribe_int)() {
  return sys_irqrmpolicy((int *) &mouse_global_hook_id);
}

bool(kbc_mouse_data)() {
  uint8_t st_reg = 0;
  util_sys_inb(KBC_OUT_BUF_ST, &st_reg);

  return (st_reg & (KBC_ST_OBF | BIT(5)));
}

bool(lb_is_pressed)() {
  return mouse_packet.lb;
}

void(mouse_ih)() {
  uint8_t byte = 0;

  bool obf_full = kbc_mouse_data();

  util_sys_inb(KBC_OUT_BUF, &byte);
  if (!obf_full)
    return;

  switch (packet_counter) {
    case 0:
      if (!(byte & MOUSE_FBYTE)) {
        mouse_ready = false;
        return;
      }

      mouse_packet.rb = byte & MOUSE_RB;

      mouse_packet.lb = byte & MOUSE_LB;

      mouse_packet.mb = byte & MOUSE_MB;
      mouse_packet.x_ov = byte & MOUSE_XOVFL;
      mouse_packet.y_ov = byte & MOUSE_YOVFL;

      mouse_packet.delta_x = 0;
      mouse_packet.delta_y = 0;

      break;
    case 1: {
      bool sign = mouse_packet.bytes[0] & MOUSE_XSIGN;

      mouse_packet.delta_x = uint8_to_int16(byte, sign);
      break;
    }
    case 2: {
      bool sign = mouse_packet.bytes[0] & MOUSE_YSIGN;

      mouse_packet.delta_y = uint8_to_int16(byte, sign);
      break;
    }
  }
  mouse_packet.bytes[packet_counter] = byte;
  packet_counter = (packet_counter + 1) % (sizeof(mouse_packet.bytes));

  mouse_ready = (packet_counter == 0); // when we catch a 0 at this point, we know we have successfully parsed 3 bytes
}

void(kbc_enable_data_report)() {
  /* wait_for_inbuff_empty();
  sys_outb(KBC_IN_BUF_CMD, 0x60);
  uint8_t cmd = 0;
  cmd |= BIT(1);
  sys_outb(KBC_IN_BUF_CMD, cmd); */
  write_command_to_mouse(MOUSE_ENABLE_DATA_REPORTING);
}

int16_t(uint8_to_int16)(uint8_t uint8_val, bool sign) {
  return ((sign ? 0xFF : 0x00) << 8) | uint8_val;
}

void(reset_kbc)() {
  write_command_to_mouse(MOUSE_DISABLE_DATA_REPORTING);
  return;
}

void(wait_for_inbuff_empty)() {
  uint8_t status_reg;

  int temp = 0;
  do {
    temp++;
    status_reg = 0;
    util_sys_inb(KBC_OUT_BUF_ST, &status_reg);
  } while (status_reg & KBC_ST_IBF);

  return;
}

uint8_t(mouse_command_feedback)() {
  uint8_t byte = 0;

  util_sys_inb(KBC_OUT_BUF, &byte);

  return byte;
}

void(write_command_to_mouse)(uint8_t command) {
  uint8_t feedback = 0;

  uint8_t temp = 0;
  do {
    wait_for_inbuff_empty();
    sys_outb(KBC_IN_BUF_CMD, KBC_WR_BYTE_TO_MOUSE);
    wait_for_inbuff_empty();
    sys_outb(KBC_IN_BUF_ARG, command);
    tickdelay(micros_to_ticks(25000));
    feedback = mouse_command_feedback();
    temp++;
  } while ((feedback != MOUSE_ACK || feedback != MOUSE_ERROR) && temp < 8);

  if (feedback != MOUSE_ACK) {
    fprintf(stderr, "%s failed while trying to execute command %x\n", __func__, command);
  }
}

bool(mouse_packet_ready)() {
  return mouse_ready;
}
