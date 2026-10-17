#include "keyboard.h"

uint8_t scancode = 0,size = 1,bytes[2];
static bool make = true;
bool complete_scancode = true;
static int hook_id = 0;

int (kbd_subscribe_int)(uint8_t *bit_no)
{
  *bit_no = IRQ1;
  hook_id = IRQ1;
  if(sys_irqsetpolicy(IRQ1, IRQ_REENABLE | IRQ_EXCLUSIVE,&hook_id))
  {
    return 1;
  }
  return 0;
}


int (kbd_unsubscribe_int)()
{
  if(sys_irqrmpolicy(&hook_id))
  {
    return 1;
  }
  return 0;
}




void (kbc_ih)()
{
  util_sys_inb(OUT_BUFF,&scancode);
}





int (kbc_get_scancode)()
{
  uint8_t status = 0;
  //getting status
  if (util_sys_inb(STATUS_REG,&status))
  {
    return 1;
  }
  //if output buffer is empty 
  if(!(status & OBUFFER_FULL))
  {
    return 1;
  }

  //reading scancode
  kbc_ih();



  //if the scancode is valid
  if(status & (PARITY | TIMEOUT))
  {
    return 1;
  }
  return 0;
}

void (kbc_verify_scancode)()
{
  //verifying if its a 2 bytes or 1 bytes
  if (scancode == TWO_BYTES)
  {
    bytes[0] = scancode;
    complete_scancode = false;
  }
  else
  {
    //if its a break code
    if (scancode & MAKE_BREAK)
    {
      make = false;
    }
    else
    {
      make = true;
    }
    //if its a 2bytes scancode
    if (!(complete_scancode))
    {
      size = 2;
      bytes[1] = scancode;
      complete_scancode = true;
    }
    else
    {
      size = 1;
      bytes[0] = scancode;
    }
  }
}
