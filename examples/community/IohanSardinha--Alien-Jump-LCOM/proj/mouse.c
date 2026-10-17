#include <lcom/lcf.h>
#include "i8042.h"
#include "mouse.h"

/** @defgroup mouse mouse
 * @{
 *
 * Contais tha code to controll the mouse controller
 */

int mouse_hook_id = 2;
uint8_t data;
uint8_t data3bytes[3];
int delta_x = 0;
int delta_y = 0;
state_t st = INIT;

int (mouse_subscribe_int)(int *irq_set)
{
	if(irq_set == NULL)
		return 0;
	*irq_set = BIT(mouse_hook_id);
	return sys_irqsetpolicy(MOUSE_IRQ,IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

int (mouse_unsubscribe_int)() {
    return sys_irqrmpolicy(&mouse_hook_id);
}

int (setMouseIRQ)(bool enable)
{
	return enable ? sys_irqenable(&mouse_hook_id) : sys_irqdisable(&mouse_hook_id);
}

int (mouse_write_command)(uint8_t cmd)
{
	uint8_t stat;
	uint8_t ack_byte = 0;
	while(ack_byte != ACK)
	{
		if(util_sys_inb(KBC_CMD_REGISTER,&stat))
			return 1;

		if(stat & IBF)
			continue;

		if(sys_outb(KBC_CMD_REGISTER,WRITE_BYTE_TO_MOUSE))
			return 1;

		if(util_sys_inb(KBC_CMD_REGISTER,&stat))
			return 1;

		if(stat & IBF)
			continue;

		if(sys_outb(KBC_OUT_BUFF,cmd))
			return 1;

		if(util_sys_inb(KBC_CMD_REGISTER,&stat))
			return 1;

		if(stat & (OBF | MOUSE))
		{
			if(util_sys_inb(KBC_OUT_BUFF,&ack_byte))
				return 1;

			if (ack_byte == MOUSE_CNTRL_ERROR)
				return 1;
		}
	}
	return 0;
}

void(mouse_ih)()
{
	mouse_read_out_buff();
}

void (mouse_read_out_buff)()
{
  uint8_t stat;
  
  for(int i = 0; i < TRIES; i++) 
  {
    if(util_sys_inb(STAT_REG, &stat))
    	continue;
    
      if( stat & (OBF | MOUSE))
      {

        if ((stat & (PAR_ERR | TO_ERR)) == 0 )
        {
          if(!util_sys_inb(KBC_OUT_BUFF, &data))
          {
          	 break;
          }
          
        }
        else
        	continue;
      }

      tickdelay(micros_to_ticks(DELAY_US));
    }
}

 void (state_machine)(struct packet *pckt, uint8_t x_len, uint8_t tolerance)
 {
	delta_x += pckt->delta_x;
	delta_y += pckt->delta_y; 	
 	switch(st)
 	{
 		case INIT:
 		{
 			if(pckt->lb && !pckt->mb && !pckt->rb)
 				st = DRAWUP;
 			break;
 		}
 		case DRAWUP:
 		{
 			int slope = delta_y == 0 ?  0 : delta_y/delta_x;
 			if(pckt->delta_x < 0 && abs(pckt->delta_x) > tolerance)
 			{
 				st = INIT;
 				delta_x = 0;
 				delta_y = 0;
 				break;
 			}
 			if(pckt->delta_y < 0 && abs(pckt->delta_y) > tolerance)
 			{
 				st = INIT;
 				delta_x = 0;
 				delta_y = 0;
 				break;
 			}	
 			if(slope<1)
 			{
 				st = INIT;
 				delta_x = 0;
 				delta_y = 0;
 				break;
 			}
 			if(!pckt->lb && !pckt->mb && !pckt->rb && delta_x >= x_len)
 			{
 				st = VERTEX;
 				delta_x = 0;
 				delta_y = 0;
 				break;
 			}
 			if(!pckt->lb && !pckt->mb && !pckt->rb)
 			{
 				st = INIT;
 				delta_x = 0;
 				delta_y = 0;
 			}
 			break;
 		}
 		case VERTEX:
 		{
 			if(abs(delta_x) > tolerance || abs(delta_y) > tolerance)
 			{
 				st = INIT;
 				delta_x = 0;
 				delta_y = 0;
 				break;
 			}
 			if(pckt->lb || pckt->mb)
 			{
 				delta_x = 0;
 				delta_y = 0;
 				st = INIT;
 				break;
 			}
 			if(!pckt->lb && !pckt->mb && pckt->rb)
 			{
 				st = DRAWDOWN;
 				delta_x = 0;
 				delta_y = 0;
 			}
 			break;
 		}
 		case DRAWDOWN:
 		{
 			int slope = delta_y == 0 ?  0 : delta_y/delta_x;
 			if(pckt->delta_x < 0 && abs(pckt->delta_x) > tolerance)
 			{
 				st = INIT;
 				delta_x = 0;
 				delta_y = 0;
 				break;
 			}
 			if(pckt->delta_y > 0 && abs(pckt->delta_y) > tolerance)
 			{
 				st = INIT;
 				delta_x = 0;
 				delta_y = 0;
 				break;
 			}	
 			if(slope>-1)
 			{
 				st = INIT;
 				delta_x = 0;
 				delta_y = 0;
 				break;
 			}
 			if(!pckt->lb && !pckt->mb && !pckt->rb && delta_x >= x_len)
 			{
 				st = CMP;
 				delta_x = 0;
 				delta_y = 0;
 				break;
 			}
 			if(!pckt->lb && !pckt->mb && !pckt->rb)
 			{
 				delta_x = 0;
 				delta_y = 0;
 				st = INIT;
 			}
 			break;
 		}
 		case CMP:
 		{
 			break;
 		}
 	}
 }

 int (createMousePacket)(struct packet *pckt, uint8_t data3byte[])
 {
 	pckt->bytes[0] = data3byte[0];
 	pckt->bytes[1] = data3byte[1];
 	pckt->bytes[2] = data3byte[2];

 	pckt->rb = (data3byte[0] & RIGHT_BUTTON);
  	pckt->mb = (data3byte[0] & MIDDLE_BUTTON);
  	pckt->lb = (data3byte[0] & LEFT_BUTTON);

  	pckt->delta_x = (data3byte[0] & X_SIGN) ? (data3byte[1] | (0xff << 8)) : data3byte[1];
  	pckt->delta_y = (data3byte[0] & Y_SIGN) ? (data3byte[2] | (0xff << 8)) : data3byte[2];

  	if(pckt->delta_x >= 250 || pckt->delta_x <= -250) pckt->delta_x = 0;
  	if(pckt->delta_y >= 250 || pckt->delta_y <= -250) pckt->delta_y = 0;
  	
  	pckt->x_ov = (data3byte[0] & X_OVERFLOW);
  	pckt->y_ov = (data3byte[0] & Y_OVERFLOW);

 	return 0;
 }

 int (set_kbc_cmd_byte_dflt)()
 {

 	uint8_t cmb = minix_get_dflt_kbc_cmd_byte();
 	uint8_t stat;
 	for(int i = 0; i < TRIES; i++)
 	{
 		if(util_sys_inb(KBC_CMD_REGISTER,&stat))
 			return 1;

 		if(stat & IBF)
 			continue;

 		if(sys_outb(KBC_CMD_REGISTER,KBC_WRITE_COMBYTE))
 			return 1;

 		if(util_sys_inb(KBC_CMD_REGISTER,&stat))
 			return 1;

 		if(stat & IBF)
 			continue;

 		if(sys_outb(KBC_OUT_BUFF,(uint32_t) cmb))
 			return 1;

 		return 0;	
 	}
 	return 1;
 }
