#include "../include/mouse.h"

static int hook_id = 12;
static uint8_t packet[3];

int mouse_subscribe(uint8_t* bit_no) {
  *bit_no = hook_id;
  if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id)) return 1;
  return 0;
}

int mouse_unsubscribe() {
  if (sys_irqrmpolicy(&hook_id)) return 1;
  return 0;
}

//Checks if its possible to write arguments to the Input Buffer 
bool (mouse_check_IBF)(void){
    uint8_t status; 

    if(util_sys_inb(STATREG, &status)) return false; 

    //IBF BIT IS ACTIVE SO WE CANNOT WRITE TO IBF
    if(status & IBF)
      return false; 
    //IBF BIT ISN'T ACTIVE SO WE CAN WRITE TO IT (PASS ARGUMENTS TO THE COMMANDS)
    else
      return true; 
}

static int byte_counter = 0;

int get_byte_count() { return byte_counter; }

void (mouse_ih)(void) {
    uint8_t statuscode, byte;

    if(byte_counter >= 3) byte_counter = 0;

    if (util_sys_inb(STATREG, &statuscode)) return;

    if (statuscode & (TIMEOUT | PAR_ERROR)) {
        util_sys_inb(OBF, &byte);
        return;
    }

    if (!(statuscode & AUX) || !(statuscode & OBF_FULL)) return;

    if (util_sys_inb(OBF, &byte)) return;

    if ((byte & FIRST_BYTE)  || byte_counter) {
        packet[byte_counter++] = byte;
    }
}

mouse_cursor_t*
cursor_new()
{
  mouse_cursor_t* cursor = (mouse_cursor_t*) malloc(sizeof(mouse_cursor_t));
  if(cursor == NULL) return NULL; 

  cursor->x = 10; 
  cursor->y = 10; 
  cursor->cursorsprite = sprite_new((const char* const*)cursor_xpm, cursor->x, cursor->y);

  return cursor;
}

struct packet mouse_get_packet() {
  struct packet pp;
  pp.bytes[0] = packet[0];
  pp.bytes[1] = packet[1];
  pp.bytes[2] = packet[2];
  pp.rb = pp.bytes[0] & RMB;
  pp.mb = pp.bytes[0] & MMB;
  pp.lb = pp.bytes[0] & LMB;
  pp.delta_x = pp.bytes[0] & X_SIGN ? (int16_t)(0xFF00 | pp.bytes[1]) : pp.bytes[1];
  pp.delta_y = pp.bytes[0] & Y_SIGN ? (int16_t)(0xFF00 | pp.bytes[2]) : pp.bytes[2];
  pp.x_ov = pp.bytes[0] & X_OVFL;
  pp.y_ov = pp.bytes[0] & Y_OVFL;

  return pp;
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

    if(sys_outb(CMDREG, WRITE_BYTE_TO_MOUSE)) return 1; 

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
    if(sys_outb(CMDREG, WRITE_BYTE_TO_MOUSE)) return 1; 

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
