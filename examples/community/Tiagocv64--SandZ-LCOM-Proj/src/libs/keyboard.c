#include <lcom/lcf.h>
#include "keyboard.h"
#include "i8042.h"

uint32_t sysCount = 0;
uint8_t data_kb;
static int hook_kb = 1;
bool wasRead_kb = false;

int (keyboard_subscribe_int)(uint8_t *bit_no){

  int idhook_back = hook_kb;

  if(sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &hook_kb) == OK){
    *bit_no = BIT(idhook_back);
    return 0;
  } else {
    return 1;
  }
}


int (keyboard_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_kb) == OK)
    return 0;
  else
    return 1;
}

void (kbc_ih)(){
  uint32_t stat;
  uint32_t tmpData;

  if (sys_inb(STAT_REG, &stat))
    return;

  if( stat & OBF ) {
    if (sys_inb(OUT_BUF, &tmpData))
      return;
    if ( (stat &(PAR_ERR | TO_ERR)) == 0 ){
      data_kb = (uint8_t) tmpData;
      wasRead_kb = true;
    } else {
      wasRead_kb = false;
    }
  }
}
