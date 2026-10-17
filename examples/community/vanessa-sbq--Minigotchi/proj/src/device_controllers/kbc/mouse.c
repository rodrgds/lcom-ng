#include "mouse.h"

int hook_id = 3; // Cannot be IRQ's real line number

static uint8_t readByte;
static bool error;


/**
 * @brief Getter for readByte
 */
uint8_t mouse_get_readByte(){
  return readByte;
}

/**
 * @brief Getter for error
 */
bool mouse_get_error(){
  return error;
}

/**
 * @brief Setter for error
 */
void mouse_set_error(bool er){
  error = er;
}

/**
 * @brief Subscribe mouse interrupts 
 */
int (mouse_subscribe_int)(uint8_t* bit_no){
  if (bit_no == NULL){
    printf("\nError while executing keyboard_subscribe_int, bit_no is NULL.\n");
    return 1;
  }
  *bit_no = hook_id;
  if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) ){
    printf("\nError while executing sys_irqsetpolicy.\n");
    return 1;
  }
  return 0;
}

/**
 * @brief Unsubscribe mouse interrupts 
 */
int (mouse_unsubscribe_int)(){
  if (sys_irqrmpolicy(&hook_id) != 0){
    printf("\nError while exectuing keyboard_unsubscribe_int.\n");
    return 1;
  }
  return 0;
}

/**
 * @brief Sends a command to the mouse 
 */
int send_byte_to_mouse(uint8_t commandToSend){
  uint8_t out_buf_byte = 0xFE;
  int tries = 10;
  while( out_buf_byte != ACK && tries >= 0 ) {

    // Tell kbc we want to send a command to the mouse.
    if (writeToKBCPort(CMDS_REG_PORT, MOUSE_WRITE_COMMAND) != 0) { 
      return 1;
    }

    // Send the command to the mouse.
    if (writeToKBCPort(ARGS_REG_PORT, commandToSend) != 0) {
      return 1;
    }

    tickdelay(micros_to_ticks(20000));
    
    // Read the acknoledgment byte.
    if (util_sys_inb(0x60, &out_buf_byte) != 0) return 1;

    // Only return if no error occoured.
    tries--;
  }
  return 0;
}

/**
 * @brief Mouse interrupt handler, reads the output buffer upon an interrupt 
 */
void (mouse_ih)(){
  if (readFromOutputBuffer(&readByte) != 0){ // Something went wrong.
    readByte = 0x00;
    error = true;
  }
}

