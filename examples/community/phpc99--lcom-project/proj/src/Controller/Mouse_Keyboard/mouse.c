#include <lcom/lcf.h>

#include "mouse.h"
#include <stdint.h>

/**
 * @file mouse.c
 * @brief Implements mouse interrupt handling, packet parsing, KBC communication and gesture recognition.
 */

static int hook_id_mouse = 12;

mouse_reading_t mouse_reading = {.byte_index = 0};

STATES state = START;

bool waiting_mouse = true;

mouse_reading_t get_mouse_reading(){
  return mouse_reading;
}
int(read_Command_KBC)();

int(mouse_subscribe_int)(uint8_t *bit_no) {

  *bit_no = hook_id_mouse;

  if (sys_irqsetpolicy(MOUSE_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &hook_id_mouse) != 0)
    return 1;

  return 0;
}

int(mouse_unsubscribe_int)() {

  if (sys_irqrmpolicy(&hook_id_mouse) != 0)
    return 1;

  return 0;
}

void(mouse_ih)() {
  read_Command_KBC();
}

int(read_Command_KBC)() {

  uint8_t status;
  uint8_t data;
  for (int i = 0; i < 5; i++) {

    util_sys_inb(KBC_ST_REG, &status); /* assuming it returns OK */
    /* loop while 8042 output buffer is empty */
    if (status & KBC_OBF) {
      util_sys_inb(KBC_OUT_BUF, &data); /* ass. it returns OK */
      if ((status & (KBC_PAR_ERR | KBC_TO_ERR)) == 0) {
        mouse_reading.elem.bytes[mouse_reading.byte_index] = data;
        mouse_reading.byte_index++;
        if (mouse_reading.byte_index == 3) {
          mouse_reading.byte_index = 0;
          mouse_reading.elem.mb = mouse_reading.elem.bytes[mouse_reading.byte_index] & BIT(2);
          mouse_reading.elem.rb = mouse_reading.elem.bytes[mouse_reading.byte_index] & BIT(1);
          mouse_reading.elem.lb = mouse_reading.elem.bytes[mouse_reading.byte_index] & BIT(0);
          mouse_reading.elem.y_ov = BIT(7) & mouse_reading.elem.bytes[mouse_reading.byte_index];
          mouse_reading.elem.x_ov = BIT(6) & mouse_reading.elem.bytes[mouse_reading.byte_index];

          if (mouse_reading.elem.bytes[0] & BIT(5)) {
            mouse_reading.elem.delta_y = 0xFF00 | mouse_reading.elem.bytes[2];
          }
          else {
            mouse_reading.elem.delta_y = 0x0000 | mouse_reading.elem.bytes[2];
          }
          if (mouse_reading.elem.bytes[0] & BIT(4)) {
            mouse_reading.elem.delta_x = 0xFF00 | mouse_reading.elem.bytes[1];
          }
          else {
            mouse_reading.elem.delta_x = 0x0000 | mouse_reading.elem.bytes[1];
          }


          waiting_mouse = false;
        }
        return 0;
      }
      else {
      }
    }
    tickdelay(micros_to_ticks(DELAY_US)); // e.g. tickdelay()
  }
  return 1;
}

int(write_command_KBC)(uint8_t port,uint8_t command){
  uint8_t status;
  for(int i = 0;i < 10; i++){
    if(util_sys_inb(KBC_ST_REG, &status) != 0) return 1;

    if((status & BIT(1)) == 0){
      if(sys_outb(port, command) != 0) return 1;

      return 0;
    }
    tickdelay(micros_to_ticks(DELAY_US)); // e.g. tickdelay()
  }

  return 1;
}



int(mouse_data_reporting)(uint8_t cmd) {

  if(sys_irqdisable(&hook_id_mouse) != 0) return 1;

  uint8_t status;

  if(sys_outb(KBC_CMD_REG, 0xD4) != 0) return 1;
  for (int i = 0; i < 5; i++) {
    util_sys_inb(KBC_ST_REG, &status);
    
    if ((status & KBC_ST_IBF) == 0) {
      sys_outb(KBC_OUT_BUF, cmd); 

      uint8_t out;
      util_sys_inb(KBC_OUT_BUF, &out);

      if (out == ACK) {
        sys_irqenable(&hook_id_mouse);
        return 0;
      }
    }

    tickdelay(micros_to_ticks(DELAY_US)); 
  }
  sys_irqenable(&hook_id_mouse);
  return 1;
}
