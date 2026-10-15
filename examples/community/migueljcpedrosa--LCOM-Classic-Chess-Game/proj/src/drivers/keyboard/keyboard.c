#include "keyboard.h"
#include "mkbd.h"
#include "../utils.h"

static int hook_id = 1;
uint8_t scan_code[2];
int kbd_index = 0;

bool last_byte_read = false;

int (keyboard_subscribe_int)(uint8_t* bit_no){

  hook_id = 1;

  *bit_no = hook_id;

  return sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int (keyboard_unsubscribe_int)(){

  return sys_irqrmpolicy(&hook_id);
}

void (kbc_ih)(){

  if (last_byte_read){
    kbd_index = 0;
    memset(scan_code, 0, 2);
  }

  uint8_t st;

  if (util_sys_inb(STATUS_PORT, &st))
    return;

  if (st & BIT(7) & BIT(6) & BIT(5))
    return;

  if ((st & BIT(0)) == 0)
    return;

  util_sys_inb(KBD_OUT_BUF, &scan_code[kbd_index]);

  if (scan_code[kbd_index] == 0xE0 && kbd_index == 0){
    kbd_index++;
    last_byte_read = false;
  }
  else{
    last_byte_read = true;
  }

  return;
}


