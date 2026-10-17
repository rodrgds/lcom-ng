#include <lcom/lcf.h>
#include "mouse.h"
#include "i8042.h"

uint8_t data_mouse;
static int hook_mouse = 2;
bool wasRead_mouse = false;
int partPacket = 0;

int (mouse_subscribe_int)(uint8_t *bit_no){

  int idhook_back = hook_mouse;

  if(sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &hook_mouse) == OK){
    *bit_no = BIT(idhook_back);
    return 0;
  } else {
    return 1;
  }
}


int (mouse_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_mouse) == OK)
    return 0;
  else
    return 1;
}

void (mouse_ih)(){
  uint32_t stat;
  uint32_t tmpData;

  if (sys_inb(STAT_REG, &stat))
    return;

  if( stat & OBF ) {
    if (sys_inb(OUT_BUF, &tmpData))
      return;
    if ( (stat &(PAR_ERR | TO_ERR)) == 0 ){
      data_mouse = (uint8_t) tmpData;
      wasRead_mouse = true;
    } else {
      wasRead_mouse = false;
    }
  }
}

int write_cmd(uint32_t cmd){
  uint32_t stat;
  uint32_t tries = 0;

  while (true){
    if (sys_inb(STAT_REG, &stat))
      return 1;

    if( (stat & IBF)  == 0)
      if (sys_outb(KBC_CMD_REG, cmd) == 0)
        break;
      else
        return 1;
    else
      tries++;

    if (tries >= 5)
      return 1;

    tickdelay(micros_to_ticks(DELAY_US));
  }
  return 0;
}

int write_arg(uint32_t arg){
  uint32_t stat;
  uint32_t tries = 0;
  while (true){
    if (sys_inb(STAT_REG, &stat))
      return 1;

    if( (stat & IBF) == 0)
      if (sys_outb(IN_BUF, arg) == 0)
        break;
      else
        return 1;
    else
      tries++;

    if (tries >= 5)
      return 1;

    tickdelay(micros_to_ticks(DELAY_US));
  }
  return 0;
}

int write_cmd_and_arg(uint32_t cmd, uint32_t arg){

  if (write_cmd(cmd))
    return 1;

  return write_arg(arg);
}

int write_cmd_to_mouse(uint32_t mouse_cmd){
  uint32_t stat;
  uint32_t tmpData;
  uint32_t tries = 0;

  if (write_cmd_and_arg(MOUSE_COMMAND, mouse_cmd))
    return 1;

  while (true){
    if (sys_inb(STAT_REG, &stat))
      return 1;

    if( stat & OBF )
      if (sys_inb(OUT_BUF, &tmpData)){
        return 1;
      } else {
        data_mouse = tmpData;
        if (data_mouse == ACK || data_mouse == NACK || data_mouse == ERROR)
          return 0;
      }
    else
      tries++;

    if (tries >= 5)
      return 1;

    tickdelay(micros_to_ticks(DELAY_US));
  }
  return 0;
}

int write_cmd_mouse_and_check(uint32_t mouse_cmd){
  data_mouse = 0;
  while (data_mouse != ACK){
    write_cmd_to_mouse(mouse_cmd);
    if (data_mouse != ACK && data_mouse != NACK)
      return 1;
  }
  return 0;
}

bool interpreter_packet(struct packet* pak){
  if (!wasRead_mouse)
    return false;
  if (partPacket == 0){
    if ((CHECK_PACK_FIRST & data_mouse) == 0)
      return false;
    pak->lb = LB & data_mouse;
    pak->rb = RB & data_mouse;
    pak->mb = MB & data_mouse;
    pak->x_ov = X_OV & data_mouse;
    pak->y_ov = Y_OV & data_mouse;
  }
  pak->bytes[partPacket++] = data_mouse;
  wasRead_mouse = false;

  if (partPacket == 3){
    partPacket = 0;
    pak->delta_x = pak->bytes[1];
    pak->delta_y = pak->bytes[2];
    if (pak->bytes[0] & X_SIGN)
      pak->delta_x |= 0xFF00;
    if (pak->bytes[0] & Y_SIGN)
      pak->delta_y |= 0xFF00;
    return true;
  }
  return false;
}
