#include <lcom/lcf.h>

#include <stdint.h>

#include "i8042.h"
#include "keyboard.h"



int globalHookId, counter = 0;

int (kb_subscribe)(uint8_t *bit_no){
	int temp_hook = TEMP_HOOK_KB;

  	if(sys_irqsetpolicy(KB_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &temp_hook) != OK)
      return 1;

  	globalHookId = temp_hook;
 

 	  *bit_no = TEMP_HOOK_KB;

	return 0;
}

int (kb_unsubscribe)(){
  
  if(sys_irqrmpolicy(&globalHookId) != OK)
    return 1;

  return 0;
}

uint8_t (kb_scan_byte)(bool poll){

	uint32_t stat, data;
	while( 1 ){
	 sys_inb(KB_STATUS_REG, &stat); /* assuming it returns OK */
        /* loop while 8042 output buffer is empty */
        if( stat & OBF ) 
        	{
            sys_inb(OUT_BUF, &data);
            if(!poll) 
            {/* assuming it returns OK */
            if ( (stat &(PAR_ERR | TO_ERR)) == 0 )
                return data;
            else
                return -1;
        	}
        	else{
 			 if ( (stat &(PAR_ERR | TO_ERR | AUX)) == 0 )
                return data;
            else
                return -1;

        	}
        }
        tickdelay(micros_to_ticks(DELAY_US));
		}

}


int (kbc_reenable)(){
    uint32_t cmd=0;
  
    sys_outb(KB_STATUS_REG,KBC_READ);

    sys_inb(OUT_BUF,&cmd);

  cmd = cmd| KB_ENABLE;

  sys_outb(KB_STATUS_REG,OUT_BUF);

  sys_outb(OUT_BUF,cmd);

  return 0;
}

void (kbc_ih)(){
  byte = kb_scan_byte(false);

  if (byte == ESC_BREAK)
  {
      is_over = true;
      make = false;
      return;
  }
 if (byte == TWO_BYTE_SCAN)
 {
    size = 2;
    return;
  }
  else{
  if (byte & 0x80)
  {
      make = false;
      return;
   }
   else{
    make = true;
   }
 }
 
   return;
}

int (kb_read_poll)(){

  uint8_t byte, tam;
  bool make = true,goOn=true;

  while(goOn)
    
    {
      byte = kb_scan_byte(true);
      if (byte == ESC_BREAK)
      {
        make = false;
        goOn=false;
        uint8_t scancode[1];
        scancode [0] = byte;
        tam = 1;
        //*size = 1;
        kbd_print_scancode(make, tam, scancode);
        return 1;
      }
      if (byte == TWO_BYTE_SCAN)
      {
       uint8_t scancode[2];
       scancode [0] = byte;
       scancode[1]  = kb_scan_byte(true);
       tam = 2;
       kbd_print_scancode(make, tam, scancode);
      
      }


      if (byte & 0x80)
      {
      make = false;
      uint8_t scancode[1];
      scancode [0] = byte;
      tam = 1;
      //*size = 1;
      kbd_print_scancode(make, tam, scancode);
  	  }
      else{
        uint8_t scancode[1];
        make = true;
        scancode[0] = byte;
        tam = 1;
        kbd_print_scancode(make, tam, scancode);
      }

    }

    return 0;
}

