#include <lcom/lcf.h>
#include "serial_port.h"

static int hook_id = 4;
static Queue* inQueue = NULL;

int sp_subscribe_int(uint8_t* bitno){
    *bitno = hook_id;
    hook_id = COM1_IRQ;
    if(sys_irqsetpolicy(COM1_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != 0) {
        printf("Failed to subscribe serial port interrupts.\n");
        return 1;
    }
    return 0;
}

int sp_unsubscribe_int(){
    if(sys_irqrmpolicy(&hook_id) != 0) {
        printf("Failed to unsubscribe serial port interrupts.\n");
        return 1;
    }
    return 0;
}

int sp_get_status(uint8_t* status){
    return util_sys_inb(UART_BASE + LINE_STATUS_R, status);
}

void sp_init(){
    uint8_t ier;
    if(util_sys_inb( UART_BASE + INTERRUPT_ENABLE_R, &ier) != 0){
        printf("Failed to read IER register.\n");
        return;
    } 
    ier &= 0xF0;
    if(sys_outb( UART_BASE + INTERRUPT_ENABLE_R, ier | IER_RECEIVED_DATA_AVAILABLE) != 0) {
        printf("Failed to write to IER register.\n");
        return;
    }
    if (sys_outb( UART_BASE + FIFO_CONTROL_R, FCR_CLEAR) != 0) {
        printf("Failed to configure FIFO.\n");
        return;
    }
    inQueue = createQueue();
    sp_clear_buffers();
}

void sp_exit(){
    destroyQueue(inQueue);
    inQueue = NULL;
}

int send_byte(uint8_t byte){
    uint8_t status, attempts = 10;
    while(attempts--){
        if(sp_get_status(&status)) {
            printf("Failed to get status.\n");
            return 1;
        }
        if(status & LSR_TRANSMITTER_HOLDING_R_EMPTY){
            if (sys_outb( UART_BASE + TRANSMITTER_HOLDING_R ,byte) != 0) {
                printf("Failed to write byte to THR.\n");
                return 1;
            }
            return 0;
        }   
    }
    return 1;
}

int read_byte(){
    uint8_t status, data;
    if(sp_get_status(&status)) {
        printf("Failed to get status.\n");
        return 1;
    }
    if(status & LSR_DATA_READY){
        if(util_sys_inb( UART_BASE + RECEIVED_BUFFER_R, &data)) {
            printf("Failed to read byte from data.\n");
            return 1;
        }
        if(!(status & (LSR_OVERRUN_ERROR | LSR_PARITY_ERROR | LSR_FRAMING_ERROR))){
            push(inQueue,data);
            return 0;
        }
    }
    return 1;
}

int sp_clear_buffers(){
    if(sys_outb(UART_BASE + FIFO_CONTROL_R, FCR_CLEAR)) return 1;
    uint8_t status, dummy;
    while (1) {
        if (sp_get_status(&status)) break;
        if (status & LSR_DATA_READY) {
            util_sys_inb(UART_BASE + RECEIVED_BUFFER_R, &dummy);
        } else {
            break;
        }
    }
    while(!isEmpty(inQueue)) {
        pop(inQueue);
    }   
    return 0;
}

void sp_ih(){
    uint8_t interrupt_iden;
    if(util_sys_inb(UART_BASE + INTERRUPT_IDENT_R, &interrupt_iden) != 0) {
        printf("Failed to read IIR register.\n");
        return;
    }
    if (!(interrupt_iden & IIR_NO_INT_PENDING)) {
        uint8_t int_type = interrupt_iden & IIR_ID;
        if (int_type == (IIR_FIFO_CT) || int_type == (IIR_OGN_RCVD_DATA_AVL)) {
            while (read_byte() == 0);
        }
    }        
}

uint8_t get_sp_byte() {
    if (isEmpty(inQueue)) {
        return 0;
    }
    return pop(inQueue);
}
