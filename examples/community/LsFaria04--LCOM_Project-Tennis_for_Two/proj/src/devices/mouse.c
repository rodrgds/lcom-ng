#include "mouse.h"
#include "i8042.h"

static int hook_id_mouse = 0;
static uint8_t byte_index = 0;            
static uint8_t current_byte;
static struct packet packet;

//getter ==============================================

int (get_mouse_hook_id)(){
  return hook_id_mouse;
}

uint8_t (get__mouse_byte_index)(){
  return byte_index;
}

void (reset_byte_index)(){
  byte_index = 0;
}

uint8_t (get_current_byte)(){
  return current_byte;
}

struct packet (get_mouse_packet)(){
  return packet;
}

//====================================================

void (mouse_ih)(){

  //handles the mouse interrupts
  if(mouse_read_byte() != 0){
    printf("Error in the byte reading \n");
    current_byte = 0;
  }

}

int (mouse_read_status)(uint8_t *status){

    //Reads the status at the status register
    if(util_sys_inb(KBC_ST_REG, status) != 0){
          return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int (mouse_read_byte)(){
    uint8_t value = 0;
    uint8_t status = 0;
    uint8_t maxIterations = 20; //maximum number of iterations that the loop will contain (avoids the program to block)

    while(maxIterations){
      //reads the status
      if(mouse_read_status(&status) != 0){
        return EXIT_FAILURE;
      }

      //checks if there was an error
      if(status & (KBC_PARITY | KBC_TIMEOUT)){
            return EXIT_FAILURE;
      }

      //checks if the output buffer is full and the byte is the mouse
      if((status & KBC_OBF) && (status & KBC_AUX)) {
        
        //Reads the byte
        if(util_sys_inb(KBC_OUT_BUF, &value) != 0){
          return EXIT_FAILURE;
        }

        current_byte = value;
        return EXIT_SUCCESS;
      }

      //Waits enough time until trying again
        tickdelay(micros_to_ticks(DELAY_US));
        maxIterations--;
    }

    return EXIT_FAILURE;
  
}

int (mouse_subscribe_int)(uint8_t *bit_no){
    *bit_no = BIT(2);  
    hook_id_mouse = *bit_no;

    if(sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse) != 0)
        return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

int (mouse_unsubscribe_int)(){
    if(sys_irqrmpolicy(&hook_id_mouse) != 0){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}


void (mouse_insert_byte)(){

  //checks if the bit 3 (mouse control byte) is active and if the current index is 0 (new byte)
  if(byte_index == 0){
    if(current_byte & MOUSE_CONTROL){
      packet.bytes[byte_index] = current_byte;
      byte_index++;
    }
    else{
      printf("The byte isn't from a new packet");
    }
  }

  else if(byte_index > 0){
    packet.bytes[byte_index] = current_byte;
    byte_index++;
  }
}

int (mouse_write_command)(uint8_t command, int port){
  uint8_t status = 0;
  uint8_t maxIterations = 20; //maximum number of iterations that the loop will contain (avoids the program to block)

    while( maxIterations ) {
        if(mouse_read_status(&status) != 0){
          return EXIT_FAILURE;
        }

        //checks if there wasn't any error
        if(status & (KBC_PARITY | KBC_TIMEOUT)){
            return EXIT_FAILURE;
        }

        //cheks if the the input buffer is empty
        if(((status & KBC_IBF)) == 0 ) {
          //write the command
            sys_outb(port, command); 
            return EXIT_SUCCESS;
        }

        //waits enough time until trying again
        tickdelay(micros_to_ticks(DELAY_US)); // e.g. tickdelay()
        maxIterations--;
    }

    return EXIT_FAILURE;

}

uint16_t (convert_to_2sComplement)(uint8_t mouse_byte){
  //converts the byte into 2's complement negative number
  return (0xFF00 | mouse_byte);
}

void (mouse_insert_in_packet)(){

  //fills the rest of the areas with the apropriate value
  packet.lb = packet.bytes[0] & MOUSE_LB;
  packet.mb = packet.bytes[0] & MOUSE_MB;
  packet.rb = packet.bytes[0] & MOUSE_RB;

  //check if x and y values are negative (2's complement) and inserts 1 if so
  if(packet.bytes[0] & MOUSE_MSB_X){
     packet.delta_x = convert_to_2sComplement(packet.bytes[1]);
  }

  else{
      packet.delta_x = packet.bytes[1]; 
  }

  if(packet.bytes[0] & MOUSE_MSB_Y){
    packet.delta_y = convert_to_2sComplement(packet.bytes[2]);
  }

  else{
    packet.delta_y = packet.bytes[2];
  }


  //fills the rest of the areas with the apropriate value
  packet.x_ov = packet.bytes[0] & MOUSE_X_OVF;
  packet.y_ov = packet.bytes[0] &  MOUSE_Y_OVF;

}

int (mouse_write_byte)(uint8_t command){
  uint8_t return_byte = 0;
  int maxIterations = 100;

  while(maxIterations){

    //0xD4 commands the KBC to forward its argument to the mouse without any interpretation
    if(mouse_write_command(MOUSE_WRITE_BYTE,KBC_CMD_REG) != 0){
      return EXIT_FAILURE;
    }

    //writes the arguments to the mouse
    if(mouse_write_command(command,KBC_OUT_BUF) != 0){
      return EXIT_FAILURE;
    }

    //read the response from the mouse
    if(util_sys_inb(KBC_OUT_BUF, &return_byte) != 0){
      return EXIT_FAILURE;
    }
    
    //if is ACK (everything ok) exit the loop
    if(return_byte == MOUSE_ACK){
      return EXIT_SUCCESS;
    }

    maxIterations--;
  }

  return EXIT_FAILURE;
}

//===========
void clear_mouse_packet() {
  byte_index = 0;
  current_byte = 0;
  memset(&packet, 0, sizeof(packet));
}
