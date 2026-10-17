#include <lcom/lcf.h>

#include <stdint.h>

#include "i8042.h"
#include "mouse.h"
#include "game.h"
#include "menu.h"

extern game_st game_state;

int globalHookIdMouse;
extern uint8_t byte, packet[3];
extern int byteCounter;
struct packet pp;
struct packet old_pp;
state_t state = INITIAL;
uint8_t global_tolerance = 100;
struct mouse_ev gesture;
 bool allowed_to_fire = false, protected= false;

int mouseX= 500,mouseY= 500;

int (mouse_subscribe)(uint8_t *bit_no){
	int temp_hook = TEMP_HOOK_MOUSE;

  	if(sys_irqsetpolicy(MOUSE_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &temp_hook) != OK)
      return 1;

  	globalHookIdMouse = temp_hook;
 
 	*bit_no = TEMP_HOOK_MOUSE;

	return 0;
}

int (mouse_unsubscribe)(){
  
  if(sys_irqrmpolicy(&globalHookIdMouse) != OK)
    return 1;

  return 0;
}

uint8_t (mouse_scan_byte)(){

	uint32_t stat, data;
	while( 1 ){
	 sys_inb(KB_STATUS_REG, &stat); /*cd2 assuming it returns OK */
        /* loop while 8042 output buffer is empty */
        sys_inb(OUT_BUF, &data);
        if( stat & OBF ) 
        	{/* assuming it returns OK */		
            	if ( (stat &(PAR_ERR | TO_ERR)) == 0 )
                	return data;
            	else
                	return -1;
        	}
      //  tickdelay(micros_to_ticks(DELAY_US));
	}
}



void (mouse_ih)(){
	byte = mouse_scan_byte();

	

	packet[byteCounter] = byte;

	if ((packet[0] & BIT(3)) == 0)
	{
		byteCounter = 0;
	}
	else{
		if (byteCounter >= 2) printPacket();
		else{
			byteCounter++;
		}
	}
}

void mouse_menu_ih(Mouse* mouse){
	byte = mouse_scan_byte();

	packet[byteCounter] = byte;

	if ((packet[0] & BIT(3)) == 0)
	{
		byteCounter = 0;
	}
	else{
		if (byteCounter >= 2) checkMenuPacket(mouse);
		else{
			byteCounter++;
		}
	}
}






void write_command(uint32_t cmd)
{
	uint32_t stat, data=0;

	while(data != ACK){
		sys_outb(KB_STATUS_REG, WRITE_TO_MOUSE);
		sys_inb(KB_STATUS_REG, &stat);
		if ((stat & IBF) == 0)
		{
			sys_outb(OUT_BUF,cmd);
			sys_inb(OUT_BUF,&data);
			return;
		}

		//tickdelay(micros_to_ticks(DELAY_US));
	}
}

void enable_cmd_int()
{
	//write_command(WRITE_TO_MOUSE);
	write_command(STREAM_MODE);
	//write_command(WRITE_TO_MOUSE);
	write_command(ENABLE_DR);

}

void disable_cmd_int()
{
	//write_command(WRITE_TO_MOUSE);
	write_command(DISABLE_MOUSE);
}



void printPacket(){
	
	for (int i = 0; i < 3; ++i)
	{
		pp.bytes[i] = packet[i];
	}
	old_pp = pp;
	pp.rb = (pp.bytes[0] & BIT(1)) >> 1;
     pp.mb = (pp.bytes[0] & BIT(2)) >> 2;
      pp.lb = (pp.bytes[0] & BIT(0));
      pp.x_ov = (pp.bytes[0] & BIT(6)) >> 6;
      pp.y_ov = (pp.bytes[0] & BIT(7)) >> 7;
      
      if(((packet[0] & BIT(4)) >> 4)==0)
        pp.delta_x = (packet[1] & 0x00ff);
      else
         pp.delta_x = (packet[1] | 0xff00);

      if(((packet[0] & BIT(5)) >> 5)==0)
         pp.delta_y = (packet[2] & 0x00ff);
      else
        pp.delta_y = (packet[2] | 0xff00);


if(pp.lb == 1)
{
	allowed_to_fire = true;
}
      //mouse_print_packet(&pp);
    	set_mouse_events();
    	check_line(gesture);


      clearPacket();
      byteCounter = 0;
      
}



void checkMenuPacket(Mouse* mouse){
	
	for (int i = 0; i < 3; ++i)
	{
		pp.bytes[i] = packet[i];
	}
	old_pp = pp;
	pp.rb = (pp.bytes[0] & BIT(1)) >> 1;
     pp.mb = (pp.bytes[0] & BIT(2)) >> 2;
      pp.lb = (pp.bytes[0] & BIT(0));
      pp.x_ov = (pp.bytes[0] & BIT(6)) >> 6;
      pp.y_ov = (pp.bytes[0] & BIT(7)) >> 7;
      
      if(((packet[0] & BIT(4)) >> 4)==0)
        pp.delta_x = (packet[1] & 0x00ff);
      else
         pp.delta_x = (packet[1] | 0xff00);

      if(((packet[0] & BIT(5)) >> 5)==0)
         pp.delta_y = (packet[2] & 0x00ff);
      else
        pp.delta_y = (packet[2] | 0xff00);


    if (pp.lb == 1)
    {
    	mouse->click = true;

    }
    else
    {
    	mouse->click = false;
    }

    	if (game_state == MAIN_MENU)
    	{
			updatePosition(mouse, pp.delta_x, pp.delta_y);
    	}

      clearPacket();
      byteCounter = 0;
      
}





