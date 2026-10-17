#include "kbc.h"
int (read_KBC_status)(uint8_t* status){
  return util_sys_inb(KBC_STATUS_REGISTER,status);
}
int (read_KBC_output)(uint8_t port, uint8_t *output, uint8_t mouse){
  if(output == NULL)return 1;
  uint8_t status ;
  uint8_t attemps = 10;
  if (read_KBC_status(&status)!=0)return 1;
  while(attemps){
    if(status & OUT_BUFFER_FULL){
      if(util_sys_inb(KBC_OUT_BUF,output)!=0)return 1;
      if(status & PARITY_ERROR)return 1;
      if(status & TIMEOUT_ERROR)return 1;
      if(!MOUSE & mouse)return 1;
      return 0;
    }
    attemps--;
    tickdelay(micros_to_ticks(DELAY_US));
  }
  return 1;

}
int (write_KBC_command)(uint8_t port, uint8_t commandByte){
  uint8_t status ;
  uint8_t attemps = 10;
  if( read_KBC_status(&status)!=0)return 1;
  while(attemps){
    if((status & IN_BUFFER_FULL)==0){
      if(sys_outb(port,commandByte)!=0)return 1;
      if(status & PARITY_ERROR)return 1;
      if(status & TIMEOUT_ERROR)return 1;
      return 0;
    }
    attemps--;  
    tickdelay(micros_to_ticks(DELAY_US));

  }
  return 1;
}
