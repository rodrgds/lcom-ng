#include <lcom/lcf.h>

#include "serial_port.h"
#include "macro_sp.h"

#include "queue.h"

#include "../utils/utils.h"

static int sp_hook_id = COM1_IRQ;

int (sp_subscribe_int)(uint8_t *bit_no) {
    *bit_no = sp_hook_id;
    return sys_irqsetpolicy(COM1_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &sp_hook_id);
}

int (sp_unsubscribe_int)() {
    return sys_irqrmpolicy(&sp_hook_id);
}

int (sp_init)(){
    int r;
    uint8_t data;

    //Configure Line Control Register
    if ((r = util_sys_inb(COM1_BASE + LCR, &data)) != 0)
        return r;

    data |= (BIT(1) | BIT(0));      //set word length to 8 bits
    data &= ~BIT(2);                //set no. of stop bits to 1
    data &= ~BIT(3);                //set parity to none
    data &= ~BIT(7);                //set DLAB to 0

    if ((r = sys_outb(COM1_BASE + IER, data)) != 0)
        return r;

    //Configure Interrupt Enable Register
    if ((r = util_sys_inb(COM1_BASE + IER, &data)) != 0)
        return r;

    data |= BIT(0);                 //enable received data interrupt
    data |= BIT(1);                 //enable transmitter empty interrupt

    if ((r = sys_outb(COM1_BASE + IER, data)) != 0)
        return r;

    //Configure FIFO Control Register
    data = 0x00 & ~BIT(7) & ~BIT(6);    //set trigger level to 1
    data |= BIT(0);                     //enable FIFO
    data |= BIT(1) | BIT(2);            //clear fifo

    if ((r = sys_outb(COM1_BASE + FCR, data)) != 0)
        return r;

    return 0;
}

int (sp_cleanup)() {
    int r;
    uint8_t data;

    // Disable interrupts
    if ((r = util_sys_inb(COM1_BASE + IER, &data)) != 0)
        return r;

    data &= ~(BIT(0) | BIT(1));  // Disable received data and transmitter empty interrupts

    if ((r = sys_outb(COM1_BASE + IER, data)) != 0)
        return r;

    // Disable FIFO
    data &= ~BIT(0);

    if ((r = sys_outb(COM1_BASE + FCR, data)) != 0)
        return r;

    return 0;
}

int (sp_get_status)(uint8_t* status) {
    return util_sys_inb(COM1_BASE + LSR, status);
}

int (sp_read)() {
    int r;
    uint8_t status, data;

    do {
        if ((r = sp_get_status(&status)) != 0)
            return r;

        if (status & BIT(0)) {                                  //check if receiver ready
            if (!(status & (BIT(1) | BIT(2) | BIT(3)))) {      //check if no errors
                if ((r = util_sys_inb(COM1_BASE + RBR, &data)) != 0)
                    return r;
                
                queue_push(QUEUE_RECEIVE, data);

            }
        } else return 0;

    } while (true);
}

int (sp_write)() {
    int r;
    uint8_t status, data;

    if (queue_is_empty(QUEUE_TRANSMIT)) return 1;

    if ((r = sp_get_status(&status)) != 0)
        return r;

    if ((status & BIT(5)) && !(status & (BIT(1) | BIT(2) | BIT(3)))) {      //check if transmitter holding register is empty and no errors
        queue_pop(QUEUE_TRANSMIT, &data);
        return sys_outb(COM1_BASE + RBR, data);
    }

    return 1;
}

void (sp_ih)(){
    uint8_t interrupt_id;

    if (util_sys_inb(COM1_BASE + IIR, &interrupt_id) != 0)
        return;

    if (interrupt_id & BIT(1)) {            //Transmitter Empty
        sp_write();
    } else if (interrupt_id & BIT(2)) {     //Received Data Available
        sp_read();
    }
}
