#include <lcom/lcf.h>
#include <i8042.h>

#include <stdbool.h>
#include <stdint.h>

#include "kbc.h"


uint8_t scancode = 0;

//Status code to get from the status register
uint8_t stat;

//Hook id to be used to set the interrupt policy
int kbc_hook_id = 1;

//Used to check if the interrupt handler fails or not
int ih_error = 0;


void(kbc_ih)() {
  
  //Reads the status from the keyboard
  util_sys_inb(STAT_REG, &stat);

  //Checks if there is a parity or timeout error and if the output buffer ins't empty
  if (((stat & (PAR_ERR | TO_ERR)) == 0) && (stat & OBF))
	  util_sys_inb(OUT_BUF, &scancode);
  else
	  ih_error = 1;
}

int(kbc_subscribe_int)(int *bit_no)
{
  if(bit_no == NULL)
    return 1;

  /*Subscribe the interruption using also the IRQ_EXCLUSIVE policy to prevent
    Minix's IH from the reading the output buffer*/
  if (sys_irqsetpolicy(KBC_IRQ,(IRQ_REENABLE | IRQ_EXCLUSIVE),&kbc_hook_id))
	return 1;
  
  return 0;
}

int (kbc_unsubscribe_int)() {
 
  //Unsubscribing the interruptions
  if(sys_irqrmpolicy(&kbc_hook_id))
    return 1;

  return 0;
}

int (kbc_poll)() {

    //Reading of the status of the keyboard
	util_sys_inb(STAT_REG, &stat);

	//Checks if there is a parity or timeout error and if AUX is cleared
	if ((stat & (PAR_ERR | TO_ERR | AUX)) == 0)
	{   
        //Check if the output buffer isn't empty
		if (stat & OBF)
		{   
            //If it isn't empty read the scancode
			util_sys_inb(OUT_BUF, &scancode);
			return 0;
		}
	}
	return 1;
}

//Used to issue a command cmd to the kbc
int (kbc_cmd)(uint8_t cmd) {

	int count = 0;
	while (count < MAX_TRIES)
	{
        //Reading of the status of the keyboard
		util_sys_inb(STAT_REG, &stat);

		/* loop while 8042 input buffer is full */
		if ((stat & IBF) == 0) {
            //Writing the command to the input buffer
			sys_outb(KBC_CMD_REG, cmd);
			return 0;
		}
		count++;
        //Wait for the keyboard
		tickdelay(micros_to_ticks(20000));
	}

	return 1;
}


int (kbc_write)(uint8_t data)
{
	int count = 0;
	while (count < MAX_TRIES)
	{
		util_sys_inb(STAT_REG, &stat);
		/* loop while 8042 input buffer is full */
		if ((stat & IBF) == 0) {
			//writing the data to the input buffer
			sys_outb(KBC_ARG_REG, data);
			return 0;
		}
		count++;
		tickdelay(micros_to_ticks(20000));
	}

	return 1;
}

int (kbc_read)(uint8_t *data){

    //Check if the data pointer is valid
    if(data == NULL)
        return 1;

	int count = 0;
	while (count < MAX_TRIES) {
		util_sys_inb(STAT_REG, &stat); /* assuming it returns OK */
		/* loop while 8042 output buffer is empty */
		if (stat & OBF) {
			 /* assuming it returns OK */
			if ((stat &(PAR_ERR | TO_ERR)) == 0)
			{
				util_sys_inb(OUT_BUF, data);
				return 0;
			}
		}
		count++;
		tickdelay(micros_to_ticks(20000));
	}
	return 1;
}

//Reenables the interruptions after the polling
int (kbc_reenable_int)(uint8_t *command){

    //Check if *command is a valid pointer
    if(command == NULL)
        return 1;

    //Requesting the command byte to be written to the Output Buffer
    if (kbc_cmd(READ_CMD))
		return 1;

    //Reading the command byte from the Output Buffer
	if(kbc_read(command))
		return 1;

    //Reenabling the keyboard interruptions
    *command |= BIT(0);

    //Programming the keyboard to receive the command back
	if(kbc_cmd(WRITE_CMD))
        return 1;

    //Writing the command to the input buffer
	if(kbc_write(*command))
        return 1;

    return 0;

}
