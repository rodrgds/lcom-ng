#ifndef __TEST7_H__
#define __TEST7_H__

//#define UART_RECEIVE
//#define UART_SEND

//RETURN RESULTS FOR RECEIVE FUNC
#define UART_RECEIVED_ALL_INFO 0
#define UART_FALSE_RECEIVE 2
#define UART_RECEIVING_INFO 3

int test_conf(unsigned short base_addr);

int test_set(unsigned short base_addr, unsigned long bits, unsigned long stop,
	     long parity, /* -1: none, 0: even, 1: odd */
	     unsigned long rate);

int test_poll(unsigned short base_addr, unsigned char tx, unsigned long bits, unsigned long stop, long parity, unsigned long rate, int stringc, char *data);

char uart_startMode();
/**@brief receive data
 * @return 0 if all data received, 3 if still reading, 2 if not receiving anything
 */
char uart_receive(unsigned char *str,unsigned int *numberOfBytesRead);
char uart_send(unsigned int dataLenght, unsigned char* data);

#endif
