#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "i8042.h"
#include "mouse.h"

int hook_id_mouse = 0;
uint32_t total_packets = 0;
bool error_reading = false;
int counter_mouse = 0;
uint8_t packet;
bool do_sync = false;
struct packet packet0;
int x_mouse = 0;
int y_mouse = 0;
extern vbe_mode_info_t vmi_p;
/*writes an arguments to the mouse*/
int(write_argument_to_mouse)(uint8_t argument) {
  // 8 bit number that uses the output buffer to check if the argument written was valid
  uint8_t validate;
  // 8 bit number that stores the status of the status register
  uint8_t status;

  // while no break
  while (true) {
    // read status from status register
    if (util_sys_inb(STATUS_REGISTER, &status) != 0) {
      return 1;
    }

    // if status has the second bit == to 1, we can write to the buffer
    if ((status & BIT(1)) == 0) {
      // write the command to the buffer command
      if (sys_outb(INPUT_BUFFER_COMMANDS, COMMAND_MOUSE) != 0) {
        return 1;
      }
      break;
    }
  }

  // while no break
  while (true) {
    // read status from the status register
    if (util_sys_inb(STATUS_REGISTER, &status) != 0) {
      return 1;
    }

    // if status has the second bit == to 1, we can write to the buffer
    if ((status & BIT(1)) == 0) {
      // write argument to the buffer arguments
      if (sys_outb(INPUT_BUFFER_ARGUMENTS, argument) != 0) {
        return 1;
      }

      // read if the argument was correctly written
      if (util_sys_inb(OUTPUT_BUFFER, &validate) != 0) {
        return 1;
      }

      // if 0xFA, it means it was correctly written
      if (validate == 0xFA) {
        return 0;
      }
    }
  }
}

// interrupt handler
void(mouse_ih)() {
  uint8_t stat;

  while (true) {
    if (util_sys_inb(STATUS_REGISTER, &stat) != 0) {
      printf("Error, could not read status\n");
      break;
    }

    if (stat & OBF) {
      if (util_sys_inb(OUTPUT_BUFFER, &packet) != 0) {
        printf("Error, could not read scancode\n");
        break;
      }

      if ((stat & (TIMEOUT | PARITY)) == 0) {
        if (error_reading) {
          counter_mouse++;
        }
        return;
      }
      else {
        printf("Error, parity or timeout error detected");
        break;
      }
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }

  // if error return true
  error_reading = true;
  // and increment counter
  counter_mouse++;
}

// subscribe to mouse
int mouse_subscribe_int() {

  hook_id_mouse = IRQ2;

  // SET POlICY IN IRQ12 EXCLUSIVELY
  if (sys_irqsetpolicy(IRQ12, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse) != 0) {
    return 1;
  }
  return 0;
}

int mouse_unsubscribe_int() {
  if (sys_irqrmpolicy(&hook_id_mouse) != 0) {
    return 1;
  }
  return 0;
}

// print mouse packets
void print_mouse_packet() {
  // if mouse if out of sync
  if (do_sync == false) {

    if ((packet & 0x08) == 0) {
      printf("Out of sync");
      return;
    }
    else
      do_sync = true;
  }

  if (counter_mouse == 0) {
    packet0.bytes[0] = packet;
    counter_mouse++;
    return;
  }

  if (counter_mouse == 1) {
    packet0.bytes[1] = packet;
    counter_mouse++;
    return;
  }

  if (counter_mouse == 2) {
    packet0.bytes[2] = packet;
    counter_mouse = 0;

    if ((packet0.bytes[0] & 0x80) != 0)
      packet0.y_ov = true;
    else
      packet0.y_ov = false;
    if ((packet0.bytes[0] & 0x40) != 0)
      packet0.x_ov = true;
    else
      packet0.x_ov = false;
    if ((packet0.bytes[0] & 0x20) != 0) {
      packet0.delta_y = packet0.bytes[2] - 256;
    }
    else
      packet0.delta_y = packet0.bytes[2];
    if ((packet0.bytes[0] & 0x10) != 0) {
      packet0.delta_x = packet0.bytes[1] - 256;
    }
    else
      packet0.delta_x = packet0.bytes[1];
    if ((packet0.bytes[0] & 0x04) != 0)
      packet0.mb = true;
    else
      packet0.mb = false;
    if ((packet0.bytes[0] & 0x02) != 0)
      packet0.rb = true;
    else
      packet0.rb = false;
    if ((packet0.bytes[0] & 0x01) != 0)
      packet0.lb = true;
    else
      packet0.lb = false;
    total_packets++;
  }

  mouse_print_packet(&packet0);
}

// enable the mouse
int(enable_mouse)() {
  // put it in stream mode
  if (write_argument_to_mouse(STREAM_MODE)) {
    return 1;
  }
  // enable mouse
  if (write_argument_to_mouse(ENABLE_MOUSE)) {
    return 1;
  }

  return 0;
}

int(disable_mouse)() {
  if (write_argument_to_mouse(DISABLE_MOUSE) != 0) {
    return 1;
  }
  if (write_argument_to_mouse(STREAM_MODE) != 0) {
    return 1;
  }

  return 0;
}

void get_mouse_coordinates() {
  // if mouse if out of sync
  if (do_sync == false) {

    if ((packet & 0x08) == 0) {
      printf("Out of sync");
      return;
    }
    else
      do_sync = true;
  }

  if (counter_mouse == 0) {
    packet0.bytes[0] = packet;
    counter_mouse++;
    return;
  }

  if (counter_mouse == 1) {
    packet0.bytes[1] = packet;
    counter_mouse++;
    return;
  }

  if (counter_mouse == 2) {
    packet0.bytes[2] = packet;
    counter_mouse = 0;

    if ((packet0.bytes[0] & 0x80) != 0)
      packet0.y_ov = true;
    else
      packet0.y_ov = false;
    if ((packet0.bytes[0] & 0x40) != 0)
      packet0.x_ov = true;
    else
      packet0.x_ov = false;
    if ((packet0.bytes[0] & 0x20) != 0) {
      packet0.delta_y = packet0.bytes[2] - 256;
    }
    else
      packet0.delta_y = packet0.bytes[2];
    if ((packet0.bytes[0] & 0x10) != 0) {
      packet0.delta_x = packet0.bytes[1] - 256;
    }
    else
      packet0.delta_x = packet0.bytes[1];
    if ((packet0.bytes[0] & 0x04) != 0)
      packet0.mb = true;
    else
      packet0.mb = false;
    if ((packet0.bytes[0] & 0x02) != 0)
      packet0.rb = true;
    else
      packet0.rb = false;
    if ((packet0.bytes[0] & 0x01) != 0)
      packet0.lb = true;
    else
      packet0.lb = false;
    counter_mouse = 0;
  }
}


void move_cursor(){

    if (packet0.x_ov || packet0.y_ov){
      return;
    }

    x_mouse += packet0.delta_x;
    y_mouse -= packet0.delta_y;

    
    if(x_mouse < 0){
      x_mouse = 5;
    }
    if (y_mouse < 0){
      y_mouse = 5;
    }
    if(x_mouse > vmi_p.XResolution - 16){
      x_mouse = vmi_p.XResolution - 16-5;
    }
    if (y_mouse > vmi_p.YResolution - 16){
      y_mouse = vmi_p.YResolution - 16-5;
    }

    packet0.delta_x = 0;
    packet0.delta_y = 0;

}
