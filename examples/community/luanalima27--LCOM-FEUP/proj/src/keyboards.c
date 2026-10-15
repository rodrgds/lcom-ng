#include <lcom/lcf.h>
#include "keyboards.h"
#include <stdint.h>
#include "i8042.h"


//#define KEYBOARD_IRQ 0

int hook_id_kbd = 0;
uint8_t scancode = 0;
//subscribe to the keyboard controller
int kbc_subscribe_int(){
  //MAKE SURE TO ALWAYS SET THE HOOK_ID TO THE SAME AS THE BIT_NO!
  hook_id_kbd = IRQ1;
  //set policy for IRQ (KEYBOARD NEEDS BOTH IRQ_REENABLE AND IRQ_EXCLUSIVE)
  if (sys_irqsetpolicy(1, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_kbd) != 0) return 1;

  //return 0 if success
  return 0;
}

int kbc_unsubscribe_int(){
  if (sys_irqrmpolicy(&hook_id_kbd) != 0) return 1;
  return 0;
}

//reads the scancodes
void (kbc_ih)() {

  // variable to store the state of the status register
  uint8_t stat;

  //number of iterations that does not cause the program to timeout
  for(int i = 0; i < 2; i++) {
    //read the status register
    if(util_sys_inb(STATUS_REGISTER, &stat) != 0) {
      printf("Error, could not read status\n");
      return;
    }
  
  //check in the status of the keyboard if the output buffer is full
  if(stat & OBF) {
    // reads the scancode from the output buffer
    if(util_sys_inb(OUTPUT_BUFFER, &scancode) != 0) {
      printf("Error, could not read scancode\n");
      return;
    }

    //checks if timout error or parity error happens
    if((stat & (TIMEOUT | PARITY) ) == 0) {
      return;
    }
    else {
      printf("Error, parity or timeout error detected");
      return;
    }

  }
  //waits X miliseconds
  tickdelay(micros_to_ticks(DELAY_US));
  }

}

//prints the scancodes
void kbd_print_scancode_assemble() {
  
  //8 bit numer array of size 2
  uint8_t bytes[2];
  //boolean for make code status
  bool make = false;
  //8 bit number for size
  uint8_t size = 1;

  //if scancode is the first byte of a two-byte long scancode (0xE0)
  if(scancode == 0xE0) {
    // size of scancode becomes 2
    size = 2;
    // the last number of the array becomes the scancode
    bytes[1] = scancode;
    return;
  }

  //if scancode is different than 0x81
  if((scancode & 0x80) == 0) {
    //makecode exists
    make = true;
  }

  //the first number of the array becomes the scancode
  bytes[0] = scancode;

  //print scancode
  kbd_print_scancode(make, size, bytes);

  //return size to default
  size = 1;

  //return makecode status to default (false)
  make = false;
  
}

//read keyboard controller
int(kbc_read)() {
  //8 bit number to write status of status register to
  uint8_t stat;

  //while no break
  while(true) {
    //read status register
     if(util_sys_inb(STATUS_REGISTER, &stat) != 0) {
      return 1;
    }
  
  // check in the status of the keyboard if the output buffer is full
  if(stat & OBF) {
    //read the scancode from the output buffer
    if(util_sys_inb(OUTPUT_BUFFER, &scancode) != 0) {
      return 1;
    }

    //checks if timout error or parity error happens
    if((stat & (TIMEOUT | PARITY) ) == 0) {
      return 0;
    }
    else {
      return 1; }
  
  }
  }
}