void clearPacket(){
	for (int i = 0; i < 3; ++i)
	{
		packet[i] = 0;
	}
}




int (kbc_write_cmd)()
{
  uint32_t cmd=0;
  sys_outb(KB_STATUS_REG,KBC_READ);
  sys_inb(OUT_BUF,&cmd);

  cmd = cmd| KB_ENABLE;

sys_outb(KB_STATUS_REG,WRITE_BYTE);

sys_outb(OUT_BUF,minix_get_dflt_kbc_cmd_byte());

  return 0;
}






void check_line(struct mouse_ev evt) 
{
	 // initial state; keep state

	switch (state) 
	{
	case INITIAL:
		
		if(evt.type == LB_PRESSED)
		{
			gesture.delta_x = 0;
      		gesture.delta_y = 0;
			state = UP;
		}

		else if(evt.type == RB_PRESSED)
		{
			gesture.delta_x = 0;
      		gesture.delta_y = 0;
			state = RIGHT;
		}
		break;

	case UP:
		
		 if(evt.type == MOUSE_MOV)
		 {
		 	state = UP;
		 }

		else if(evt.type == LB_RELEASED)
		{
	 		state = COMP_UP;
		}
     	else 
     	{
     		state = INITIAL;
     	}
     	break;

	case RIGHT:
		
		if(evt.type == MOUSE_MOV)
		 {
		 	state = RIGHT;
		 }

		else if(evt.type == RB_RELEASED)
		{
	 		state = COMP_RIGHT;
	 		break;
		}

       else
       {
       	 state = INITIAL;
       }
       break;
	
	case COMP_UP:
		
		if(evt.type == MOUSE_MOV)
		{
			state = INITIAL;
			allowed_to_fire = true;
			
			break;	
		}
		break;

	
	case COMP_RIGHT:
		
		if(evt.type == MOUSE_MOV)
		{
			state = INITIAL;
			protected = true;
			
			break;	
		}
		break;

	default:
		break;
	}	


}

void set_mouse_events()
{
	if(pp.lb == 1 && pp.rb == 0 && pp.mb ==0) //left button pressed
	{
		if(pp.delta_y != 0 || (pp.delta_x != 0 && pp.delta_y != 0))
		{
			if(state == UP)
			{
				if (fire_tolerance())
				{
					gesture.delta_x += pp.delta_x;
      				gesture.delta_y += pp.delta_y;
					gesture.type = MOUSE_MOV;
				}
				else
				{
					state = INITIAL;
					return;
				}
			}
			else if (state == INITIAL)
				{
					gesture.type = LB_PRESSED;
				}
			else
			{
				gesture.type = BUTTON_EV;
			}
		}
		else if (state == INITIAL)
		{
			gesture.type = LB_PRESSED;
		}
		else
		{
			gesture.type = BUTTON_EV;
		}
	}
	else if(pp.lb == 0 && pp.rb == 0 && pp.mb == 0) // all buttons released
	{
		if(state == UP)
		{
			gesture.type = LB_RELEASED;
		}
		else if (state == RIGHT)
		{
			gesture.type = RB_RELEASED;
		}
		else
		{
			gesture.type = MOUSE_MOV;	
		}
	}

	else if(pp.lb == 0 && pp.rb == 1 && pp.mb ==0) //right button pressed
	{
		if(pp.delta_x != 0 || (pp.delta_y != 0 && pp.delta_x != 0))
		{
			if (state == RIGHT)
			{
				if(protect_tolerance())
				{
       				gesture.delta_x += pp.delta_x;
      				gesture.delta_y += pp.delta_y;
					gesture.type = MOUSE_MOV;
				}
				else
				{
					state = INITIAL;
					return;
				}
			}
			else if (state == INITIAL)
			{
				gesture.type = RB_PRESSED;
			}
			else
			{
				gesture.type = BUTTON_EV;
			}
		}
		else if (state == INITIAL)
		{
			gesture.type = RB_PRESSED;
		}
		else
		{
			gesture.type = BUTTON_EV;
		}		
	}
	else
	{
		gesture.type = BUTTON_EV;
	}	
}




bool fire_tolerance()
{
	if(pp.delta_x <= global_tolerance)
		{
		return true;
		}
		else
		{
			return false;
		}

}

bool protect_tolerance()
{
	if(pp.delta_y <= global_tolerance)
		{
		return true;
		}
		else
		{
			return false;
		}

}
