/**
 * @file mouse.c
 * @brief Implementation of i8042 mouse controller operations.
 *
 * This file contains the implementation of the functions declared in mouse.h.
 */

#include <lcom/lcf.h>
#include "mouse.h"
#include "../keyboard/keyboard.h"

int hook_id_mouse;
struct packet pack;
int num_byte = 0;
enum mouse_ev_t btn;

int (mouse_subscribe_int)(uint8_t *bit_no_mouse) {
  hook_id_mouse = *bit_no_mouse;
  if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &hook_id_mouse) == 0) {
    return 0;
  }
  printf("Error: Subscribing mouse failed.\n");
  return 1;
}

int (mouse_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id_mouse) == 0) {
    return 0;
  }
  printf("Error: UnSubscribing mouse failed.\n");
  return 1;
}

void(mouse_ih)() {
  uint8_t data;
  if (read_byte(&data)) { // Error reading
    num_byte = 0; // Reset state on error
    return;
  }

  if (num_byte >= 3) {
    num_byte = 0;
  }

  // Synchronization check: First byte must have BIT(3) set
  if (num_byte == 0 && !(data & BIT(3))) {
    printf("Mouse IH: Sync error, discarding byte\n");
    return; //Discard byte and wait for a valid first byte
  }

  // Store byte
  pack.bytes[num_byte] = data;

  if (num_byte == 0) {
    pack.mb = (data & BIT(2)); // Middle button
    pack.rb = (data & BIT(1)); // Right button
    pack.lb = (data & BIT(0)); // Left button
    pack.x_ov = (data & BIT(6)); // X overflow
    pack.y_ov = (data & BIT(7)); // Y overflow
  } else if (num_byte == 1) {
    if (pack.bytes[0] & BIT(4)) { 
      pack.delta_x = (int16_t)(data | 0xFF00); // Negative delta
    } else {
      pack.delta_x = (int16_t)data; // Positive delta
    }
  } else if (num_byte == 2) {
    if (pack.bytes[0] & BIT(5)) { 
      pack.delta_y = (int16_t)(data | 0xFF00); // Negative delta
    } else {
      pack.delta_y = (int16_t)data; // Positive delta
    }
  }
  num_byte++;
}

int(mouse_write_cmd)(uint8_t cmd) {
  int attempts = 5; // Retry attempts
  uint8_t ack;

  while (attempts > 0) {
    // Tell KBC we want to write to the mouse
    if (write_byte(KBC_WRITE_TO_MS, KBD_CMD_REG) != 0) {
      printf("Error: Failed to send 0xD4 command to KBC\n");
      return 1;
    }

    // Write the mouse command byte
    if (write_byte(cmd, KBC_OUT_BUF) != 0) { // Write command to KBC data port (forwarded to mouse)
      printf("Error: Failed to send command\n");
      return 1;
    }

    tickdelay(micros_to_ticks(DELAY_US)); 

    if (util_sys_inb(KBC_OUT_BUF, &ack) != 0) {
      printf("Error: Failed to read acknowledgment for command\n");
      return 1;
    }

    if (ack == MS_ACKNOWLEDGE) { 
      return 0;
    } else if (ack == MS_NACKNOWLEDGE) {
      printf("Info: Received NACK (0xFE) for command\n");
      attempts--;
      tickdelay(micros_to_ticks(DELAY_US));
      continue;
    } else if (ack == KBC_ERROR) { 
      printf("Error: Received ERROR (0xFC) for command\n");
      return 1;
    } else {
      printf("Warning: Received unexpected byte instead of ACK\n");
      attempts--;
      tickdelay(micros_to_ticks(DELAY_US));
      
      if (attempts == 0) return 1; 
      else continue; 
    }
  }
   printf("Error: Failed to get ACK for command\n");
  return 1; // Failed after retries
}

int(mouse_nable_data_reporting)() {
  if (mouse_write_cmd(MS_ENABLE_REPORTING) != 0) {
    printf("Error enabling data reporting.\n");
    return 1;
  }
  //printf("Mouse data reporting enabled.\n");
  return 0;
}


int(mouse_disable_data_reporting)() {
  if (mouse_write_cmd(MS_DISABLE_REPORTING) != 0) {
    printf("\x1b[0;31mError: disabling data reporting.\n\x1b[0m");
    return 1;
  }
  //printf("Mouse data reporting disabled.\n");
  return 0;
}

void(handle_buttons)(struct packet pack) {
  if (pack.lb && !pack.rb && !pack.mb) {
    btn = LB_PRESSED;
  } else if (!pack.lb && !pack.rb && !pack.mb && btn == LB_PRESSED) {
    btn = LB_RELEASED;
  } else if (!pack.lb && pack.rb && !pack.mb) {
    btn = RB_PRESSED;
  } else if (!pack.lb && !pack.rb && !pack.mb && btn == RB_PRESSED) {
    btn = RB_RELEASED;
  } else if (pack.delta_x != 0 || pack.delta_y != 0) {
    btn = MOUSE_MOV;
  }
}

struct packet get_mouse_packet() {
  return pack;
}
