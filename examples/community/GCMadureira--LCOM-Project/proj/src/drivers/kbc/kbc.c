/** 
 * @file kbc.c
 * @brief Source code file for all the logic related to the kbc
 */


#include "kbc.h"


int (kbc_read_command)(uint8_t* command){
  if(sys_outb(KBC_CMD_REG, RD_CMD_BYTE)) return 1;
  return util_sys_inb(KBC_OUT_REG, command);
}

int (kbc_write_command)(uint8_t command){
  return sys_outb(KBC_CMD_REG, command);
}

int (kbc_write_command_arg)(uint8_t arg){
  return sys_outb(KBC_CMDARG_REG, arg);
}

bool (kbc_valid_output)(bool aux){
  uint8_t status;
  if(util_sys_inb(KBC_STAT_REG, &status)) return false; //error reading status register
  if(status & (KBC_PAR_ERR | KBC_TO_ERR)) return false; //parity error or timeout error
  if(!(status & KBC_ST_OBF)) return false; //output buffer empty

  if(aux) return status & KBC_ST_AUX; //mouse data
  else return !(status & KBC_ST_AUX); //keyboard data
}

void (kbc_discard_output)(){
  uint8_t discard;
  util_sys_inb(KBC_OUT_REG, &discard);
}
