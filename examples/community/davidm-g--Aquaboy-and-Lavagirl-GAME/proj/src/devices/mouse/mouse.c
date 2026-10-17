#include "mouse.h"

struct packet packet;
int mouse_hook_id = 2;
uint8_t kbd_outbuf;
int byte_counter = 0;
int(mouse_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;
  *bit_no = BIT(mouse_hook_id);
  return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

int(mouse_unsubscribe_int)() {
  return sys_irqrmpolicy(&mouse_hook_id);
}

int write_to_kbc_mouse(uint8_t port, uint8_t cmdbyte) {
  uint8_t st;
  int attempts = 10;
  while (attempts > 0) {
    if (util_sys_inb(ST_REGISTER, &st) != 0) {
      return 1;
    } /* assuming it returns OK */ /* loop while 8042 input buffer is not empty */
    if ((st & IBF) == 0) {         // if input buffer is not full
      if (sys_outb(port, cmdbyte) != 0) {
        return 1;
      } /* no args command */
      return 0;
    }
    tickdelay(micros_to_ticks(20000));
    attempts--;
  }
  return 1; // if attempts end, throw error
}
int read_value_data_from_kbc_mouse(uint8_t port, uint8_t *output) {
  int attempts = 10;
  while (attempts > 0) {
    if (can_read_outbuf_mouse() == 0) {
      if (util_sys_inb(OUT_BUF, output) == 0) { // read data from output buffer
        return 0;
      }
    }
    tickdelay(micros_to_ticks(20000));
    attempts--;
  }
  return 1; // if attempts end, throw error
}
int can_read_outbuf_mouse() {
  uint8_t st = 0;
  // read status
  if (util_sys_inb(ST_REGISTER, &st) != 0) {
    return 1;
  }
  // empty out_buf
  if ((st & OBF) == 0) {
    return 1;
  }
  // timeout or parity error
  if ((st & (TIM_ERR | PAR_ERR)) != 0) {
    util_sys_inb(OUT_BUF, &kbd_outbuf);
    return 1;
  }
  // mouse data not in buffer
  if ((st & AUX) == 0) {
    util_sys_inb(OUT_BUF, &kbd_outbuf);
    return 1;
  }

  return 0;
}
int send_cmd_mouse(uint8_t cmd) { // function to send commands to mouse
  uint8_t response = 0X00;
  int attempts = 10;
  while (attempts > 0) {
    if (write_to_kbc_mouse(KBC_CMD_REG, REQUEST_MOUSE))
      return 1; // write 0xD4 to port 0x64
    if (write_to_kbc_mouse(WRITE_CMD_BYTE, cmd))
      return 1; //	write the code for the command to port 0x60
    if (read_value_data_from_kbc_mouse(OUT_BUF, &response))
      return 1; // read the acknowledgement byte
    if (response == ACK)
      return 0; // when ack, ggwp
    attempts--;
  }
  return 1;
}

int change_sample_rate(uint8_t rate) {
  uint8_t response = 0X00;
  int attempts = 10;
  while (attempts > 0) {
    if (write_to_kbc_mouse(KBC_CMD_REG, REQUEST_MOUSE))
      return 1; // write 0xD4 to port 0x64
    if (write_to_kbc_mouse(WRITE_CMD_BYTE, 0xF3))
      return 1; //	write the code for the command to port 0x60
    if (read_value_data_from_kbc_mouse(OUT_BUF, &response))
      return 1; // read the acknowledgement byte
    if (response == ACK) {
      if (write_to_kbc_mouse(KBC_CMD_REG, REQUEST_MOUSE))
        return 1;
      if (write_to_kbc_mouse(WRITE_CMD_BYTE, rate))
        return 1;
      if (read_value_data_from_kbc_mouse(OUT_BUF, &response))
        return 1;
      if (response == ACK) // when ack, ggwp
        return 0;
    }
    attempts--;
  }
  return 1;
}
void(mouse_ih)() {
  int attempts = 10;
  while (attempts > 0) {
    if (can_read_outbuf_mouse() == 0) {
      if (util_sys_inb(OUT_BUF, &kbd_outbuf) == 0) {
        return;
      }
    }
    attempts--;
  }
}

void bytes_to_packet() {
  if (byte_counter == 0 && (kbd_outbuf & FIRST_BYTE)) { // if its the first byte the BIT(3) must be 1, but we need
    packet.bytes[byte_counter] = kbd_outbuf;            // byte_counter==0 to be sure of it
    byte_counter++;
  }
  else if (byte_counter == 1 || byte_counter == 2) {
    packet.bytes[byte_counter] = kbd_outbuf;
    byte_counter++;
  }
}
void packet_parse() { // see documentation of struct packet

  if (packet.bytes[0] & X_OV)
    return;
  if (packet.bytes[0] & Y_OV)
    return;
  packet.lb = (packet.bytes[0] & LB); // simple masks, see mouse data packet byte 1
  packet.rb = (packet.bytes[0] & RB);
  packet.mb = (packet.bytes[0] & MB);
  if (packet.bytes[0] & X_MSB) {                 // if x_delta MSB is 1
    packet.delta_x = (0xFF00 | packet.bytes[1]); // we need to span the msb through the rest of the delta x bits, since it has 16 bits
  }
  else {
    packet.delta_x = packet.bytes[1]; // by default, zeroes are used to fill the rest of the bits, so we dont need to do anything
  }
  if (packet.bytes[0] & Y_MSB) {                 // if y_delta MSB is 1
    packet.delta_y = (0xFF00 | packet.bytes[2]); // we need to span the msb through the rest of the delta y bits, since it has 16 bits.
  }
  else {
    packet.delta_y = packet.bytes[2]; // by default, zeroes are used to fill the rest of the bits, so we dont need to do anything
  }
  packet.y_ov = (packet.bytes[0] & Y_OV);
  packet.x_ov = (packet.bytes[0] & X_OV);
}
