#include <lcom/lcf.h>
#include <stdbool.h>
#include "mouse.h"

static int hook_id_mouse = 2;
static uint8_t packet[3];
static uint8_t packet_idx = 0;

MouseEvent mouse_event = { .dx = 0, .dy = 0, .lb = false, .mb = false, .rb = false };
bool packet_ready = false;

/** Drena o output buffer do KBC */
static void mouse_flush_output(void) {
  uint8_t stat, junk;
  while (util_sys_inb(KBC_STATUS_REG, &stat) == OK &&
         (stat & FULL_OUT_BUFFER)) {
    util_sys_inb(KBC_OUT_CMD, &junk);
  }
}

/** Envia um comando ao mouse e aguardar ACK (0xFA) */
static int mouse_write_cmd(uint8_t cmd) {
  uint8_t ack;
  
  for (int tries = 0; tries < 3; tries++) {
    if (write_KBC_command(KBC_IN_CMD, MOUSE_CMD_WRITE)) return 1;
    if (write_KBC_command(KBC_OUT_CMD, cmd)) return 1;
    if (read_KBC_output(KBC_OUT_CMD, &ack, 1)) return 1;

    if (ack == 0xFA) return 0;    /* OK */
    if (ack == 0xFE) continue;    /* tentar denovo / retry */
  }
  return 1;
}

int (mouse_subscribe_int)(uint8_t *bit_no) {
  if (!bit_no) return 1;
  hook_id_mouse = 2;  // Reset hook_id a cada subscrição
  *bit_no = BIT(hook_id_mouse);
  return sys_irqsetpolicy(MOUSE_IRQ,
                          IRQ_REENABLE | IRQ_EXCLUSIVE,
                          &hook_id_mouse);
}

int (mouse_unsubscribe_int)(void) {
  return sys_irqrmpolicy(&hook_id_mouse);
}

int (mouse_enable_data_report)(void) {
  mouse_flush_output();
  return mouse_write_cmd(MOUSE_ENABLE_DATA_REPORTING);
}

int (mouse_disable_data_report)(void) {
  return mouse_write_cmd(MOUSE_DISABLE_DATA_REPORTING);
}

void (mouse_ih)(void) {
  uint8_t stat, data;

  if (read_KBC_status(&stat)) return;
  if (!(stat & FULL_OUT_BUFFER)) return;

  if (read_KBC_output(KBC_OUT_CMD, &data, 1)) return;
  if (stat & (PARITY_ERROR | TIMEOUT_ERROR)) return;

  if (packet_idx == 0 && !(data & BIT(3))) return;
  packet[packet_idx++] = data;

  if (packet_idx < 3) return;
  packet_idx = 0;

  if (packet[0] & (MOUSE_X_OVERFLOW | MOUSE_Y_OVERFLOW)) return;

  mouse_event.lb = packet[0] & MOUSE_LEFT_BUTTON;
  mouse_event.rb = packet[0] & MOUSE_RIGHT_BUTTON;
  mouse_event.mb = packet[0] & MOUSE_MIDDLE_BUTTON;
  
  mouse_event.dx = (packet[0] & MOUSE_MSB_X_DELTA) ? (int8_t)packet[1] : packet[1];
  mouse_event.dy = (packet[0] & MOUSE_MSB_Y_DELTA) ? (int8_t)packet[2] : packet[2];

  packet_ready = true;
}
