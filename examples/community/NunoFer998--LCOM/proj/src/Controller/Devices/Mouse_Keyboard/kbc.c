#include "kbc.h"

int (write_command_byte)(uint8_t port, uint8_t command) {
  uint8_t status;
  int trys = 10;
  while( trys > 0 ) {
    if(aux_sys_inb(KBC_ST_REG, &status) != 0) return 1; 
    if( (status & KBC_IBF) == 0 ) {
      if (sys_outb(port, command) != 0) return 1; 
      return 0;
    }
    tickdelay(micros_to_ticks(20000)); 
    trys--;
  }
  return 1;
}

int (read_command_byte)(uint8_t* command) {
  uint8_t status;
  int trys = 10;
  while(trys > 0) {
    aux_sys_inb(KBC_ST_REG, &status); 
    if( status & KBC_OBF ) {
      if (aux_sys_inb(KBC_OUT_BUF, command) == OK); 
      if ( (status &(KBC_PAR_ERR | KBC_TO_ERR)) == 0 ) {
        return 0;
      }
      else
        return 1;
    }
  tickdelay(micros_to_ticks(20000)); 
  trys--;
  }
  return 1;
}

int (mouse_read_command_byte)(uint8_t* response) {
  uint8_t status;
  int trys = 10;
  while(trys > 0) {
    aux_sys_inb(KBC_ST_REG, &status); 
    if( status & KBC_OBF ) {
      if (aux_sys_inb(KBC_OUT_BUF, response) == OK); 
      if (((status &(KBC_PAR_ERR | KBC_TO_ERR)) == 0) && (MOUSE_DATA & status)) {
        return 0;
      }
    }
    tickdelay(micros_to_ticks(20000)); 
    trys--;
  }
  return 1;
}
