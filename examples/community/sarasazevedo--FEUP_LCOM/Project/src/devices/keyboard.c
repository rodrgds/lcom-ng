#include <lcom/lcf.h>
#include "keyboard.h"


int keyboard_hook_id = 1;
uint8_t scancode;  //Read scancode

uint8_t bytes[2];  
int keyboard_size = 0;
bool keyboard_complete;  //True when obtaining a complete scancode


//Subscribes Keyboard interrupts and disables default IH
int (keyboard_subscribe_int)(uint8_t *irq_set_mask) {
  if (irq_set_mask == NULL) return 1;

  *irq_set_mask = BIT(keyboard_hook_id);

  if (sys_irqsetpolicy(KEYBOARD_IRQ_LINE, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id) != 0) return 1;  //IRQ_EXCLUSIVE to disable default IH

  return 0;

}


//Unsubscribes Keyboard interrupts
int (keyboard_unsubscribe_int)() {
  if (sys_irqrmpolicy(&keyboard_hook_id) != 0) return 1;
  return 0;
}




//Configure KBC to enable interrupts on OBF from Keyboard
int (keyboard_int_enabler)() {

  uint8_t commandByte;    

  if (kbc_get_conf(&commandByte) != 0) return 1;   //Obtain previous configuration

  commandByte = commandByte | ENABLE_KEYBOARD_INT; //Set bit

  if (kbc_configure(commandByte) != 0) return 1;  //Send new configuration

  return 0;

}




//Configure KBC to disable interrupts on OBF from Keyboard
int (keyboard_int_disabler)() {

  uint8_t commandByte;    
  if (kbc_get_conf(&commandByte) != 0) return 1;   //Obtain previous configuration

  commandByte = commandByte ^ ENABLE_KEYBOARD_INT;

  if (kbc_configure(commandByte) != 0) return 1;   //Send new configuration

  return 0;

}



//Reads a byte coming from the keyboard and tries to build a scancode
void (kbc_ih)() {
   bool from_mouse = false;
   if(keyboard_complete) {
    keyboard_size = 0;
    keyboard_complete = false;
  }
  kbc_get_out(&scancode, from_mouse);
   bytes[keyboard_size] = scancode;
    keyboard_size++;

  if ((scancode != FIRST_OF_TWO_BYTES) || keyboard_size == 2)  {   //If it is a 1-byte scancode
      keyboard_complete = true;
    }


  
}





