#include <lcom/lcf.h>
#include "i8042.h"
#include "keyboard.h"

/** @defgroup keyboard keyboard
 * @{
 *
 * Contais tha code to control the keyboard controller
 */

///Keyboard data for the case the scancode is 2 byte long
uint8_t data2byte[2];

///If the code is 2 byte long
bool is_2_byte = false;

///Break code
uint8_t data;

///Keyboard hook id
int kbd_hook_id = 1;

///If the scancode recived is new or not
bool new_scancode = false;

#ifdef LAB3
int sys_inb_count = 0;
#endif

///Subscribe for keyboard interupts
int (keyboard_subscribe_int)(int *irq_set) {
    if(irq_set == NULL)
        return 1;

    *irq_set = BIT(kbd_hook_id);
    return sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE , &kbd_hook_id);
}


///Unsubscribe from keyboard interupts
int (keyboard_unsubscribe_int)() {
    return sys_irqrmpolicy(&kbd_hook_id);
}


///Keyboard interupt handler
void (keyboard_IH)()
{
  uint8_t stat;
  
  for(int i = 0; i < TRIES; i++) 
  {

    new_scancode = false;

    if(util_sys_inb(STAT_REG, &stat))
    	continue;
    
      if( stat & OBF) /*loop while 8042 output buffer is empty*/
      {

        if ((stat & (PAR_ERR | TO_ERR | MOUSE)) == 0 )
        {
          if(!util_sys_inb(KBC_OUT_BUFF, &data))
          {
                    new_scancode = true;
                    break;
          }
        }
        else
        	continue;
      }

      tickdelay(micros_to_ticks(DELAY_US));
    }
}


///Reads from the out buffer
int (keyboard_read_out_buff)(uint8_t *output)
{
  uint8_t stat;
  
  for(int i = 0; i < TRIES; i++) 
  {

    new_scancode = false;

    if(util_sys_inb(STAT_REG, &stat))
      continue;
    
      if( stat & OBF) /*loop while 8042 output buffer is empty*/
      {

        if ((stat & (PAR_ERR | TO_ERR | MOUSE)) == 0 )
        {
          if(!util_sys_inb(KBC_OUT_BUFF, output))
          {
            new_scancode = true;
            return 0;
          }
        }
        else
          continue;
      }

      tickdelay(micros_to_ticks(DELAY_US));
    }

    return 1;
}


///Prints scancode
int (print_scancode)()
{
	if(data == 0xe0)
	{
		data2byte[0] = data;
		is_2_byte = true;
		return 0;
	}
	if(is_2_byte)
	{
		is_2_byte = false;
		data2byte[1] = data;
		if(data & BIT(7))
			return kbd_print_scancode(false, 2, data2byte);

		return kbd_print_scancode(true, 2, data2byte);
	}
	if(data & BIT(7))
		return kbd_print_scancode(false, 1, &data);

	return kbd_print_scancode(true, 1, &data);
}


///Write comand to register
int (keyboard_write_command)(uint8_t reg, uint32_t cmd)
{
  uint8_t stat;

  for (int i = 0; i < TRIES; ++i)
  {
    if(util_sys_inb(STAT_REG,&stat))
    {
  
      continue;
    }
    if((stat & IBF) == 0)
    {
      if(sys_outb(reg,cmd) == 0)
       { 
        return 0;
        }
      continue;
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }
  return 1;
}


///Enable keyboard interupts
int (keyboard_enable_interupts)()
{
  uint8_t cmd_mask = 0;
  int ret = keyboard_write_command(KBC_CMD_REGISTER,KBC_READ_COMBYTE);
  ret = ret || keyboard_read_out_buff(&cmd_mask);
  cmd_mask = cmd_mask | KBC_CB_INT;
  ret = ret || keyboard_write_command(KBC_CMD_REGISTER, KBC_WRITE_COMBYTE);
  ret = ret || keyboard_write_command(KBC_IN_BUFF, cmd_mask);
  return ret;
}
