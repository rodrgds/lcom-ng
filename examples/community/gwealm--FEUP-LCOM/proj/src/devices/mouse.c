#include "devices/mouse.h"
#include "events/events.h"
#include "devices/kbc.h"

static int irq_set;
static int hook_id;

static bool is_in_sync;

static uint8_t bytes[MOUSE_PACKET_SIZE];
static size_t byte_no = 0;

static uint16_t width, height;
static int16_t x, y;

int (mouse_init)(uint16_t screen_width, uint16_t screen_height) {

  #ifndef __LCOM_OPTIMIZED__
  
  if (mouse_enable_data_reporting() != 0) {
    printf("mouse_init: could not enable data reporting\n");
  }
  
  #else
  
  int err_no;
  if((err_no = mouse_enable_stream_mode()) != OKAY) {
    printf("mouse_init: could not enable stream mode\n");
    return err_no;
  }

  if ((err_no = mouse_enable_data_reporting_custom()) != OKAY) {
    printf("mouse_init: could not enable data reporting\n");
    return err_no;
  }

  #endif

  if (mouse_subscribe_interrupts() != 0) {
    printf("mouse_init: could not subscribe to interrupts\n");
    return MOUSE_EINT;
  }

  width = screen_width;
  height = screen_height;
  x = width / 2;
  y = height / 2;

  event_on(INTERRUPT, mouse_handle_interrupt);
  return OKAY;
}

int (mouse_cleanup)() {
  if (mouse_unsubscribe_interrupts() != 0) {
    printf("mouse_cleanup: could not unsubscribe from interrupts\n");
    return MOUSE_EINT;
  }

  int err_no;
  if ((err_no = mouse_disable_data_reporting()) != OKAY) {
    printf("mouse_cleanup: could not disable data reporting\n");
    return err_no;
  }

  event_unregister(INTERRUPT, mouse_handle_interrupt);
  return OKAY;
}

int (mouse_subscribe_interrupts)() {
  hook_id = MOUSE_IRQ; irq_set = BIT(hook_id);
  return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int (mouse_unsubscribe_interrupts)() {
  return sys_irqrmpolicy(&hook_id);
}

void (mouse_parse_packet)(struct mouse_packet *packet, uint8_t *bytes) {
  uint8_t first_byte = bytes[0];

  packet->lb = first_byte & MOUSE_BTN_LEFT;
  
  bool x_sign = first_byte & MOUSE_X_SIGN;
  uint8_t x_delta = bytes[1];
  packet->delta_x = (-x_sign << 8) | x_delta;

  bool y_sign = first_byte & MOUSE_Y_SIGN;
  uint8_t y_delta = bytes[2];
  packet->delta_y = (-y_sign << 8) | y_delta;
}

void (mouse_handle_interrupt)(struct event *ev) {
  if (*((int*) ev->data) & irq_set) {
    uint8_t byte;
    if (kbc_read_byte(KBC_REG_OUT_BUF, &byte) != OKAY) {
      printf("mouse_handle_interrupt: error reading byte from output buffer\n");
      return;
    }

    struct kbc_status stat;
    if (kbc_read_status(&stat) != OKAY) {
      printf("mouse_handle_interrupt: error reading status register\n");
      return;
    }

    if (stat.parity_error || stat.timeout_error) {
      printf("mouse_handle_interrupt: parity or timeout error\n");
      return;
    }

    if (!is_in_sync) {
      if (!(byte & MOUSE_SYNC_MASK)) {
        return;
      }

      is_in_sync = true;
    }

    size_t currIndex = byte_no % MOUSE_PACKET_SIZE;
    byte_no = (byte_no + 1) % MOUSE_PACKET_SIZE;

    bytes[currIndex] = byte;

    if (byte_no == 0) {
      struct mouse_packet packet;
      mouse_parse_packet(&packet, bytes);
      mouse_handle_packet(&packet);

      memset(&bytes, 0, sizeof(bytes) * sizeof(*bytes));
    }
  }
}

void (mouse_handle_packet)(struct mouse_packet *packet) {
  if (packet->delta_x != 0) {
    x += packet->delta_x; 

    if (x < 0) {
      x = 0;
    } else if (x >= width) {
      x = width - 1;
    }
  }

  if (packet->delta_y != 0) {
    y -= packet->delta_y;
  
    if (y < 0) {
      y = 0;
    } else if (y >= height) {
      y = height - 1;
    }
  }

  if (packet->lb) {
    uint16_t pos[] = { x, y }; 
    event_dispatch(MOUSE_CLICK, pos, sizeof(pos));
  }
}

int (mouse_enable_data_reporting_custom)() {
  int err_no;

  uint8_t cmd[] = { 0xF4 };
  if ((err_no = kbc_send_command(MOUSE, cmd, sizeof(cmd))) != OKAY) {
    printf("mouse_enable_data_reporting_custom: could not send command\n");
    return err_no;
  }

  return OKAY;
}

int (mouse_disable_data_reporting)() {
  int err_no;

  uint8_t cmd[] = { 0xF5 };
  if ((err_no = kbc_send_command(MOUSE, cmd, sizeof(cmd))) != OKAY) {
    printf("mouse_disable_data_reporting: could not send command\n");
    return err_no;
  }

  return OKAY;
}

int (mouse_enable_stream_mode)() {
  int err_no;

  uint8_t cmd[] = { 0xEA };
  if((err_no = kbc_send_command(MOUSE, cmd, sizeof(cmd))) != OKAY) {
    printf("mouse_enable_stream_mode: could not send command\n");
    return err_no;
  }

  return OKAY;
}

uint16_t (mouse_get_x)() {
  return x;
}

uint16_t (mouse_get_y)() {
  return y;
}

int (mouse_is_in_position)(uint16_t min_x, uint16_t min_y, uint16_t max_x, uint16_t max_y) {
  uint16_t x = mouse_get_x();
  uint16_t y = mouse_get_y();

  if (x >= min_x && x <= max_x && y >= min_y && y <= max_y) {
    return OKAY;
  }

  return MOUSE_EBOUNDS;
}

