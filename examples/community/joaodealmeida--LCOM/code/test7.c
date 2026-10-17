#include "test7.h"
#include "uart.h"
#include <stdio.h>
#include <minix/bitmap.h>
#include <minix/drivers.h>

#include "Utilities.h"//debug



char wait_to_be_ready(unsigned short base_addr) {
    unsigned long lsr;
//do{
        uart_read(base_addr, UART_LSR_REG, &lsr);
        //tickdelay(2);
//}while(!bit_isset(lsr, LSR_TRANSMITTER_HOLDING_REGISTER_EMPTY_BIT));
        //returns 0 if can be read
    return !bit_isset(lsr, LSR_TRANSMITTER_HOLDING_REGISTER_EMPTY_BIT);
}



//unsigned char tx;
char startSign='S';
char endSign='.';
unsigned char bits=8;
unsigned char stop=1;
uart_parity_type parity=UART_NO_PARITY;
//unsigned char parity=0;///< parity
const unsigned long rate=9600;///<default rate to be used
char uart_startMode()
{
    uartConf oldConf, newConf;
    unsigned long oldIER, oldFCR;

    //UART_COM1_ADDR

    //Get Current Configuration
    uart_get_config(UART_COM1_ADDR, &oldConf);
    newConf = oldConf;

    DEBUG_MSG("DEBUG_0");

    newConf.bits = bits;
    newConf.stop = stop;
    newConf.bitRate = rate;
    newConf.parity = parity;

    DEBUG_MSG("DEBUG_1");

    //Get Current IER
    uart_read(UART_COM1_ADDR, UART_IER_REG, &oldIER);
    DEBUG_MSG("DEBUG_2");
    //Disable Interrupt Register
    uart_write(UART_COM1_ADDR, UART_IER_REG, 0);
    DEBUG_MSG("DEBUG_3");

    //Get Current FCR
    uart_read(UART_COM1_ADDR, UART_FCR_REG, &oldFCR);
    DEBUG_MSG("DEBUG_4");
    //Disable FIFO Register
    uart_write(UART_COM1_ADDR, UART_FCR_REG, oldFCR & ~FCR_ENABLE_FIFO_BIT);
    DEBUG_MSG("DEBUG_5");
    uart_set_config(UART_COM1_ADDR, newConf);

	return 0;
}

char uart_receive(unsigned char *str,unsigned int *numberOfBytesRead)
{
	unsigned long chr;
	//do {
		//if it's not ready leave and come back later
		if (wait_to_be_ready(UART_COM1_ADDR)!=OK) return 1;
		//wait_to_be_ready(UART_COM1_ADDR);

		uart_read(UART_COM1_ADDR, UART_RBR_REG, &chr);
		str[(*numberOfBytesRead)]= (char)chr;
		if ((*numberOfBytesRead)<30) (*numberOfBytesRead)+=1;
		LOG_CHAR("uart_rec_read:",(char)chr);

		//check if starting to read data (avoids confusion with a sign equal to end sign)
		if( (*numberOfBytesRead) == 1) if(str[0]!=startSign) return UART_FALSE_RECEIVE;

		//check if not receiving any info (reading last byte left on previous info "pack")
		//note: protocol does not allows 2 end signs
		//the largest value possible to receive = H_RES and is it's
		//value is lesser than the value of 2 end signs together
		if( (*numberOfBytesRead) >1)
			if(str[(*numberOfBytesRead)-1]==endSign&& str[(*numberOfBytesRead)-2]==endSign)
				return UART_FALSE_RECEIVE;

		//depracated due to protocol change //need to check end with 2 bytes because data may randomly have the same value as the end sign otherwise
	//} while(str[numberOfBytesRead-1]!='~'||str[numberOfBytesRead-2]!='~');
	//} while(
			//(char)chr!='.'||
			//(*numberOfBytesRead) <4);
		//(char)chr != '.');
	if ((char)chr==endSign && (*numberOfBytesRead) >1) return UART_RECEIVED_ALL_INFO;
	else return UART_RECEIVING_INFO;
}

char uart_send(unsigned int dataLenght, unsigned char* data)
{
	  while (wait_to_be_ready(UART_COM1_ADDR)!=OK) ;
	        uart_write(UART_COM1_ADDR, UART_RBR_REG, (unsigned long)'S');

	unsigned int i;
       for (i = 0; i < dataLenght; i++) {
      //      strLength = strlen(strings[i]);
      //      for (j = 0; j < strLength; j++) {
            	 while (wait_to_be_ready(UART_COM1_ADDR)!=OK)i; //wait_to_be_ready(UART_COM1_ADDR);
            	 DEBUG_SDEC(data[i]); LOG_SDEC("uart_send data_i:",data[i]);
                uart_write(UART_COM1_ADDR, UART_RBR_REG, (unsigned long) data[i]);
            }
           // wait_to_be_ready(base_addr);
          //  uart_write(base_addr, UART_RBR_REG, (unsigned long)' ');
        //}
       while (wait_to_be_ready(UART_COM1_ADDR)!=OK) ;
        uart_write(UART_COM1_ADDR, UART_RBR_REG, (unsigned long)'.');

        return 0;
}

