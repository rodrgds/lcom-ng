#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdint.h>
#include <stdbool.h>

#include "keyboard.h"


static int kbd_hookID = 0;
uint8_t key_scancode = 0;

#ifdef LAB3
int counter = 0;
#endif

int(kbc_subscribe_int)(uint8_t *bit_no){

  kbd_hookID = *bit_no;

  if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hookID) != OK){
    return 1;
  }

  return 0;
}

int(kbc_unsubscribe_int)() {
  if (sys_irqrmpolicy(&kbd_hookID) != OK)
    return 1;
  return 0;
}

int(util_sys_inb)(int port, uint8_t *value) {
  uint32_t var;
  sys_inb(port, &var);
  *value = (uint8_t)var;
#ifdef LAB3
  counter++;
#endif

  return 0;
}

//defined in lab3.c
void(kbc_ih)() {

  uint8_t st;
  util_sys_inb(STAT_REG, &st); 

  if (st & OBF){
    util_sys_inb(OUT_BUF, &key_scancode);
    if ((st & (PAR_ERR | TO_ERR)) == 0)
      return;
  }
}

void processKeyScancode(uint8_t scancode){

  bool make;
  uint8_t size;
  bool has2Bytes = false;

  if (scancode == 0xE0) { //checks if scancode has 2 bytes
    has2Bytes = true;
  }

  else {
    if ((scancode & BIT(7)) == 0)
      make = true;
    else
      make = false;

    if (has2Bytes) {

      size = 2;
      uint8_t bytes[size];

      bytes[0] = 0xE0;
      bytes[1] = scancode;

      kbd_print_scancode(make, size, bytes);
    }

    else {

      size = 1;
      uint8_t bytes[size];

      bytes[0] = scancode;

      kbd_print_scancode(make, size, bytes);
    }
    has2Bytes = false;
  }
}

//Functions related to enable interrupts

int readCommandByte(uint8_t command_byte){
  //Reads from the output buffer 0x60
  if(util_sys_inb(OUT_BUF, &command_byte)==OK)
    return 0;
  else{
    printf("Failed reading controller command byte\n");
    return 1;
  }
}

int writeCommandByte(uint8_t command_byte){
  //Writes to the output buffer 0x60
  if(sys_outb(OUT_BUF, command_byte)==OK){
    return 0;
  }
  else{
    printf("Failed writing controller command byte\n");
    return 1;
  }
}

int processCommandByte(uint8_t command_byte){

  uint8_t status=0;

  //Checks status register
  util_sys_inb(STAT_REG, &status);

  if((status&BIT(1))==0){ //Checks if inputBuffer is full
    sys_outb(KBC_CMD_REG, command_byte);
    return 0;
  }

  return 1;
}

void enableInterrupts(){

  //Checks if can read command byte
  processCommandByte(READ_COMD_BYTE);

  //Reads controller's command byte
  readCommandByte(READ_COMD_BYTE);

  //Set value
  processCommandByte(WRITE_COMD_BYTE);

  //Writes controller's command byte
  writeCommandByte(WRITE_COMD_BYTE| 0x01);

}
