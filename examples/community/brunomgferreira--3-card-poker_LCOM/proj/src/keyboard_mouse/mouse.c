#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>
#include "mouse.h"

#include "i8042.h"
#include "kbc.h"

static int mouse_hook_id = MOUSE_HOOK_BIT;

struct packet mPacket;
struct mouse_position mPosition = {100, 100};
int byteIndex = 0;
uint8_t bytes[3];
uint8_t byte;

enum mouse_states current_state = INITIAL;

int(mouse_subscribe_int)(uint8_t *bit_no)
{
  if (bit_no == NULL)
    return 1;

  *bit_no = BIT(mouse_hook_id);

  // call int sys_irqsetpolicy(int irq_line, int policy, int *hook_id);
  if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id))
    return 1;

  return 0;
}

int(mouse_unsubscribe_int)()
{
  // call int sys_irqrmpolicy(int *hook_id);
  if (sys_irqrmpolicy(&mouse_hook_id))
    return 1;

  return 0;
}

void(mouse_ih)()
{
  if (kbc_read_output(KBC_IN_BUF, &byte, 1))
    printf("mouse_ih: ERROR \n");

  // process_packet_byte();
  // if (byteIndex == 3) byteIndex = 0;
}

void(process_packet_byte)()
{
  if ((byteIndex == 0 && byte & BIT(3)) || (byteIndex > 0))
  {
    mPacket.bytes[byteIndex] = byte;
    bytes[byteIndex] = byte;
    byteIndex++;
  }

  if (byteIndex == 3)
  {
    mPacket.lb = bytes[0] & MOUSE_LB;
    mPacket.mb = bytes[0] & MOUSE_MB;
    mPacket.rb = bytes[0] & MOUSE_RB;
    mPacket.x_ov = bytes[0] & MOUSE_XOVFL;
    mPacket.y_ov = bytes[0] & MOUSE_YOVFL;
    mPacket.delta_x = (bytes[0] & MOUSE_MSBX) ? (0xff00 | bytes[1]) : bytes[1];
    mPacket.delta_y = (bytes[0] & MOUSE_MSBY) ? (0xff00 | bytes[2]) : bytes[2];
  }
}

int(mouse_write)(uint8_t command)
{
  uint8_t tries = TRIES;
  uint8_t res;

  do
  {
    if (kbc_write_command(KBC_CTRL_REG, WRITE_BYTE_TO_MOUSE))
      return 1;

    if (kbc_write_command(KBC_OUT_BUF, command))
      return 1;

    tickdelay(micros_to_ticks(DELAY));

    if (util_sys_inb(KBC_OUT_BUF, &res))
      return 1;

    if (res == ACK)
      return 0;

    tries--;
  } while (res != ACK && tries);

  return 1;
}

int(mouse_gesture)(uint8_t x_len, uint8_t tolerance) {
  static int16_t total_x_mov = 0;
  static int16_t total_y_mov = 0;

  int16_t delta_x = mPacket.delta_x;
  int16_t delta_y = mPacket.delta_y;

  switch (current_state)
  {
  case INITIAL:

    if (mPacket.lb && !mPacket.rb && !mPacket.mb)
      current_state = FIRST_LINE;
    break;

  case FIRST_LINE:

    if (!mPacket.lb) {
      if (abs(delta_x) <= tolerance && 
          abs(delta_y) <= tolerance && 
          total_y_mov / total_x_mov > 1 && 
          total_x_mov >= x_len) {
        current_state = VERTEX;
      }
      else {
        current_state = INITIAL;
      }

      total_x_mov = 0;
      total_y_mov = 0;
    }
    else if (mPacket.mb || mPacket.rb) {
      current_state = INITIAL;
      total_x_mov = 0;
      total_y_mov = 0;
    }
    else if ((delta_x < 0 || delta_y < 0) && 
            (abs(delta_x) > tolerance || 
            abs(delta_y) > tolerance)) {
      current_state = INITIAL;
      total_x_mov = 0;
      total_y_mov = 0;
    }
    else {
      total_x_mov += delta_x;
      total_y_mov += delta_y;
    }
    break;

  case VERTEX:

    if ((abs(delta_x) > tolerance || abs(delta_y) > tolerance))
    {
      current_state = INITIAL;
    }
    else if (!mPacket.lb && !mPacket.rb && !mPacket.mb)
    {
      total_x_mov += delta_x;
      total_y_mov += delta_y;
    }
    else if (!mPacket.lb && mPacket.rb && !mPacket.mb)
    {
      if (delta_x <= tolerance)
      {
        current_state = SECOND_LINE;
      }
      else
      {
        current_state = INITIAL;
      }
      total_x_mov = 0;
      total_y_mov = 0;
    }
    break;

  case SECOND_LINE:

    if (!mPacket.rb)
    {
      if ((abs(delta_x) <= tolerance &&
          abs(delta_y) <= (tolerance)) &&
          abs(total_y_mov / total_x_mov) > 1 &&
          total_x_mov >= x_len) {
        current_state = COMPLETE;
        return 0;
      }
      else
      {
        current_state = INITIAL;
      }
      total_x_mov = 0;
      total_y_mov = 0;
    }
    else if (mPacket.lb || mPacket.mb)
    {
      current_state = INITIAL;
      total_x_mov = 0;
      total_y_mov = 0;
    }
    else if ((delta_x < 0 || delta_y > 0) && 
            (abs(delta_x) > tolerance || 
            abs(delta_y) > tolerance)) {
      current_state = INITIAL;
      total_x_mov = 0;
      total_y_mov = 0;
    }
    else
    {
      total_x_mov += delta_x;
      total_y_mov += delta_y;
    }
    break;

  case COMPLETE:
    current_state = COMPLETE;
    break;
  }

  return 0;
}

void (update_mouse_position)() {
  // The screen width, and height are static (currently corresponds to the mode 0x14C)
  int width = 1152, height = 864;
  if (mPosition.x + mPacket.delta_x < 0 && !mPacket.x_ov) mPosition.x = 0;
  else if (mPosition.x + mPacket.delta_x > width && !mPacket.x_ov) mPosition.x = width;
  else if (!mPacket.x_ov) mPosition.x += mPacket.delta_x;

  if (mPosition.y - mPacket.delta_y < 0 && !mPacket.y_ov) mPosition.y = 0;
  else if (mPosition.y - mPacket.delta_y > height && !mPacket.y_ov) mPosition.y = height;
  else if (!mPacket.y_ov) mPosition.y -= mPacket.delta_y;
}
