#include <lcom/lcf.h>
#include <mouse.h>
#include <keyboard.h>
#include <i8042.h>
#include <utils.h>
#include <stdint.h>

int MOUSE_HOOK_ID = MOUSE_IRQ;
uint8_t PACKET[3]={0,0,0};
int PACKET_BYTE_COUNTER=0;

int (mouse_subscribe_int)(uint8_t *bit_no) {
  *bit_no = MOUSE_HOOK_ID;

  //subscribe a notification on every interrupt in the IRQ12 (interrupt request line) of the Mouse
  if(sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &MOUSE_HOOK_ID) != OK){
    return 1;
  }
  return 0;
}

int (mouse_unsubscribe_int()) {
  //unsubscribe a previous subscription of a specific interrupt notification (the one associated with the HOOK_ID)
  if(sys_irqrmpolicy(&MOUSE_HOOK_ID) != OK){
    return 1;
  }
  return 0;
}

void (mouse_ih)(void) {
  uint8_t packet_byte;

  if(util_sys_inb(OUT_BUF,&packet_byte) != 0) return;
  PACKET[PACKET_BYTE_COUNTER % 3] = packet_byte;
  PACKET_BYTE_COUNTER++;
}

void build_packet_struct(struct packet *p, uint8_t *packet){

  (*p).bytes[0] = packet[0];
  (*p).bytes[1] = packet[1];
  (*p).bytes[2] = packet[2];

  (*p).lb = (packet[0] & B1_LEFT);
  (*p).rb = ((packet[0] & B1_RIGHT));
  (*p).mb = ((packet[0] & B1_MID));

  if(((packet[0] & B1_XSIGN)) != 0) //x is negative
    (*p).delta_x = -((packet[1] ^ 0xFF) + 1);
  else
    (*p).delta_x = packet[1];
  
  if(((packet[0] & B1_YSIGN)) != 0) //x is negative
    (*p).delta_y = -((packet[2] ^ 0xFF) + 1);
  else
    (*p).delta_y = packet[2];

  (*p).x_ov = ((packet[0] & B1_XOVFL));
  (*p).y_ov = ((packet[0] & B1_YOVFL));
}

int mouse_enable_data_rep() {

  if(mouse_write_command(MC_EN_DATA_REP) != 0) 
    return 1;

  return 0;
}

int mouse_disable_data_rep() {

    if(mouse_write_command(MC_DIS_DATA_REP) != 0)
        return 1;
    return 0;
}

int mouse_write_command(uint8_t cmd){
  int fail = 0;
  int failLimit = 5;
  uint8_t state;
  uint8_t ackB;

    while (fail < failLimit) {
      if(kbc_issue_command(&state,KBC_WRITE_MOUSE) != 0){
        tickdelay(micros_to_ticks(DELAY_US));
        fail++;
      }
      else{
        //mouse commands are written as arguments to de write mouse kb command
        if(kbc_write_command_args(cmd) != 0)
          return 1;

        

        if(mouse_check_ack(&ackB)!=0)
          return 1;

        if(ackB == MC_ACK) return 0;
        else if(ackB == MC_NACK) fail++;
        else return 1;
        
      }
    };
    return 1; // after 5 error quits trying
}

int mouse_check_ack(uint8_t *ackB){
  if(kbc_read_command_return(ackB) != 0)
    return 1;
  return 0;
}

int actual_x_len = 0, actual_y_len = 0;
bool drawing_first_line = false; // true if we are drawing the first line
bool drawing_second_line = false; // true if we are drawing the second line
bool first_drawing_complete=false; // true if the first line is complete
state_t state = INIT;
