#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include <stdlib.h>
#include "i8042.h"
#include "keyboard.h"

int k_hook_id = 1;
//uint8_t scancode, value;

uint8_t scancode;
uint8_t data;



int (kbc_subscribe_int)(uint8_t *bit_no){
    *bit_no = BIT(k_hook_id);
    sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &k_hook_id);
    return 0;
}

int (kbc_unsubscribe_int)(){
    return sys_irqrmpolicy(&k_hook_id);
}


void (kbc_ih)(){
  while(1){
    if (util_sys_inb(KBC_ST_REG, &data) != 0) {
      return;
    }
    
    if ((data & KBC_OBF) != 0) {
      if (util_sys_inb(KBC_OUT_BUF, &scancode) != 0) {
        printf("Could not read data\n");
        return;
      } 
      if ((data & KBC_PAR_ERR) != 0 ){ 
        printf("Parity error\n");
        return;
      }
      if ((data & KBC_TO_ERR) != 0 ){
        printf("Timeout error\n");
        return;
      }
      break;
    }
  }
}


void kbc_scancodes(){
  uint8_t bytes[2];
  bool scan = false;
  int size = 0;

  if(scancode == TWO_BYTES) {
    bytes[size] = scancode;
    size++;
    scan = true;
  }
    scan = !(scancode & KBD_BREAK);
    bytes[size] = scancode;
    kbd_print_scancode(scan, size+1, bytes);  
    size = 0;
}


int kbc_poll(){
  uint8_t cmdByte;
  
  sys_outb(KBC_CMD_REG, KBC_READ);
  util_sys_inb(KBC_OUT_BUF, &cmdByte);
  
  cmdByte |= BIT(0);

  sys_outb(KBC_CMD_REG, KBC_OUT_BUF);
  sys_outb(KBC_WRITE, cmdByte);

  return 0;
}

