#include "kbd.h"

int hook_id_keyboard = 0;

int keyboard_subscribe_interrupt(){
  hook_id_keyboard = KBD_KEY;
  if(sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_keyboard)){
    printf("Error while subscribing to keyboard's interrupts\n");
    return 1;
  }
  return 0;
}

int keyboard_unsubscribe_interrupt(){
  if(sys_irqrmpolicy(&hook_id_keyboard)){
    printf("Error while unsubscribing to keyboard's interrupts\n");
    return 1;
  }
  return 0;
}

void read_scancode(u8_t* ret){
  u8_t data;
  uint stat;

  int tries = 10;
  while(tries--) {
    // printf("Trying to read scancode\n");
    sys_inb(KBC_ST_REG, &stat); 
    if( stat & KBC_OBF ) {
      util_sys_inb(KBC_OUT_BUF, &data);
      if ( (stat &(KBC_PAR_ERR | KBC_TO_ERR)) == 0 )
        *ret = data;
      else
        *ret = 0;
      return;
      }
    tickdelay(2000);
  }
  *ret = 0;
}

char char_map[256] = {0x1, 0xf, 0xe, 0x2a, 0x1d, 0x38, 0x1c, 0x48, 0x4b, 0x4d, 0x50, 0, 
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                      0, 0, 0, 0, 0, 0, 0, 0, 0x39, 0, 0, 0, 
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                      0xb, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0, 0, 
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                      0, 0x1e, 0x30, 0x2e, 0x20, 0x12, 0x21, 0x22, 0x23, 0x17, 0x24, 0x25, 
                      0x26, 0x32, 0x31, 0x18, 0x19, 0x10, 0x13, 0x1f, 0x14, 0x16, 0x2f, 0x11, 
                      0x2d, 0x15, 0x2c, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                      0, 0, 0, 0 };
u8_t char_to_scancode(char c){
  u8_t uc = (u8_t) c;
  return char_map[uc];  
}
