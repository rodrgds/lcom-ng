#include <lcom/lcf.h>
#include <stdint.h>

#include "../inc/i8042.h"
#include "../inc/mouse.h"

uint8_t mouse_index = 0;
uint8_t current_byte;
uint8_t mouse_bytes[3];

int mouse_id = 2;
uint8_t mouse_data;

int (mouse_subscribe_int)(uint8_t *bit_no){
if(bit_no==NULL)return 1;
  
  *bit_no = BIT(mouse_id);

  if(sys_irqsetpolicy(MOUSE_IRQ,(IRQ_REENABLE | IRQ_EXCLUSIVE),&mouse_id)!=0)return 1; 

  return 0;
}

int (mouse_unsubscribe_int)(){
  if(sys_irqrmpolicy(&mouse_id)!=0) return 1;
  return 0;
}

void (mouse_ih)(){
  
  uint8_t st;
  util_sys_inb(CMD_REG,&st);

  if(st & BIT(7))return;

  if(st & BIT(6))return;

  if((st & BIT(0))&&(st & BIT(5)))
    util_sys_inb(OUT_BUF, &mouse_data);
    
}

int (update_mouse)(int16_t delta_x, int16_t delta_y, bool rb, bool mouse_int) {

  int direction = 0;

  if (rb && !mouse_int) direction = 5;
  else if (!rb && !mouse_int){
    int tolerance = 13;
    if (abs(delta_x) <= tolerance) delta_x = 0;
    if (abs(delta_y) <= tolerance) delta_y = 0;

    int moveX = delta_x; 
    int moveY = delta_y;
    

    if (abs(moveX) > abs(moveY))
    {
        if (moveX > 0){direction = 1; }
        else{ direction = 2; }

    }
    else if(abs(moveX) < abs(moveY))
    { 
        if (moveY > 0) {direction = 4;}
        else {direction = 3; }
    }
  }
    return direction;
}


int mouse_disable_stream(){
    uint8_t response = 0x00;
    
    uint8_t status;

    while(response != 0xFA)
    {
        if(util_sys_inb(STAT_IN, &status) != 0) return 1;
        if(sys_outb(CMD_REG, 0xD4) != 0) return 1;
        if((status & BIT(1)) == 0) {if(sys_outb(WRITE_CMD, 0xF5)!=0) return 1;}
        if(util_sys_inb(OUT_BUF, &response) != 0) return 1;
    }
    
    return 0;
}
