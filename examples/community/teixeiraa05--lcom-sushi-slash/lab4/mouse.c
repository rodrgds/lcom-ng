#include <lcom/lcf.h>
#include "mouse.h"
#include "../lab3/kbc.h"
#include "i8042.h"

// ============ STATIC STATE ============
static bool mouse_error = false;
static int hook_id_mouse;
static uint8_t data;
static uint8_t packet_bytes[PACKET_SIZE];
static int byte_count = 0;
static struct packet p;

// ============ INTERRUPT SUBSCRIPTION ============
int mouse_write_aux_command(uint8_t command) {
  if (kbc_write_command(KBC_CMD_REG, WRITE_TO_AUX)) return 1;
  if (kbc_write_command(KBC_IN_BUF, command)) return 1;
  return 0;
}

int (mouse_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  hook_id_mouse = MOUSE_IRQ; 
  *bit_no = hook_id_mouse;
  if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse)) return 1;

  return 0;
}

int (mouse_unsubscribe_int)(void) {
  if (sys_irqrmpolicy(&hook_id_mouse)) return 1;

  return 0;
}

// ============ COMMAND & ACK HANDLING ============

// Helper: read a single byte from KBC output buffer if mouse data is available
// Returns 0 on success (byte stored in *byte), 1 on failure (not ready or error)
int mouse_read_byte(uint8_t *byte) {
  if (byte == NULL) return 1;

  uint8_t status = 0;
  if (kbc_read_status(&status)) return 1;

  if ((status & OBF) && (status & MOUSE_DATA)) {
    if (util_sys_inb(KBC_OUT_BUF, byte)) return 1;
    if (status & (PARITY_ERR | TIMEOUT_ERR)) {
      byte_count = 0; // A discarded byte invalidates the partial packet.
      return 1;
    }
    return 0;
  }

  return 1;
}

int mouse_read_ack(uint8_t *ack) {
  if (ack == NULL) return 1;

  for (int wait = 0; wait < KBC_MAX_ATTEMPTS; wait++) {
    if (mouse_read_byte(ack) == 0) return 0;
    tickdelay(micros_to_ticks(MOUSE_RESPONSE_DELAY));
  }

  return 1;
}

int (mouse_change_data_report)(uint8_t command) {
  uint8_t response = 0;

  for (int attempt = 0; attempt < KBC_MAX_ATTEMPTS; attempt++) {
    if (mouse_write_aux_command(command)) return 1;

    if (mouse_read_ack(&response)) continue; // timeout/status failure, resend command
    if (response == ACK_BYTE) return 0;
    if (response == NACK_BYTE || response == ERROR_BYTE) continue;
  }

  return 1;
}

int (mouse_enable_data_report)(void) {
  return mouse_change_data_report(ENABLE_DATA_REPORTING);
}

int (mouse_disable_data_report)(void) {
  return mouse_change_data_report(DISABLE_DATA_REPORTING);
}


// ============ PACKET HANDLING ============


int (mouse_packet_is_ready)() {
  if (mouse_error) return 0;
  if (byte_count == 0) {
    if (!(data & ALWAYS_1)) return 0; // actually skip unsynced bytes
    packet_bytes[0] = data;
    byte_count++;
    return 0;
  } else {
    packet_bytes[byte_count] = data;
    byte_count++;
    if (byte_count == PACKET_SIZE) {
      byte_count = 0; // reset for next packet
      return 1; // packet is ready
    }
    return 0; // packet not ready yet
  }
}

struct packet* (handle_packet) () {
  p.bytes[0] = packet_bytes[0];
  p.bytes[1] = packet_bytes[1];
  p.bytes[2] = packet_bytes[2];

  p.lb = (packet_bytes[0] & LB) != 0;
  p.mb = (packet_bytes[0] & MB) != 0;
  p.rb = (packet_bytes[0] & RB) != 0;

  p.delta_x = (packet_bytes[0] & X_SIGN) ? (packet_bytes[1] - 256) : packet_bytes[1]; 
  p.delta_y = (packet_bytes[0] & Y_SIGN) ? (packet_bytes[2] - 256) : packet_bytes[2]; 

  p.x_ov = (packet_bytes[0] & X_OVF) != 0;
  p.y_ov = (packet_bytes[0] & Y_OVF) != 0;

  return &p;
}

// ============ INTERRUPT HANDLER ============

