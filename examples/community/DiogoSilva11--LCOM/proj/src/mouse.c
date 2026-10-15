#include <lcom/lcf.h>

#include <stdint.h>

#include "mouse.h"
#include "m_i8042.h"

static uint8_t packet_content[3];

static int hook_id;
static int counter = 0;

int (mouse_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;

  hook_id = MOUSE_IRQ;
  *bit_no = hook_id;

  if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &hook_id))
    return 1;

  return 0;
}

int (mouse_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id))
    return 1;
  
  return 0;
}

int (mouse_enable)() {
  if (sys_irqdisable(&hook_id))
    return 1;

  if (mouse_enable_data_reporting())
    return 1;

  if (sys_irqenable(&hook_id))
    return 1;

  return 0;
}

int (mouse_disable)() {
  if (sys_irqdisable(&hook_id))
    return 1;

  if (mouse_disable_data_reporting())
    return 1;

  if (sys_irqenable(&hook_id))
    return 1;

  return 0;
}

int (mouse_state)(uint8_t *st) {
  if (st == NULL)
    return 1;

  if (util_sys_inb(KBC_ST_REG, st))
    return 1;

  if (*st & KBC_ST_ERROR)
    return 1;

  return 0;
}

int (mouse_cmd)(uint8_t info, bool arg) {
  uint8_t stat = 0;
  int tries = 10; // try for a limited amount of time
  while (tries) {
    if (mouse_state(&stat))
      return 1;

    /* loop while 8042 input buffer is not empty */
    if (!(stat & KBC_ST_IBF)) {
      uint8_t reg = arg ? KBC_CMD_ARG : KBC_CMD_REG; // check if an argument needs to be passed
      if(sys_outb(reg, info)) {
        printf("no proper writing\n");
        return 1;
      }

      if (!arg) return 0; // The controller prefix itself has no mouse ACK.
      uint8_t ack_byte;
      if (mouse_ret_value(&ack_byte))
        return 1;

      if (ack_byte == MOUSE_ACK)
          return 0;
      else if (ack_byte == MOUSE_ERROR)
        return 1;

      return 0;
    }

    tickdelay(micros_to_ticks(DELAY_US));
    tries--;
  }

  printf("time-out\n");
  return 1;
}

int (mouse_ret_value)(uint8_t *data) {
  if (data == NULL) {
    printf("null pointer\n");
    return 1;
  }

  uint8_t stat = 0;
  int tries = 10; // try for a limited amount of time 
  while (tries) {
    if (mouse_state(&stat))
      return 1;

    /* loop while 8042 output buffer is empty */
    if ((stat & KBC_ST_OBF) && (stat & KBC_ST_AUX)) { // if OBF is set and AUX is cleared
      if (util_sys_inb(KBC_OUT_BUF, data)) // read return value from output buffer
        return 1;

      if (mouse_state(&stat)) // check error in status byte
        return 1;
      else
        return 0;
    }

    tickdelay(micros_to_ticks(DELAY_US));
    tries--;
  }

  printf("time-out\n");
  return 1;
}

int (mouse_disable_data_reporting)() {
  /* write command byte */
  if (mouse_cmd(MOUSE_WRITE_CMD, false))
    return 1;
  if (mouse_cmd(MOUSE_DIS_DR, true))
    return 1;

  return 0;
}

int (mouse_packet_full)(){
  if(counter == 3){
    counter = 0;
    return 1;
  }
  return 0;
}

void (mouse_ih)() {
  uint8_t st = 0;
  if (mouse_state(&st)) // read the status register
    return;
  
  /* check if we are set to execute the read operation */
  if (!(st & KBC_ST_OBF) || !(st & KBC_ST_AUX)) {
    printf("invalid operation\n");
    return;
  }

  uint8_t byte;
  if (util_sys_inb(KBC_OUT_BUF, &byte))
    return;

  packet_content[counter] = byte;
  counter++;
}

int (mouse_deltax)() {
  if (packet_content[0] & BIT(4))
    return (int16_t)(-BIT(8) + packet_content[1]);  
  
  return (int16_t)packet_content[1];
}

int (mouse_deltay)() {
  if (packet_content[0] & BIT(5))
    return (int16_t)(-BIT(8) + packet_content[2]);

  return (int16_t)packet_content[2];
}

struct packet (mouse_organize_struct)() {
  struct packet p;

  p.bytes[0] = packet_content[0];
  p.bytes[1] = packet_content[1];
  p.bytes[2] = packet_content[2];
  p.delta_x = mouse_deltax();
  p.delta_y = mouse_deltay();
  p.lb = packet_content[0] & BIT(0);
  p.mb = packet_content[0] & BIT(2);
  p.rb = packet_content[0] & BIT(1);
  p.x_ov = packet_content[0] & BIT(6);
  p.y_ov = packet_content[0] & BIT(7);

  return p;
}

int (mouse_overflow)() {
  if (packet_content[0] & (BIT(7) | BIT(6)))
    return 1;

  return 0;
}

struct mouse_ev (mouse_event)() {
  struct packet p = mouse_organize_struct();
  struct mouse_ev *event = mouse_detect_event(&p);
  return *event;
}
