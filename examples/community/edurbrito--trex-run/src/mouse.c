// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "mouse.h"
#include "i8254.h"
#include "kbd.h"

static int hook_id = MOUSE_IRQ;
static uint8_t pack[3];
unsigned int byteCounter = 0;

bool insync = false;

int(mouse_subscribe_int)(uint8_t *bit_no){
 
  *bit_no = hook_id;
  if(sys_irqsetpolicy(MOUSE_IRQ,IRQ_REENABLE | IRQ_EXCLUSIVE ,&hook_id) != OK) return SET_POLICY_FAIL;
  
  for(int i = 0; i < 20; i++){
    uint8_t status,temp;
    if(util_sys_inb(STAT_REG,&status) != OK) return NOT_OK;

    if(kbd_reading_status(status,_OBF) == BIT_CHECKED){
      if(util_sys_inb(OUT_BUF,&temp) != OK) return NOT_OK;
      }
    else{
      break;
    }
    
  }
  
  return OK;
}

int(mouse_unsubscribe_int)(){

  if(sys_irqrmpolicy(&hook_id) != OK) return RM_POLICY_FAIL;

  uint8_t status,temp;
  if(util_sys_inb(STAT_REG,&status) != OK) return NOT_OK;

  if(kbd_reading_status(status,_OBF) == BIT_CHECKED)
    if(util_sys_inb(OUT_BUF,&temp) != OK) return NOT_OK;

  return OK;
}

int mouse_data_reporting(bool en) {
      
  if(en) {if(mouse_write_commands(ENABLE_REP) != OK) return NOT_OK;}  
  else {if(mouse_write_commands(DISABLE_REP) != OK) return NOT_OK;}
    
  return OK;
}

uint16_t rev2comp(uint8_t num, struct packet * p, uint8_t bit) {
  uint16_t extended = num;
  if (BIT(bit) & p->bytes[0]) { // negative number
      extended -= 256;
      return extended;
    }
    else // positive number
      return extended;
}

void packet_assemble(struct packet * p) {
    p->bytes[0] = pack[0];
    p->bytes[1] = pack[1];
    p->bytes[2] = pack[2];
    p->lb = BIT(0) & p->bytes[0];
    p->rb = BIT(1) & p->bytes[0];
    p->mb = BIT(2) & p->bytes[0];
    p->x_ov = BIT(6) & p->bytes[0];
    p->y_ov = BIT(7) & p->bytes[0];
    p->delta_x = rev2comp(p->bytes[1],p,4);
    p->delta_y = rev2comp(p->bytes[2],p,5);
}

void (mouse_ih) () {

    uint8_t byte;

    if(util_sys_inb(OUT_BUF,&byte) != OK ) return;
    if(byteCounter % 3 == 0 && byte & BIT(3))
      insync = true;
        
    if(insync) 
      pack[byteCounter % 3] = byte;
    
    byteCounter++;
}

int mouse_write_commands(uint8_t cmd){

  do{
    
    if(kbd_write_commands(KBC_CMD_REG,WRITE_MOUSE) != OK) return NOT_OK;
    if(kbd_write_commands(OUT_BUF, cmd) != OK) return NOT_OK;

    uint8_t ack = 0;
   
    if (util_sys_inb(OUT_BUF,&ack) != OK) return NOT_OK; 

    if (ack == ACK) { 
      break;
    }
    else if(ack == ERROR){
      return NOT_OK;
    }

  }while(1);

  return OK;
}
