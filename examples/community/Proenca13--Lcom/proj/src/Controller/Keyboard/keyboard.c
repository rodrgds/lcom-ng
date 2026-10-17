#include "keyboard.h"
int keyboard_hook_id = 1;
uint8_t scancode = 0;
int (keyboard_subscribe_interrupts)(){
  return sys_irqsetpolicy(KEYBOARD_IRQ,IRQ_REENABLE|IRQ_EXCLUSIVE,&keyboard_hook_id);
}
int (keyboard_unsubscribe_interrupts)(){
  return sys_irqrmpolicy(&keyboard_hook_id);
}
void (kbc_ih)(){
  read_KBC_output(KBC_OUT_BUF,&scancode,0);
}
int (keyboard_restore)(){
  uint8_t command_byte ;
  if(write_KBC_command(KBC_IN_BUFFER,KBC_READ_CMD)!=0)return 1;
  if(read_KBC_output(KBC_OUT_BUF,&command_byte,0)!=0)return 1;
  command_byte = command_byte | BIT(0);
  if(write_KBC_command(KBC_IN_BUFFER,KBC_WRITE_CMD_BYTE)!=0)return 1;
  if(write_KBC_command(KBC_WRITE_CMD_BYTE,command_byte)!=0)return 1;
  return 0;
}
