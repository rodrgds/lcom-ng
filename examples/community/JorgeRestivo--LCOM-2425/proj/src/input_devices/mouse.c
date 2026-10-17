#include "mouse.h"
#include "i8042.h"
#include <lcom/lcf.h>
#include <minix/syslib.h> // For Minix system library functions
#include <minix/sysutil.h>


const uint8_t crosshair_margin = 10;
uint8_t byte_index = 0;
static int hook_id_mouse = 2; // IRQ 12
#define MOUSE_PACKET_SIZE 3

uint8_t mouse_packet[MOUSE_PACKET_SIZE];
#define OK 0


static struct packet current_packet;
static bool packet_ready = false;

void fire_bullet(Vec_pos c);



int(mouse_subscribe_int)(uint8_t *bit_no) {
  *bit_no = BIT(hook_id_mouse);
  return sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse);
}

int(mouse_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id_mouse);
}

void(proj_mouse_ih)(void) {

  uint8_t mouse_byte;
  uint8_t status;

  if (util_sys_inb(STAT_REG, &status) != OK)
    return;

  if ((status & (OBF | AUX)) == (OBF | AUX)) {
    if ((status & (PAR_ERR | TO_ERR)) == 0) {

      util_sys_inb(MOUSE_OUT_BUF, &mouse_byte);
      
      if (byte_index == 0 && !(mouse_byte &BIT(3))){
        return;
      }
    
      mouse_packet[byte_index] = mouse_byte;  // Move to next byte (0 → 1 → 2 →   0...)
      byte_index++;

      if (byte_index == 3) {
            parse_mouse_packet(mouse_packet, &current_packet);
            byte_index = 0;

          
            cursor_pos.x += current_packet.delta_x;
            cursor_pos.y -= current_packet.delta_y;

            const uint8_t margin = 10;
            if (cursor_pos.x < crosshair_margin) cursor_pos.x = margin;
            if (cursor_pos.x > SCREEN_WIDTH - margin) cursor_pos.x = SCREEN_WIDTH - margin;
            if (cursor_pos.y < margin) cursor_pos.y = margin;
            if (cursor_pos.y > SCREEN_HEIGHT - margin) cursor_pos.y = SCREEN_HEIGHT - margin;

            if (current_packet.lb) {
                fire_bullet(cursor_pos);
            }
      }
    }
  }
}

int(mouse_write_cmd)(uint8_t cmd) {
  uint8_t stat;
  for (int i = 0; i < 5; i++) {
    if (util_sys_inb(STAT_REG, &stat) != OK)
      continue;
    if ((stat & IBF) == 0) {
      if (sys_outb(KBC_CMD_REG, WRITE_BYTE_TO_MOUSE) != OK)
        return 1;
      tickdelay(micros_to_ticks(DELAY_US));
      if (sys_outb(OUT_BUF, cmd) != OK)
        return 1;

      uint8_t resp;
      for (int j = 0; j < 5; j++) {
        if (util_sys_inb(OUT_BUF, &resp) != OK)
          continue;
        if (resp == ACK)
          return 0;
        if (resp == NACK)
          break; 
        if (resp == ERROR)
          return 1;
      }
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }
  return 1;
}

int(mouse_disable_data_reporting)() {
  return mouse_write_cmd(DISABLE_DATA_REPORT);
}



bool mouse_packet_ready() {

  return packet_ready;

}



struct packet get_mouse_packet() {

  packet_ready = false;
  return current_packet;

}



void parse_mouse_packet(uint8_t bytes[3], struct packet *pp){
  pp->bytes[0] = bytes[0];
  pp->bytes[1] = bytes[1];
  pp->bytes[2] = bytes[2];
  pp->lb = bytes[0] & BIT(0);
  pp->rb = bytes[0] & BIT(1);
  pp->mb = bytes[0] & BIT(2);
  pp->x_ov = bytes[0] & BIT(6);
  pp->y_ov = bytes[0] & BIT(7);
  pp->delta_x = (bytes[0] & BIT(4)) ? (int16_t) (0xFF00 | bytes[1]) : bytes[1];
  pp->delta_y = (bytes[0] & BIT(5)) ? (int16_t) (0xFF00 | bytes[2]) : bytes[2];
}
