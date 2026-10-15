#include <lcom/lcf.h>
#include "mouse.h"

static int hook_id = 3;
static struct packet packet;

static uint8_t packet_array[3];
static int packetIndex = 0;
static uint8_t curentByte = 0;

struct packet* get_packet(){
  return &packet;
}

int (mouse_subscribe_int)(uint8_t *bit_no){
  *bit_no = hook_id;
  if(sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != 0) {
    printf("Failed to subscribe mouse interrupts.\n");
    return 1;
  }
  return 0;
}

int (mouse_unsubscribe_int)() {
  if(sys_irqrmpolicy(&hook_id) != 0) {
    printf("Failed to unsubscribe mouse interrupts.\n");
    return 1;
  }
  return 0;
}

void (mouse_ih)(){
  if (kbc_read_data(KBC_OUT_BUF, &curentByte, 1) != 0) {
    printf("Erro ao ler byte do mouse.\n");
  }
}

void packet_parse(){
  memcpy(packet.bytes, packet_array, 3);
  packet.lb = packet_array[0] & MOUSE_LB;
  packet.rb = packet_array[0] & MOUSE_RB;
  packet.mb = packet_array[0] & MOUSE_MB;
  packet.x_ov = packet_array[0] & MOUSE_XOVF;
  packet.y_ov = packet_array[0] & MOUSE_YOVF;
  packet.delta_x = (packet_array[0] & MOUSE_XSIGN) ? (0xFF00 | packet_array[1]) : packet_array[1];
  packet.delta_y = (packet_array[0] & MOUSE_YSIGN) ? (0xFF00 | packet_array[2]) : packet_array[2];
}

int check_packet_complete(){
  if (packetIndex == 0 && !(curentByte & BIT(3))) {
    return 0; 
  }

  packet_array[packetIndex] = curentByte;
  packetIndex++;

  if (packetIndex == 3) {
    packetIndex = 0;
    return 1; 
  }

  return 0; 
}

int mouse_write_command(uint8_t command){
  uint8_t attemps = MAX_TRIES;
  uint8_t mouse_response;

  do {
    attemps--;
    if (kbc_write_command(KBC_CMD_REG, WRITE_BYTE_TO_MOUSE)) return 1;
    if (kbc_write_command(KBC_IN_BUF, command)) return 1;
    tickdelay(micros_to_ticks(WAIT_KBC));
    if (util_sys_inb(KBC_OUT_BUF, &mouse_response)) return 1;
    if (mouse_response == KBC_ACK) return 0;
  } while (mouse_response != KBC_ACK && attemps);

  return 1;
}


