#include <lcom/lcf.h>
#include "keyboard.h"

int hook_kbd =KEYBOARD_IRQ;
bool status_error =false;
uint8_t scancode=0;
uint8_t scancode_bytes[2]={0,0};

//function to enable keyboard interrupt notifications
int (kbd_subscribe_int)(uint8_t *bit_no){
  *bit_no = hook_kbd;
  if(sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE,&hook_kbd))return 1;
  return 0;
}

//function to disable keyboard interrupt notifications
int (kbd_unsubscribe_int)(){
   return sys_irqrmpolicy(&hook_kbd);
}

//interruption handler
void (kbc_ih)(){
  uint8_t status;
  //reads the status
  util_sys_inb(STATUS_REGISTER,&status);
  //checks if status error ocurred
  if(status & (PARITY_ERROR |TIMEOUT_ERROR|MOUSE_ACTIVE))status_error=true;
  if(status & OBF)util_sys_inb(OUT_BUF,&scancode);
  else scancode=ERROR; //if  there's no new scancode to read 
}


//reenable keyboard interrupts
int (kbd_enable)(){
  uint8_t cmd;

  //reads command byte
  if(sys_outb(IN_BUF,READ_CMD_BYTE))return 1;
  if(util_sys_inb(OUT_BUF,&cmd))return 1;
  
  //changes the command to enable interrupts on obf
  cmd=cmd | ENABLE_KBD_OBF;

  //sets the new command byte
  if(sys_outb(IN_BUF,WRITE_CMD_BYTE))return 1;
  if(sys_outb(OUT_BUF,cmd))return 1;
  return 0;
}



