#include "mouse.h"

int hook_id;

int mouse_subscribe_interrupt(){
  hook_id = MOUSE_KEY;
  if(sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id))
    return 1;

  return 0;
}

int mouse_unsubscribe_interrupt(){
  if(sys_irqrmpolicy(&hook_id))
    return 1;
  return 0;
}

int parse_to_packet(u8_t byte[3], struct packet* pp){
  pp->bytes[0] = byte[0];
  pp->bytes[1] = byte[1];
  pp->bytes[2] = byte[2];

  pp->lb = byte[0] & BIT(0);
  pp->rb = byte[0] & BIT(1);
  pp->mb = byte[0] & BIT(2);
  pp->x_ov = byte[0] & BIT(6);
  pp->y_ov = byte[0] & BIT(7);

  pp->delta_x = byte[1];
  pp->delta_y = byte[2];

  if(byte[0] & BIT(4))
    pp->delta_x |= 0xFF00;
  if(byte[0] & BIT(5))
    pp->delta_y |= 0xFF00;

  return 0;
}

int mouse_enable_data_reporting_ours(){
  uint stat;
  u8_t kbc_cmd = 0xD4;
  u8_t mouse_cmd = 0xF4;
  int safe_counter = 0;
  u8_t code;

  while( 1 ) {
    sys_inb(KBC_ST_REG, &stat); /* assuming it returns OK */
    /* loop while 8042 input buffer is not empty */
    if( (stat & KBC_ST_IBF) == 0 ) {
      sys_outb(KBC_CMD_REG, kbc_cmd); /* no args command */
      sys_outb(KBC_OUT_BUF, mouse_cmd);
      read_scancode(&code);
      if(code == 0xFA){
        // printf("mouse disabled\n");
        return 0;
      }
    }
    tickdelay(micros_to_ticks(200)); // e.g. tickdelay()

    safe_counter++;
    if(safe_counter > 10)
      return 1;
  }
  return 0;
}

int mouse_disable_data_reporting(){
  uint stat;
  u8_t kbc_cmd = 0xD4;
  u8_t mouse_cmd = 0xF5;
  int safe_counter = 0;
  u8_t code;

  while( 1 ) {
    sys_inb(KBC_ST_REG, &stat); /* assuming it returns OK */
    /* loop while 8042 input buffer is not empty */
    if( (stat & KBC_ST_IBF) == 0 ) {
      sys_outb(KBC_CMD_REG, kbc_cmd); /* no args command */
      sys_outb(KBC_OUT_BUF, mouse_cmd);
      read_scancode(&code);
      if(code == 0xFA){
        // printf("mouse disabled\n");
        return 0;
      }
    }
    tickdelay(micros_to_ticks(200)); // e.g. tickdelay()

    safe_counter++;
    if(safe_counter > 10)
      return 1;
  }
  return 0;
}


