#include "mouse.h"
#include "i8042.h"
#include <lcom/lcf.h>
static int mouse_hook_id;
static int current_data_packet = 0;

uint8_t dataPackets[3];



void mouse_delay(){
  tickdelay(micros_to_ticks(20000));
}

int disableMouseDataReporting(){
  return writeByteToMouse(0xF5);
}

int enableMouseDataReporting(){
//  return mouse_enable_data_reporting();
  if(enableMouseStreaming() != OK){
    return 1;
  }
  return writeByteToMouse(0xF4);
}

int writeByteToMouse(uint32_t command){
  while(true) { 
    if(check_input_buffer() != OK || sys_outb(0x64, 0xD4) != OK){
      //Do nothing
    }
    else if(check_input_buffer() || sys_outb(0x60, command )){
      //Do nothing
    }
    else if(check_output_buffer() == OK) return 0;
    mouse_delay();
  }
  return 0;
}

int enableMouseStreaming(){
  return writeByteToMouse(0xEA);
}

void enableMouseRemote(){
  writeByteToMouse(0xF0);
}

int mouse_subscribe_int(uint8_t *hook_id){
  //IRQ_EXCLUSIVE
  mouse_hook_id = *hook_id;
  return sys_irqsetpolicy(MOUSE_IRQ, IRQ_EXCLUSIVE | IRQ_REENABLE, &mouse_hook_id);
}

int mouse_unsubscribe_int(){
  return sys_irqrmpolicy(&mouse_hook_id);
}

int check_input_buffer(){
  uint32_t status;
  if(sys_inb(STATUS_REG, &status) == OK){
    if(status & BIT(1)){
      //Input buffer is full, can't write commands or arguments
      return 1;
    } else {
      return 0;
    }

  } else {
    printf("Error reading from status register in mouse.c\n");
    return 1;
  }
}

int check_output_buffer(){
  uint32_t status;
  if(sys_inb(STATUS_REG, &status) == OK){
    if(status & BIT(0)){
      uint32_t outputBuffer;
      if(sys_inb(0x60, &outputBuffer) != OK){
          printf("Error line 83\n");
          return 1;
      }
      //If everything is ok, return OK
      return outputBuffer != 0xFA;
    } else {
      return 0;
    }

  } else {
    printf("Error reading from status register in mouse.c\n");
    return 1;
  }
}

void mouse_read(){
  int timeout = 3;
  uint32_t readData;
  while(timeout){
    if(check_input_buffer() == OK){
      if(sys_inb(0x60, &readData) == OK){
        //printf("Current_data_packet = %d, %x\n", current_data_packet, readData);
        if(current_data_packet != 0 || (current_data_packet == 0 && (readData & BIT(3)) == BIT(3))){
          // BIT 3 of first packet is always 1
          //read of a packet success
          dataPackets[current_data_packet++] = readData;
        }
        break;
      }
    }
    timeout--;
    //mouse_delay();
  }
}

struct packet readMouseDataPacket(){
  struct packet p;
  for(unsigned i=0; i < 3; i++){
    p.bytes[i] = dataPackets[i];
  }
  
  p.lb = p.bytes[0] & BIT(0);
  p.rb = p.bytes[0] & BIT(1);
  p.mb = p.bytes[0] & BIT(2);
  p.x_ov = p.bytes[0] & BIT(6);
  p.y_ov = p.bytes[0] & BIT(7);
  uint16_t deltaXAdd = 0x0,  deltaYAdd = 0x0;
  if(p.bytes[0] & BIT(4)){
    deltaXAdd = 0xFF00;
  }
  if(p.bytes[0] & BIT(5)){
    deltaYAdd = 0xFF00;
  }
  p.delta_x = deltaXAdd | p.bytes[1];
  p.delta_y = deltaYAdd | p.bytes[2];
  current_data_packet = 0;
  return p;
}

bool isMouseDataPacketReady(){
  return current_data_packet == 3;
}

void (mouse_ih)(){
  mouse_read();
  
}
