#include <lcom/lcf.h>
#include <stdint.h>

#include "kbd.h"
#include "i8254.h"

static int hook_id = KBD_IRQ;
uint8_t scancodes[2];
bool two_byte_code = false;

int (kbd_subscribe_int)(uint8_t *bit_no) {
  
  *bit_no = hook_id;
  if(sys_irqsetpolicy(KBD_IRQ,IRQ_REENABLE | IRQ_EXCLUSIVE ,&hook_id) != OK) return SET_POLICY_FAIL;
  
  return OK;
}

int (kbd_unsubscribe_int)() {

  if(sys_irqdisable(&hook_id) != OK) return DISABLE_FAIL;
  if(sys_irqrmpolicy(&hook_id) != OK) return RM_POLICY_FAIL;
  
  return OK;
}

int (kbd_reading_status)(uint8_t status,uint8_t bit_check){

  switch (bit_check)
  {
  case _PAR:
    if(status >> 7) return PARITY_ERROR;
    return NO_ERROR;
  case _TOUT:
    if((status >> 6) & 0x1) return TIMEOUT_ERROR;
    return NO_ERROR;
  case _OBF:
    if(status & 0x1) return BIT_CHECKED;
    return BIT_NOT_CHECKED;
  case _IBF:
    if((status >> 1) & 0x1) return FULL_NOT_WRITE;
    return CAN_WRITE;
  case _SYS:
    if((status >> 2) & 0x1) return INITIALIZED;
    return RESET_SYS;
  case _A2:
    if((status >> 3) & 0x1) return COMMAND_BYTE;
    return DATA_BYTE;
  case _INH:
    if((status >> 4) & 0x1) return INHIBITED;
    return BIT_NOT_CHECKED;
  case _AUX:
    if((status >> 5) & 0x1) return MOUSE_DATA;
    return NO_MOUSE_DATA;
  default:
    return BIT_NOT_CHECKED;
  }

  return NOT_OK;
}

void (kbc_ih)() {
  
  uint8_t status , scancode;

  while(1) {

    if(util_sys_inb(STAT_REG,&status) != OK) break;
    
    if(kbd_reading_status(status,_OBF) == BIT_CHECKED && kbd_reading_status(status,_AUX == NO_MOUSE_DATA)) {

      if(util_sys_inb(OUT_BUF,&scancode) != OK) break;
      
      if(kbd_reading_status(status,_PAR) == NO_ERROR && kbd_reading_status(status,_TOUT) == NO_ERROR){

        if (two_byte_code) {
            scancodes[1] = scancode;
            two_byte_code = false;
        }
        else if(scancode == TWO_BYTES_CODE) {
            scancodes[0] = scancode;
            two_byte_code = true;
        }
        else {
            scancodes[0] = scancode;
        }
        break;

      }
      else
        break;
            
    }
    break;
  }
}

bool (kbd_make_code)(uint8_t scancode){
    if (scancode & BIT(7)) return false;
    else return true;
}

int (kbd_write_commands)(int reg, uint8_t cmd){
  uint8_t status = 0;
  while( 1 ) {
    if(util_sys_inb(STAT_REG, &status) != OK) return NOT_OK;

    if(kbd_reading_status(status,_IBF) ==  CAN_WRITE) {
      if(sys_outb(reg, cmd) != OK) return NOT_OK; 
      return OK;
    }
    tickdelay(micros_to_ticks(DELAY));
  }
}

int (kbd_reenable)(){
  uint8_t command_byte;

  if(kbd_write_commands(KBC_CMD_REG, READ_CMD) != OK) return NOT_OK;

  if (util_sys_inb(OUT_BUF, &command_byte) != OK) return NOT_OK;
  
  command_byte |= BIT(0);

  if(kbd_write_commands(KBC_CMD_REG,WRITE_CMD) != OK) return NOT_OK;

  if(kbd_write_commands(OUT_BUF, command_byte) != OK) return NOT_OK;

  return OK;

}