void (mouse_ih)(void) {
  mouse_error = false;

  if (mouse_read_byte(&data) == 0) {
    return; // success, byte is in &data
  }

  mouse_error = true;
}

// ============ GETTERS ============
bool (mouse_has_error)() {
  return mouse_error;
}

uint8_t (mouse_get_data)() {
  return data;
}

// ============ PROJ ============

int mouse_init(MouseGameState *state, int screen_width, int screen_height, uint8_t *bit_no) {
    if (state == NULL || bit_no == NULL) {
        return 1;
    }

    // Initialize state
    memset(state, 0, sizeof(MouseGameState));
    state->x = screen_width / 2; // Start in the middle of the screen
    state->y = screen_height / 2;
    state->prev_x = state->x;
    state->prev_y = state->y;

    mouse_error = false;
    byte_count = 0;

    // Subscribe to mouse interrupts
    if (mouse_subscribe_int(bit_no)) {
        printf("[mouse] Failed to subscribe to mouse interrupts\n");
        return 1;
    }

    // Enable data reporting
    if (mouse_enable_data_report()) {
        printf("[mouse] Failed to enable data reporting\n");
        mouse_unsubscribe_int();
        return 1;
    }

    return 0;
}

int mouse_cleanup(void) {

  int ret_value = 0;
  if (mouse_disable_data_report() != 0) {
      printf("[mouse] Failed to disable data reporting\n");
      ret_value = 1;
    }

  if (mouse_unsubscribe_int() != 0) {
      printf("[mouse] Failed to unsubscribe from mouse interrupts\n");
      ret_value = 1;
    }

  return ret_value;

}

// ============ HIGH-LEVEL PACKET PROCESSING & GAME STATE UPDATE ============

bool mouse_proj_ih(MouseGameState *state, int screen_width, int screen_height) {
    if (state == NULL) {
        return false;
    }

    // Read one byte; only feed successful reads to the packet assembler.
    mouse_ih();
    if (mouse_has_error()) return false;

    // Check if a full packet was parsed
    if (mouse_packet_is_ready()) {
        struct packet *pp = handle_packet();

        mouse_update_state(state, pp, screen_width, screen_height);

        return true; // Packet processed and state updated
    }

    return false; // No complete packet yet
}

void mouse_update_state(MouseGameState *state, struct packet *pp, int screen_width, int screen_height) {
    state->prev_x = state->x;
    state->prev_y = state->y;

    // Update button states
    state->lb_pressed = pp->lb;
    state->mb_pressed = pp->mb;
    state->rb_pressed = pp->rb;

    // Update relative movement deltas
    state->dx = pp->delta_x;
    state->dy = -pp->delta_y; // Invert Y-axis

    // Update absolute position with clamping
    state->x += state->dx;
    state->y += state->dy;

    if (state->x < 0) state->x = 0;
    /* Original cursor_point.xpm is 12 by 12, drawn top-left. */
    int max_x = screen_width > 12 ? screen_width - 12 : 0;
    int max_y = screen_height > 12 ? screen_height - 12 : 0;
    if (state->x > max_x) state->x = max_x;
    if (state->y < 0) state->y = 0;
    if (state->y > max_y) state->y = max_y;

    // Detect slash gesture
    int velocity_squared = state->dx * state->dx + state->dy * state->dy;
    const int SLASH_THRESHOLD_SQUARED = SLASH_THRESHOLD * SLASH_THRESHOLD;
    state->is_slashing = (velocity_squared > SLASH_THRESHOLD_SQUARED) && state->lb_pressed;

}

// ============ GETTERS ============

bool mouse_get_movement(MouseGameState *state, int *dx, int *dy) {
    if (dx == NULL || dy == NULL) {
        return false;
    }
    *dx = state->dx;
    *dy = state->dy;
    return true;
}

// Maybe uselless, but could be helpful for some game logic
uint8_t mouse_get_buttons(MouseGameState *state) {
    uint8_t buttons = 0;
    if (state->lb_pressed) buttons |= 0x01; // Left button bit
    if (state->mb_pressed) buttons |= 0x02; // Middle button bit
    if (state->rb_pressed) buttons |= 0x04; // Right button bit
    return buttons;
}

void mouse_get_position(MouseGameState *state, int *x, int *y) {
    if (x != NULL) {
        *x = state->x;
    }
    if (y != NULL) {
        *y = state->y;
    }
}
