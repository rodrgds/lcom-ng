#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#include "mouse.h"
 

int hook_id_mouse = 12; 
uint8_t packetbyte; 

//Subscribes mouse interrupts in exclusive mode on IRQ PORT 12
int (mouse_subscribe_int)(uint8_t *bit_no){

  hook_id_mouse = MOUSE_IRQ; 
  *bit_no = hook_id_mouse; 

  if(sys_irqsetpolicy(MOUSE_IRQ, IRQ_ENABLE | IRQ_EXCLUSIVE, &hook_id_mouse) != OK) return 1; 

  return 0; 
}

//Unsubscribes mouse interrupts
int (mouse_unsubscribe_int)(void){

  //Unsubscribes mouse interrupts
  if(sys_irqrmpolicy(&hook_id_mouse) != OK) return 1; 

  return 0; 
}

//Checks if its possible to write arguments to the Input Buffer 
bool (mouse_check_IBF)(void){
    uint8_t status; 

    if(util_sys_inb(STATUS_REG, &status)) return false; 

    //IBF BIT IS ACTIVE SO WE CANNOT WRITE TO IBF
    if(status & IBF)
      return false; 
    //IBF BIT ISN'T ACTIVE SO WE CAN WRITE TO IT (PASS ARGUMENTS TO THE COMMANDS)
    else
      return true; 
}

//Writes command to the IBF and returns success or failure and the ACK Byte by reference on variable response
int (mouse_write_command)(uint32_t argument, uint8_t* response){

  //Count will try a few times before actually declaring an error
  int count = 0; 

  //Attemps 3 times to execute the command
  while(count < 3){
    if(!mouse_check_IBF()){
      count++; 
      continue;
    }
    else{

      //Writes argument to the IBF
      if(sys_outb(IBF, argument)) return 1; 

      //Reads Acknowledge from OBF
      if(util_sys_inb(OBF, (response))) return 1; 

      return 0; 
    }
    count ++; 
  }

  return 1; 
}

int (mouse_enables_data_reporting)(void){

  uint8_t response = 0X00; //Asserts the acknowledge byte
  uint8_t seconderror = 0X00; //Asserts if there was a second error

  while(seconderror < 2){

    if(sys_outb(CONTROL_REG, WRITE_BYTE_TO_MOUSE)) return 1; 

    if(mouse_write_command(ENABLE_DATA_REPORTING, &response)) return 1; 

    //Acknowledge byte assertion
    if(response == ACK) return 0; 
    else if(response == NACK) seconderror++; 
    else if(response == ERROR) return 1; 
    else return 1;
  }
  
  return 1; 
}

int (mouse_disable_data_reporting)(void){

  uint8_t response = 0x00; 
  uint8_t seconderror = 0; 

  //Attemps 2 times to pass the command to the Mouse
  while(seconderror < 2){
    
    //Sends Write byte to mouse command to mouse's control register
    if(sys_outb(CONTROL_REG, WRITE_BYTE_TO_MOUSE)) return 1; 

    //Writes command args to mouse
    mouse_write_command(DISABLE_DATA_REPORTING, &response); 

    //Handles response from the mouse (Acknowledge byte)
    if(response == ACK) return 0; 
    else if (response == NACK) seconderror++; 
    else if (response == ERROR) return 1; 
    else return 1; 
  }

  return 1;  
}

int (mouse_reset_to_default)(void){

  //Disables data Reporting
  if(mouse_disable_data_reporting()) return 1;

  //Unsubscribes mouse interrupts 
  if(mouse_unsubscribe_int()) return 1; 

  return 0; 
}

int (mouse_check_OBF)(void){
  
  uint8_t status; 

  //Reads status byte of mouse's OBF
  if(util_sys_inb(STATUS_REG, &status)) return 1; 

  if(status & OBF_FULL && status & AUX){
    if(status & PARITY_ERROR || status & TIMEOUT_ERROR)
      return 1; 
    else 
      return 0; 
  }
  else
    return 1;   
}

//Interrupt handler reads the current byte from the OBF
void(mouse_ih)(void){
  //If didn't occurr any error while checking OBF, then it is safe to read the byte out of the OBF
  if(mouse_check_OBF()){
    packetbyte = 0X00; 
  }
  else{
    if(util_sys_inb(OBF, &packetbyte)){
      packetbyte = 0X00; 
    } 
  }

}
 
mouse_cursor_t* cursor; 

mouse_cursor_t*
cursor_new()
{
  mouse_cursor_t* new_cursor = (mouse_cursor_t*) malloc(sizeof(mouse_cursor_t));
  if(new_cursor == NULL) return NULL; 

  new_cursor->x = 10; 
  new_cursor->y = 10; 
  new_cursor->cursorsprite = sprite_new((const char* const*)cursor_xpm, new_cursor->x, new_cursor->y);

  return new_cursor; 
}

void draw_mouse_cursor(){
  sprite_draw(cursor->cursorsprite);
}

void mouse_update(struct packet * pacote){
  
 cursor->x  = cursor->x++; 

  draw_mouse_cursor();
}

//Falta colocar a funcao para updatar a posicao do rato

void (mouse_parse_packet)(struct packet* packet){
  packet->rb = packet->bytes[0] & RIGHT_PRESS; 
  packet->mb = packet->bytes[0] & MID_PRESS; 
  packet->lb = packet->bytes[0] & LEFT_PRESS; 

  packet->x_ov = packet->bytes[0] & X_OVFL; 
  packet->y_ov = packet->bytes[0] & Y_OVFL; 

  if(packet->bytes[0] & X_SIGN){
    packet->delta_x = packet->bytes[1] | 0XFF00; 
  } 
  else 
    packet->delta_x = packet->bytes[1]; 

  if(packet->bytes[0] & Y_SIGN){
    packet->delta_y = packet->bytes[2] | 0XFF00; 
  }
  else 
    packet->delta_y = packet->bytes[2]; 
}


