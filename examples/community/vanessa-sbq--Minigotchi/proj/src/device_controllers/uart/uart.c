#include "uart.h"

int serial_hook_id = 7;
static Queue* rxQueue;


/**
 * @brief Functions for RX Side
 */
int serial_read_byte(){
    uint8_t st;
    uint8_t byte;

    if(util_sys_inb(SERIAL_PORT + LINE_STATUS, &st) != 0) {
        return 1;
    }
    if(st & LSR_DATA_READY){
        if(util_sys_inb(SERIAL_PORT + RECEIVE_BUFFER, &byte)) {
            return 1;
        }
        if(!(st & (LSR_OVERRUN_ERROR | LSR_PARITY_ERROR | LSR_FRAMING_ERROR))){
            enqueue(rxQueue, byte);
            return 0;
        }
    }
    return 1;
}





/**
 * @brief Functions for TX Side
 */
int serial_send_byte(uint8_t byte){
    uint8_t st;
    uint8_t attmps = 10;

    while(attmps--){
        if( util_sys_inb(SERIAL_PORT + LINE_STATUS, &st)) {
            return 1;
        }

        if(st & LSR_THR_EMPTY){
            return sys_outb(SERIAL_PORT + TRANSMIT_HOLD, byte);
        }
    }

    return 1;
}





/**
 * @brief Subscribes UART interrupts
 */
int serial_subscribe_int(uint8_t* bit_no){
    *bit_no = serial_hook_id;
    if(sys_irqsetpolicy(SERIAL_IRQ_1, IRQ_REENABLE | IRQ_EXCLUSIVE, &serial_hook_id) != 0){
        printf("Error in sys_irqsetpolicy()\n");
        return 1;
    }
    return 0;
}

/**
 * @brief This function initializes the Serial Port
 */
int serial_init(){
    uint8_t lcr;

    if(util_sys_inb(SERIAL_PORT + INTERRUPT_ENABLE, &lcr) != 0){
        printf("Error in utils_sys_inb()\n");
        return 1;
    }

    lcr &= 0xF0;

    if(sys_outb(SERIAL_PORT + INTERRUPT_ENABLE, lcr | IER_DATA_AVAIL_INT) != 0){
        printf("Error in sys_outb()\n");
        return 1;
    }
    
    rxQueue = createQueue();
    return 0;
}





/**
 * @brief Unsubscribes UART interrupts
*/
int serial_unsubscribe_int(){
    if(sys_irqrmpolicy(&serial_hook_id) != 0){
        printf("Error in sys_irqrmpolicy()\n");
        return 1;
    }
    return 0;
}

/**
 * @brief Free RX queue
*/
void serial_exit(){
    free(rxQueue);
}





/**
 * @brief Get RX queue 
 */
Queue* serial_get_receive_queue(){
    return rxQueue;
}

/**
 * @brief UART interrupt handler
 */
void serial_interrupt_handler(){
    uint8_t ir;

    if(util_sys_inb(SERIAL_PORT + INTERRUPT_ID, &ir) != 0){
        printf("Error in util_sys_inb()\n");
        return;
    }

    if(!(ir & IIR_NO_INTERRUPT)){
        if(ir & IIR_INTERRUPT_ID == IIR_CHAR_TIMEOUT){
            while(!serial_read_byte());
        }
    }

    serial_clear_interrupts();
}

/**
 * @brief Clear UART interrupts
 */
int serial_clear_interrupts(){
    if(sys_outb(SERIAL_PORT + FIFO_CONTROL, FCR_CLEAR) != 0){
        printf("Error in sys_outb()\n");
        return 1;
    }

    return 0;
}





