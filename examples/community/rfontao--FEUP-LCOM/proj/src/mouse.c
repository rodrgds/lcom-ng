#include <lcom/lcf.h>

#include "kbc.h"
#include "mouse.h"
#include "vbe.h"
#include "game.h"

uint32_t mouse_x = 0;
uint32_t mouse_y = 0;

extern uint16_t hres; /*XResolution*/
extern uint16_t vres; /*YResolution*/

extern int cameraX;
extern int cameraY;


extern uint8_t scancode;

//Status code to get from the status register
extern uint8_t stat; //Not sure if needed

//Hook id to be used to set the interrupt policy
int mouse_hook_id = 12;

//Used to check if the interrupt handler fails or not
int mouse_ih_error;



void(mouse_ih)() {

  //Reads the status from the keyboard
  util_sys_inb(STAT_REG, &stat);

  //Checks if there is a parity or timeout error and if the output buffer ins't empty
  if (((stat & (PAR_ERR | TO_ERR)) == 0) && (stat & OBF)) {
    util_sys_inb(OUT_BUF, &scancode);
    mouse_ih_error = 0;
  }
  else
    mouse_ih_error = 1;
}

int(mouse_subscribe_int)(int *bit_no) {
  if (bit_no == NULL)
    return 1;

  /*Subscribe the interruption using also the IRQ_EXCLUSIVE policy to prevent
	Minix's IH from the reading the output buffer*/
  if (sys_irqsetpolicy(MOUSE_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &mouse_hook_id))
    return 1;

  return 0;
}

int(mouse_unsubscribe_int)() {

  //Unsubscribing the interruptions
  if (sys_irqrmpolicy(&mouse_hook_id))
    return 1;

  return 0;
}

int(mouse_send_cmd)(uint8_t cmd) {

  uint8_t ack;

  do {

    if (kbc_cmd(MOUSE_CMD))
      return 1;

    if (kbc_write(cmd))
      return 1;

    if (kbc_read(&ack))
      return 1;

    if (ack == MOUSE_ACK_ERROR)
      return 1;

  } while (ack == MOUSE_NACK);

  return 0;
}

int(mouse_remote)() {

  //Reading of the status of the keyboard
  util_sys_inb(STAT_REG, &stat);

  //Checks if there is a parity or timeout error and if AUX is cleared
  if ((stat & (PAR_ERR | TO_ERR)) == 0) {
    //Check if the output buffer isn't empty
    if (stat & OBF) {
      //If it isn't empty read the scancode
      util_sys_inb(OUT_BUF, &scancode);
      return 0;
    }
  }
  return 1;
}


void updateMousePosition(int deltax, int deltay){

	int mx = (int)mouse_x;
	int my = (int)mouse_y;
	/*if(((mx + deltax) < XRES) && ((mx + deltax) >= 0)){
		mx += deltax;
	}
	else
	{
		if((mx + deltax) >= hres)
		{
			mx = hres-1;  
		}
		else if((mx + deltax)<0)
		{
			mx = 0;
		}
	}

	if(((my - deltay) < YRES) && ((my - deltay) >= 0)){
		my -= deltay;
	}
	else
	{
		if((my - deltay) >= vres)
		{
			my = vres - 1;  
		}
		else if((my - deltay)<0)
		{
			my = 0;
		}
	}
	*/
	if(((mx + deltax) < (cameraX + hres) && ((mx + deltax) >= cameraX))){
		mx += deltax;
	}
	else
	{
		if((mx+deltax) >= (cameraX + hres))
		{
			mx = cameraX + hres - 1;
		}
		else if((mx + deltax) < cameraX)
		{
			mx = cameraX;
		}
	}

	if(((my - deltay) < (cameraY + vres)) && ((my - deltay) >= cameraY)){
		my -= deltay;
	}
	else
	{
		if((my - deltay) >= (cameraY + vres))
		{
			my = cameraY + vres - 1;  
		}
		else if((my - deltay) < cameraY)
		{
			my = cameraY;
		}
	}


	mouse_x = (uint32_t)mx;
	mouse_y = (uint32_t)my;

}
