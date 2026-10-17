#include <lcom/lcf.h>
#include "KBC.h"

//Reads KBC's STAT_REG to get the status byte
int (kbc_get_status)(uint8_t *stat) {
    if (stat == NULL) return 1;

    if (util_sys_inb(KBC_STAT_REG, stat) != 0) return 1;

    return 0;
}


//Reads the output buffer register
int (read_OBF)(uint8_t *op) {
  return util_sys_inb(KBC_OUT_BUF, op);

}



//Checks for existing errors on the control register status
int (check_errors)(uint8_t status, bool from_mouse) {

  if((status & PAR_ERROR_BIT) != 0) {
    printf("Parity error \n");
    return 1;
  }

  if((status & TIME_ERROR_BIT) != 0) {
    printf("TImeout error \n");
    return 1;
  }
  //Checks if the data is coming from the keyboard or not
  if(from_mouse) {
     if(!(status & MOUSE_DATA_BIT) != 0) {
     printf("Data not from the correct device \n");
     printf("Expected data from mouse\n");
     return 1;
     }
  }
  else {
    if((status & MOUSE_DATA_BIT)!= 0) {
    printf("Data not from the correct device \n");
    printf("Expected data from keyboard\n");
    return 1;
    }
  }
  return 0;


}



//Reads OUT_BUF when full, verifies data validity and if it comes from the wanted device. Parameter from_mouse should be true when we want to get data from Mouse. It should be false when we want to get data from Keyboard or get the return of a KBC Command
int (kbc_get_out)(uint8_t *out, bool from_mouse) {

    int tries = LIMIT_TRIES;
    uint8_t status;


    while(tries != 0) {         //Limited amount of tries, to be robust to errors
        if (kbc_get_status(&status) != 0) return 1;  //Read KBC status byte

        if ((status & OBF_BIT) != 0)  {      //If OUT_BUF is full, can read it
            if (util_sys_inb(KBC_OUT_BUF, out) != 0) return 1;

            if (check_errors(status, from_mouse) != 0) return 1;
            return 0;
        }
       //If OUT_BUF not full, wait some time
            tickdelay(micros_to_ticks(DELAY_US));
            tries--;
    }
    return 1;   //Surpassed limit of tries, there might have been an error
   
}





//Writes to KBC's input buffers (port 0x64 or 0x60) when not full
int (kbc_write_input)(int port, uint8_t commandOrArg) {

    int tries = LIMIT_TRIES;
    uint8_t status;

    while (tries != 0) {      //Limited number of tries, to be robust to errors

        if (kbc_get_status(&status) !=0) return 1;   //Read status byte

        if ((status & IBF_BIT) == 0) {     //If input buffer not full, can write to it. If full, wait some time.
            if (sys_outb(port, commandOrArg) != 0) return 1;  
            
            return 0;
        }
        
        tickdelay(micros_to_ticks(DELAY_US));
        tries--;
        
    }
    return 1;  //Surpassed limit of tries

}




//Sends KBC command to write Command Byte and writes one
int (kbc_configure)(uint8_t commandByte) {
    if (kbc_write_input(KBC_CTRL_REG, WRITE_COMMAND_BYTE) != 0) return 1;  //Write KBC Command 0x60 to port 0x64

    if (kbc_write_input(KBC_IN_BUF, commandByte) != 0) return 1;    //Write KBC Command argument to port 0x60

    return 0;
}




//Sends KBC command to read Command Byte and reads it from OUT_BUF
int (kbc_get_conf)(uint8_t *commandByte) {
    if (commandByte == NULL) return 1;

    if (kbc_write_input(KBC_CTRL_REG, READ_COMMAND_BYTE) != 0) return 1;

    if (kbc_get_out(commandByte, false) != 0) return 1;


    return 0;

}




