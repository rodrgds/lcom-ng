// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "menu.h"
#include "i8042.h"

int m_hook_id = 5;
struct packet pp;
uint8_t data, byte_index = 0;
uint8_t packet[3];
int (mouse_subscribe_int)(uint8_t *bit_no) {

    *bit_no = BIT(m_hook_id);

    if(sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &m_hook_id) != 0) {
      printf("There was an error in sys_irqsetpolicy!\n");
      return 1;
    }

    return 0;
}

int (mouse_unsubscribe_int)() {

    if(sys_irqrmpolicy(&m_hook_id) != 0) {
      printf("There was an error in sys_irqsetpolicy!\n");
      return 1;
    }

    return 0;
}

int (read_kbc_status)(uint8_t* status) {
    if(util_sys_inb(KBC_ST_REG, status) !=0) {
      printf("Error in KBC Status\n");
      return 1;
    }

    return 0;
}

int (read_kbc_output)(uint8_t port, uint8_t *output, uint8_t mouse) {
  uint8_t status, tries = 3;
  while (tries > 0) {

    read_kbc_status(&status);

    if ((status & KBC_OBF) != 0) {

        if(util_sys_inb(port, output) != 0){ 
            printf("Error in reading output!\n");
            return 1;
        }

        if((status & KBC_PAR_ERR) != 0){
            printf("Parity error!\n");
            return 1;
        }

        if((status & KBC_TO_ERR) != 0){
            printf("Timeout error!\n");          
            return 1;
        }

        return 0; 
    }
    tickdelay(micros_to_ticks(20000));
    tries--;
  }
  return 1;
}


void (sync_mouse_bytes)() {
  if (byte_index == 0 && (data & BIT(3))) {
    //BIT 3 is active in CTRL Byte
    packet[byte_index] = data;
    byte_index++;
  }
  else if (byte_index > 0) { 
    //Gets X and Y Shift
    packet[byte_index] = data;
    byte_index++;
  }
}

int (write_kbc_command)(uint8_t port, uint8_t command_byte) {

    uint8_t status, tries = 3;

    while (tries > 0) {

        if (read_kbc_status(&status) != 0){
            printf("Error in writing status!\n");
            return 1;
        }

        if ((status & KBC_IBF) == 0){

            if(sys_outb(port, command_byte) != 0){
                printf("Error in writing command_byte!\n");
                return 1;
            }

            return 0;
        }
        tickdelay(micros_to_ticks(20000));
        tries--;
    }
    
    return 1;
}

int (write_to_mouse)(uint8_t command) {

  uint8_t tries = 3;
  uint8_t mouse_response = '\0';

  while(mouse_response != ACK && tries) {

    if (write_kbc_command(KBC_CMD_REG, WRITE_BYTE_TO_MOUSE)) {
      printf("Error in writing byte to mouse command in write_to_mouse\n");
      return 1;
    } 

    if (write_kbc_command(KBC_OUT_BUF, command)) {
      printf("Error in writing given command in write_to_mouse\n");
      return 1;
    } 

    tickdelay(micros_to_ticks(20000));

    if (util_sys_inb(KBC_OUT_BUF, &mouse_response)) {
      printf("Error in getting mouse response in write_to_mouse\n");
      return 1;
    }

    if (mouse_response == ACK) return 0;

    tries--;
  } 

  return 1;
}

void (mouse_ih)(){
  if(read_kbc_output(KBC_OUT_BUF, &data, 1)) {
    printf("Error in reading byte from mouse\n");
  } 
}



void (build_mouse_packet)() {
  for (int i = 0 ; i < 3 ; i++) {
    pp.bytes[i] = packet[i];
  }

  pp.lb = packet[0] & BIT(0);
  pp.rb = packet[0] & BIT(1);
  pp.mb = packet[0] & BIT(2);
  pp.delta_x = (packet[0] & BIT(4)) ? (0xFF00 | packet[1]) : packet[1];
  pp.delta_y = (packet[0] & BIT(5)) ? (0xFF00 | packet[2]) : packet[2];
  pp.x_ov = packet[0] & BIT(6);
  pp.y_ov = packet[0] & BIT(7);
}
