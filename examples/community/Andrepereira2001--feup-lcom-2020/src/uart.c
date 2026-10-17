#include <lcom/lcf.h>
#include <stdint.h>
#include <minix/sysutil.h>

#include "uart.h"
#include "uartVars.h"

/** @brief uart hook id */
int uart1_hook_id = 0;

int uart_subscribe_int(uint8_t *bit_no){
    uart1_hook_id = UART_IRQ1;
    *bit_no = uart1_hook_id;
    if(sys_irqsetpolicy(UART_IRQ1,IRQ_REENABLE|IRQ_EXCLUSIVE,&uart1_hook_id) != 0){
        printf("sys_irqsetpolicy error!\n");
        return 1;
    }
    return 0;
}

int uart_unsubscribe_int(){
    if( sys_irqrmpolicy(&uart1_hook_id) != 0){
        printf("sys_irqrmpolicy error!\n");
        return 1;
    } 
    return 0;
}

int config_uart1(){
    uint8_t data;

    // write command register
    if (util_sys_inb(COM1_REG + LCR_REG,&data) != 0){
        printf("Error while reading control reg\n");
        return 1;
    }

    data |= LCR_8_BITS_CHAR | LCR_ODD_PARITY;


    if (sys_outb(COM1_REG + LCR_REG,data) != 0){
        printf("Error while setting control reg\n");
        return 1;
    }

    // write FIFO REGISTER
    data = 0x00;
    data = FIFO_ENABLE | FIFO_CLEAR_RCVR | FIFO_CLEAR_XMIT | FIFO_4;

    if (sys_outb(COM1_REG + FCR_REG,data) != 0){
        printf("Error while setting fifo reg\n");
        return 1;
    }

    // write interrupt enable register
    if (util_sys_inb(COM1_REG + IER_REG,&data) != 0){
        printf("Error while reading interrupt enable reg\n");
        return 1;
    }
    data = IER_EN_RDA_INT | IER_EN_THR_INT | IER_EN_RLS_INT;

    if (sys_outb(COM1_REG + FCR_REG,data) != 0){
        printf("Error while setting interrupt enable reg\n");
        return 1;
    }

    return 0;
}

bool check_LSR_byte_received(){
    uint8_t data;
    util_sys_inb(COM1_REG + LSR_REG,&data);
    if(data & LSR_DATA_RECEIVED){
        if(data & LSR_OVER_ERROR || data & LSR_PARATY_ERROR || data & LSR_FRAMING_ERROR){
            return false;
        }
        return true;
    }
    return false;
}

bool check_LSR_THR_empty(){
    uint8_t data;
    util_sys_inb(COM1_REG + LSR_REG,&data);
    if(data & LSR_THR_EMPTY){
        return true;
    }
    return false;
}

void read_RBR_byte(uint8_t *data){
    util_sys_inb(COM1_REG + RBR_REG,data);
}

void send_THR_byte(uint8_t data){
    sys_outb(COM1_REG + THR_REG,data);
}
