#include <lcom/lcf.h>

#include "i8042.h"
#include "keyboard.h"
#include "mouse.h"

#define DELAY_US 20000
int hook_mouse_id = 5;

int (mouse_subscribe_int)(uint8_t *bit_no) {
  *bit_no = (uint8_t) hook_mouse_id;
  if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_mouse_id))
    return 1;
  return 0;
}

int (mouse_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_mouse_id))
    return 1;
  return 0;
}

int counter_mouse_ih = 0;

void(mouse_ih)(void) {
  uint8_t status = 0;
  if (counter_mouse_ih >= 3)
    counter_mouse_ih = 0;

  util_sys_inb(KBC_ST_REG, &status);

  if (status & (TIME_ERROR | PAR_ERROR))
    return;
  if (!(status & AUX_MOUSE))
    return;
  if (!(status & OBF))
    return;

  uint8_t byte = 0;
  util_sys_inb(KBC_OUT_BUF, &byte);
  if ((byte & BIT(3)) || counter_mouse_ih) {
    packet_mouse_ih[counter_mouse_ih++] = byte;
  }
  else{
    counter_mouse_ih = 0;
  }
}

void (mouse_get_packet)(const uint8_t *bytes, struct packet *pp) {

  pp->bytes[0] = bytes[0];
  pp->bytes[1] = bytes[1];
  pp->bytes[2] = bytes[2];

  pp->x_ov = (pp->bytes[0] & BIT(6)) >> 6;
  pp->y_ov = (pp->bytes[0] & BIT(7)) >> 7;

  pp->lb = pp->bytes[0] & BIT(0);
  pp->mb = (pp->bytes[0] & BIT(2)) >> 2;
  pp->rb = (pp->bytes[0] & BIT(1)) >> 1;

  int16_t XDelta = pp->bytes[1];
  int16_t YDelta = pp->bytes[2];

  if ((pp->bytes[0] & BIT(4)) != 0)
    XDelta = XDelta - 256;
  if ((pp->bytes[0] & BIT(5)) != 0)
    YDelta = YDelta - 256;

  pp->delta_x = XDelta;
  pp->delta_y = YDelta;
  return;
}

int (mouse_disable_data)(){
    
    if(mouse_issue_command(0xF5) != 0) return 1;
    return 0;
}

int (mouse_enable_data)(){
    
    if(mouse_issue_command(0xF4) != 0) return 1;
    return 0;


}


int (mouse_issue_command)(uint8_t cmd){
    uint8_t status;
    uint8_t acknow;

    while(1){
        util_sys_inb(KBC_ST_REG,&status);
        if((status & IBF) == 0){
            if(sys_outb(KBC_CMD_REG,0xD4) != 0) return 1;
            break;
        }
    }
    
    while(1){
        util_sys_inb(KBC_ST_REG,&status);
        if((status & IBF) == 0) {
            if(sys_outb(KBC_ARG_REG,cmd) != 0) return 1;

            util_sys_inb(KBC_OUT_BUF,&acknow);
            
            if(acknow == 0xFA) return 0;
            else if (acknow == 0xFE){
                tickdelay(micros_to_ticks(DELAY_US));
            }
            else if(acknow == 0xFC) return 1;

            else tickdelay(micros_to_ticks(DELAY_US));
        }
    }
    return 1;

}


