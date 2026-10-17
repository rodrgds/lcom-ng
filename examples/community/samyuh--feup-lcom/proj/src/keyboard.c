#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include <stdint.h>

#include "keyboard.h"
#include "i8042.h"

int kbc_setHook = KBC_IRQ;
uint8_t scanCode[2];

extern bool check;

void (kbc_ih)(void)
{
  
  uint8_t outBuf = 0;
  uint8_t statusByte;

  
 for (int i = 0; i< 20; i++)
  {
      util_sys_inb(KBC_STAT_REG, &statusByte);

      
      if((statusByte & KBC_OBF))
      {
        /* assuming it returns OK */
        if((statusByte & (KBC_PARITY | KBC_TIMEOUT | KBC_AUX)) == 0)
        {
            util_sys_inb(KBC_OUT_BUF, &outBuf);

            if (check) scanCode[1] = outBuf;
            else scanCode[0] = outBuf;
            break;
        }
      }
}
}

bool (keyboard_makecode)(uint8_t byte){
    if (byte & BIT(7)) return false;
    else return true;
}

int (keyboard_subscribe_int)(uint8_t *bit_no) 
{

  *bit_no = kbc_setHook;
  if (sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbc_setHook)!= 0) return 1;
  return 0;
}

int (keyboard_unsubscribe_int)() 
{
  if(sys_irqdisable(&kbc_setHook) != 0) return 1;
  if(sys_irqrmpolicy(&kbc_setHook)!= 0) return 1;
  return 0;
}

int (keyboard_reenable)()
{
  uint8_t byte;

  if(sys_outb(KBC_STAT_REG, READ_COMMAND_BYTE)!= 0) return 1;
  if(util_sys_inb(KBC_OUT_BUF, &byte)!= 0) return 1;

  byte = (byte | BIT(0));
  
  if(sys_outb(KBC_STAT_REG, WRITE_COMMAND_BYTE)!= 0) return 1;
  if(sys_outb(KBC_IN_BUF, byte)!= 0) return 1;
  return 0;
}
