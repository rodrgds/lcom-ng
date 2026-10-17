#include "uart.h"

#include <stdio.h>

#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/bitmap.h>



int uart_read(unsigned long port_addr, unsigned long offset, unsigned long* buffer){

	if(sys_inb(port_addr + offset, buffer) != OK ){
		printf("uart_read: Failed to read");
		return -1;
	}

	return 0;
}

int uart_write(unsigned long port_addr, unsigned long offset, unsigned long buffer){

	if(sys_outb(port_addr + offset, buffer) != OK ){
		printf("uart_write: Failed to write");
		return -1;
	}

	return 0;
}

int uart_get_divisor_latch(unsigned long port, ushort_t* divisor_latch) {
	unsigned long buffer;


    if (uart_set_dlab(port) != 0) {
        return -1;
    }

    if (uart_read(port, UART_DL_LSB, &buffer) != 0) {
        return -1;
    }

    divisor_latch->b.lsb = (unsigned char) buffer;

    if (uart_read(port, UART_DL_MSB, &buffer) != 0) {
        return -1;
    }

    divisor_latch->b.msb = (unsigned char) buffer;

    if (uart_unset_dlab(port) != 0) {
        return -1;
    }

    return 0;
}

int uart_set_divisor_latch(unsigned long port, ushort_t divisor_latch) {
    unsigned long buffer;

    if (uart_set_dlab(port) != 0) {
        return -1;
    }

    buffer = (unsigned long)divisor_latch.b.lsb;

    if (uart_write(port, UART_DL_LSB, buffer) != OK) {
        return -1;
    }

    buffer = (unsigned long)divisor_latch.b.msb;

    if (uart_write(port, UART_DL_MSB, buffer) != OK) {
        return -1;
    }

    if (uart_unset_dlab(port) != OK) {
        return -1;
    }

    return 0;
}

int uart_set_dlab(unsigned long port) {
    unsigned long lcr;

    if (uart_read(port, UART_LCR_REG, &lcr) != 0) {
        return 1;
    }

    bit_set(lcr, LCR_DLAB_BIT);

    if (uart_write(port, UART_LCR_REG, lcr) != 0) {
        return 1;
    }

    return 0;
}

int uart_unset_dlab(unsigned long  port) {
    unsigned long lcr;

    if (uart_read(port, UART_LCR_REG, &lcr) != 0) {
        return 1;
    }

    bit_unset(lcr, LCR_DLAB_BIT);

    if (uart_write(port, UART_LCR_REG, lcr) != 0) {
        return 1;
    }

    return 0;
}

int uart_get_config(unsigned long port, uartConf *config){
	unsigned long lcr,ier;
	ushort_t divisor_latch;

	//LCR DATA
	if (uart_read(port, UART_LCR_REG, &lcr) != 0) {
	        return -1;
	}

	config->parity = UART_LCR_GET_PARITY(lcr);
	config->stop = UART_LCR_GET_STOP_BITS(lcr);
	config->bits = UART_LCR_GET_BITS_PER_CHAR(lcr);

    //IER DATA
    if (uart_read(port, UART_IER_REG, &ier) != 0) {
        return -1;
    }

    config->receivedDataInterrupt = bit_isset(ier, IER_ENABLE_RECEIVED_DATA_INTERRUPT) != 0;
    config->transmitterEmptyInterrupt = bit_isset(ier, IER_ENABLE_TRANSMITTER_EMPTY_INTERRUPT) != 0;
    config->receiverLineStatusInterrupt = bit_isset(ier, IER_ENABLE_RECEIVER_LINE_STATUS_INTERRUPT) != 0;

	//DIVISOR LATCH

    if (uart_get_divisor_latch(port, &divisor_latch) != 0) {
    	return -1;
    }

	config->bitRate = BitRate(divisor_latch.w.value);

	return 0;
}

int uart_set_config(unsigned long port, uartConf config){
	unsigned long lcr, new_lcr;
	unsigned char bit6,bit7;
	ushort_t divisor_latch;


	//GET CURRENT LCR
	if (uart_read(port, UART_LCR_REG, &lcr) != 0) {
		return 1;
	}

	//Define new LCR
	bit6 = (lcr & 0x40);
	bit7 = (lcr & 0x80);
	new_lcr = 0;

	new_lcr += (config.bits - 5);  //Number of bits per char
	new_lcr += (config.stop - 1) << LCR_NO_STOP_BITS_BIT;
	new_lcr += (config.parity) << LCR_PARITY2_BIT;
	new_lcr += bit6 << LCR_SET_BREAK_ENABLE_BIT;
	new_lcr += bit7 << LCR_DLAB_BIT;


	//NEW DL
	divisor_latch.w.value = DL(config.bitRate);
	if (uart_set_divisor_latch(port, divisor_latch) != 0) {
		return -1;
	}

	//Send Configuration
	if (uart_write(port, UART_LCR_REG, new_lcr)) {
		return -1;
	}

	return 0;
}

