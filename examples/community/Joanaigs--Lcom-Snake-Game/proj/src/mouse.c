#include "mouse.h"
#include "i8042.h"
#include <lcom/lcf.h>


int hook_idR = 4;
int mouse_number_bytes = 0;
uint8_t command;

int mouse_subscribe(uint8_t *bit_no, int hook) {
  hook_idR = hook;
  *bit_no = BIT(hook_idR);
  return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_idR);
}

struct packet parse_packet() {
  struct packet pp;
  pp.bytes[0] = mouse[0];
  pp.bytes[1] = mouse[1];
  pp.bytes[2] = mouse[2];
  pp.rb = pp.bytes[0] & RIGHT_BUTTON;
  pp.mb = pp.bytes[0] & MIDDLE_BUTTON;
  pp.lb = pp.bytes[0] & LEFT_BUTTON;
  pp.x_ov = pp.bytes[0] & X_OVERFLOW;
  pp.y_ov = pp.bytes[0] & Y_OVERFLOW;

  bool x_neg = pp.bytes[0] & MSB_X_DELTA;
  bool y_neg = pp.bytes[0] & MSB_Y_DELTA;
  int16_t delta_x = pp.bytes[1], delta_y = pp.bytes[2];
  if (x_neg)
    delta_x |= 0xFF00;
  if (y_neg)
    delta_y |= 0xFF00;

  pp.delta_x = delta_x;
  pp.delta_y = delta_y;

  return pp;
}

void mouse_ih() {
  if (mouse_number_bytes >= 3)
    mouse_number_bytes = 0;
  if (util_sys_inb(STATUS_REG, &statusCode))
    return;
  if ((statusCode & PARITY_BIT) || (statusCode & TOB)) {
    printf("error");
    return;
  }
  if (!(statusCode & OBF) || !(statusCode & MOUSE_AUX))
    return;
  if (util_sys_inb(OUT_BUF, &scanC))
    printf("error");
  mouse[mouse_number_bytes] = scanC;
  mouse_number_bytes++;
}

int(mouse_unsubscribe)() {
  return sys_irqrmpolicy(&hook_idR);
}

uint8_t outuputByte;
int mouse_enable_data_report(bool enable) {
  for (int i = 0; i < 5; ++i) {
    for (int num = 0; num < 10; num++) {
      uint8_t byte;
      if (util_sys_inb(STATUS_REG, &byte))
        return 1;
      if (!(byte & IBF)) {
        sys_outb(CMD_KBC, WRITE_MOUSE);
        return 0;
      }
      tickdelay(micros_to_ticks(DELAY));
    }
    if (enable) {
      for (int num = 0; num < 10; num++) {
        uint8_t byte;
        if (util_sys_inb(STATUS_REG, &byte))
          return 1;
        if (!(byte & IBF)) {
          sys_outb(OUT_BUF, ENABLE_DATA_REPORT);
          return 0;
        }
        tickdelay(micros_to_ticks(DELAY));
      }
    }
    else {
      for (int num = 0; num < 10; num++) {
        uint8_t byte;
        if (util_sys_inb(STATUS_REG, &byte))
          return 1;
        if (!(byte & IBF)) {
          sys_outb(OUT_BUF, DISABLE_DATA_REPORT);
          return 0;
        }
        tickdelay(micros_to_ticks(DELAY));
      }
    }
    if (util_sys_inb(OUT_BUF, &outuputByte) != 0) {
      return 1;
    }
    if (outuputByte == ACK) {
      return 0;
    }
    tickdelay(micros_to_ticks(DELAY));
  }
  return 1;
}




struct mouse_ev (mouse_get_event)(struct packet *pp) {
    static struct mouse_ev event;
    static uint8_t last = 0;

    if (pp == NULL)
        return event;

    // current button presses
    uint8_t button_presses = pp->bytes[0] & (LEFT_BUTTON | RIGHT_BUTTON | MIDDLE_BUTTON);
    int16_t delta_x = pp->delta_x;
    int16_t delta_y = pp->delta_y;

    if ((button_presses ^ last) == LEFT_BUTTON && !(last & LEFT_BUTTON)) {
        event.type = LB_PRESSED;
        last |= LEFT_BUTTON;

    } else if ((button_presses ^ last) == RIGHT_BUTTON && !(last & RIGHT_BUTTON)) {
        event.type = RB_PRESSED;
        last |= RIGHT_BUTTON;

    } else if ((button_presses ^ last) == LEFT_BUTTON && (last & LEFT_BUTTON)) {
        event.type = LB_RELEASED;
        last &= ~LEFT_BUTTON;

    } else if ((button_presses ^ last) == RIGHT_BUTTON && (last & RIGHT_BUTTON)) {
        event.type = RB_RELEASED;
        last &= ~RIGHT_BUTTON;

    } else if ((delta_x || delta_y) && (button_presses == last)) {
        event.type = MOUSE_MOV;

    } else
        event.type = BUTTON_EV;

    event.delta_x = delta_x;
    event.delta_y = delta_y;
    return event;
}
