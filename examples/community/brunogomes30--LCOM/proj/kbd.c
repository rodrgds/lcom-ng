#include "kbd.h"
#include "i8042.h"
#include <lcom/lcf.h>
//void kbc_ih(){}

static int kbd_hook_id;
uint8_t arr[2];
int nBytes = 1;
void kbd_delay() {
  tickdelay(micros_to_ticks(20000));
  
}

int(kbd_subscribe_int)(uint8_t *bit_no) {
  kbd_hook_id = *bit_no;
  return sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id);
}

int(kbd_unsubscribe_int)() {

  return sys_irqrmpolicy(&kbd_hook_id);
}

uint32_t kbd_read() {
  int timeout = 1;
  uint32_t status;
  uint32_t kbd_code = 0;
  while (timeout > 0) {

    if (sys_inb(0x64, &status) != OK) {
      printf("keyboard status error");
      return 0xFFFF;
    }
    if (status & OUT_BUF_STATUS) {
      if (sys_inb(0x60, &kbd_code) != OK) {
        printf("keyboard error reading");
        return 0xFFFF;
      }

      if ((status & (PARITY_ERROR | TIME_OUT_ERROR)) == 0) {
        return kbd_code;
      }
      else {
        printf("ERROR KBD_READ();");
        return 0xFFFF;
      }
    }
    timeout--;
  }
  return 0;
}

uint32_t(kbd_read_code)() {
  uint32_t kbd_code = kbd_read();
  if ((kbd_code & 0x000000FF) == 0x000000E0) {
    nBytes = 2;
    //First byte
    arr[0] = 0xE0;
    arr[1] = kbd_code;
    return 0xE0;
  }
  else if (nBytes == 2) {
    arr[1] = kbd_code;
  }
  else {
    arr[0] = kbd_code;
    nBytes = 1;
  }
  if (kbd_code == 0xFFFF) {
    return 0xFFFF;
  }
  return kbd_code;
}

int (read_cmd_byte)(uint32_t *cmd){
  uint32_t st;
  int i, j;
  for(i = 0; i < 5; i++){
    sys_inb(STATUS_REG, &st);
    if((st & BIT(1)) == 0){
      sys_outb(STATUS_REG, 0x20);
      for(j = 0; j < 5;j++){
        sys_inb(STATUS_REG, &st);
        if((st & BIT(1)) == 0){
          sys_inb(0x60, cmd);
          break;
        }
      }
      sys_inb(0x60, cmd);
      break;
    }
    kbd_delay();
  }
  return i != 5 || j != 5;
}

int (write_cmd_byte)(uint32_t *cmd){
  uint32_t st;
  int i, j;
  for(i = 0; i < 5; i++){
    sys_inb(STATUS_REG, &st);
    if((st & BIT(1)) == 0){
      sys_outb(STATUS_REG, 0x60);
      for(j = 0; j < 5;j++){
        sys_inb(STATUS_REG, &st);
        if((st & BIT(1)) == 0){
          sys_outb(0x60, *cmd);
          break;
        }
        kbd_delay();  
      }
      break;
    }
    kbd_delay();
  }
  return i != 5 || j != 5;
}

uint8_t is_break_code(uint32_t keycode){
  return (keycode & BIT(7)) >> 7;
}

