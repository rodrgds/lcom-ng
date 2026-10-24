#include <lcom/lcf.h>
#include <stdint.h>
#include <stdbool.h>

#include "mouse.h"
#include "../utils/utils.h"

#define MOUSE_IRQ 12

#define KBC_STATUS_REG 0x64
#define KBC_OUT_BUF 0x60
#define KBC_CMD_REG 0x64
#define KBC_WRITE_MOUSE 0xD4

#define KBC_OBF BIT(0)
#define KBC_IBF BIT(1)
#define KBC_AUX BIT(5)
#define KBC_PARITY_ERROR BIT(7)
#define KBC_TIMEOUT_ERROR BIT(6)

#define MOUSE_ACK 0xFA
#define MOUSE_ENABLE_DATA_REPORTING 0xF4
#define MOUSE_DISABLE_DATA_REPORTING 0xF5

#define MOUSE_LB BIT(0)
#define MOUSE_X_SIGN BIT(4)
#define MOUSE_Y_SIGN BIT(5)
#define MOUSE_X_OVF BIT(6)
#define MOUSE_Y_OVF BIT(7)
#define MOUSE_FIRST_BYTE BIT(3)

#define MAX_TRIES 5
#define DELAY_US 20000

static int mouse_hook_id = MOUSE_IRQ;

static uint8_t packet[3];
static uint8_t packet_index = 0;
static bool ready = false;

static int kbc_wait_input_empty(void) {
  uint8_t status;

  for (int tries = 0; tries < MAX_TRIES; tries++) {
    if (util_sys_inb(KBC_STATUS_REG, &status) != 0) {
      return 1;
    }

    if (!(status & KBC_IBF)) {
      return 0;
    }

    tickdelay(micros_to_ticks(DELAY_US));
  }

  return 1;
}

static int mouse_read_ack(uint8_t *response) {
  uint8_t status;

  if (response == NULL) {
    return 1;
  }

  for (int tries = 0; tries < MAX_TRIES; tries++) {
    if (util_sys_inb(KBC_STATUS_REG, &status) != 0) {
      return 1;
    }

    if ((status & KBC_OBF) && (status & KBC_AUX)) {
      if (status & (KBC_PARITY_ERROR | KBC_TIMEOUT_ERROR)) {
        return 1;
      }

      if (util_sys_inb(KBC_OUT_BUF, response) != 0) {
        return 1;
      }

      return 0;
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }

  return 1;
}

static int mouse_write_command(uint8_t command) {
  uint8_t response;

  for (int tries = 0; tries < MAX_TRIES; tries++) {
    if (kbc_wait_input_empty() != 0) {
      return 1;
    }

    if (sys_outb(KBC_CMD_REG, KBC_WRITE_MOUSE) != OK) {
      return 1;
    }

    if (kbc_wait_input_empty() != 0) {
      return 1;
    }

    if (sys_outb(KBC_OUT_BUF, command) != OK) {
      return 1;
    }

    if (mouse_read_ack(&response) != 0) {
      continue;
    }

    if (response == MOUSE_ACK) {
      return 0;
    }
  }

  return 1;
}

int project_mouse_subscribe(uint32_t *irq_set) {
  if (irq_set == NULL) {
    return 1;
  }

  mouse_hook_id = MOUSE_IRQ;
  *irq_set = BIT(mouse_hook_id);

  if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) != OK) {
    return 1;
  }

  return 0;
}

int project_mouse_unsubscribe(void) {
  if (sys_irqrmpolicy(&mouse_hook_id) != OK) {
    return 1;
  }

  return 0;
}

int project_mouse_enable_data_reporting(void) {
  int ret;
  sys_irqdisable(&mouse_hook_id);
  ret = mouse_write_command(MOUSE_ENABLE_DATA_REPORTING);
  sys_irqenable(&mouse_hook_id);
  return ret;}

int project_mouse_disable_data_reporting(void) {
  int ret;
  sys_irqdisable(&mouse_hook_id);
  ret = mouse_write_command(MOUSE_DISABLE_DATA_REPORTING);
  sys_irqenable(&mouse_hook_id);
  return ret;
}

int project_mouse_set_sample_rate(uint8_t rate) {
  int ret;
  sys_irqdisable(&mouse_hook_id);
  ret = mouse_write_command(0xF3); // Set Sample Rate command
  if (ret == 0) ret = mouse_write_command(rate);
  sys_irqenable(&mouse_hook_id);
  return ret;
}

int project_mouse_set_resolution(uint8_t res) {
  int ret;
  sys_irqdisable(&mouse_hook_id);
  ret = mouse_write_command(0xE8); // Set Resolution command
  if (ret == 0) ret = mouse_write_command(res);
  sys_irqenable(&mouse_hook_id);
  return ret;
}

void project_mouse_ih(void) {
  uint8_t status;
  uint8_t data;

  ready = false;

  if (util_sys_inb(KBC_STATUS_REG, &status) != 0) return;
  if (!(status & KBC_OBF)) return;
  if (!(status & KBC_AUX)) return;

  util_sys_inb(KBC_OUT_BUF, &data);

  if (status & (KBC_PARITY_ERROR | KBC_TIMEOUT_ERROR)) return;

  if (packet_index == 0 && !(data & MOUSE_FIRST_BYTE)) return;

  packet[packet_index] = data;
  packet_index++;

  if (packet_index == 3) {
    packet_index = 0;
    ready = true;
  }
}

bool project_mouse_packet_ready(void) {
  return ready;
}

uint8_t* project_mouse_get_packet(void) {
  return packet;
}
