#include <lcom/lcf.h>
#include "kbd.h"

static int hook_id = 1;
static uint8_t scancode = 0;

static uint8_t scancode_array[2]; 
static uint8_t scancode_index = 0;

uint8_t* get_scancode(){
  return &scancode;
}

uint8_t* get_scancode_array(){
  return scancode_array;
}

uint8_t get_scancode_index(){
  return scancode_index;
}

int (kbd_subscribe_int)(uint8_t *bit_no){
  *bit_no = hook_id;
  hook_id = KBD_IRQ;
  if(sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != 0) {
    printf("Failed to subscribe keyboard interrupts.\n");
    return 1;
  }
  return 0;
}

int (kbd_unsubscribe_int)() {
  if(sys_irqrmpolicy(&hook_id) != 0) {
    printf("Failed to unsubscribe keyboard interrupts.\n");
    return 1;
  }
  return 0;
}

void (kbc_ih)(void){
  kbc_read_data(KBC_OUT_BUF, &scancode,0);
}


int (check_scancode_complete)(){
  
  if (scancode == TWO_BYTE_CODE) {
    scancode_array[scancode_index] = scancode;
    scancode_index++;
    return 1;
  }else{
    scancode_array[scancode_index] = scancode;
    scancode_index=0;
    return 0;
  } 
}

int (kbd_enable_interrupts)(){
  uint8_t cmd;

  if(kbc_write_command(KBC_CMD_REG, KBC_READ_CMD) != 0){
    return 1;
  }

  if(kbc_read_data(KBC_OUT_BUF, &cmd, 0) != 0){
    return 1;
  }

  cmd = cmd | BIT(0);

  if(kbc_write_command(KBC_CMD_REG, KBC_WRITE_CMD) != 0){
    return 1;
  }
  
  if(kbc_write_command(KBC_OUT_BUF, cmd) != 0){
    return 1;
  }

  return 0;
}


