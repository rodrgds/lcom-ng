#include <lcom/lcf.h>
#include "serialport.h"
#include "../macros.h"

int ser_hook_id = 3;

int ser_subscribe_int(uint8_t* bitno) {
    *bitno = BIT(ser_hook_id);
    return sys_irqsetpolicy(COM1_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &ser_hook_id);
}

int ser_unsubscribe_int() {
    return sys_irqrmpolicy(&ser_hook_id);
}

uint8_t sp_ih() {
    uint8_t iir;
    if(util_sys_inb(COM1 + IIR, &iir)) return 1;
    if(!(iir & IIR_NO_INT_PENDING) && (iir & IIR_INT_PENDING == IIR_CHAR_TIMEOUT_FIFO)) {
        uint8_t status;
        uint8_t byte;
        for (int i = 10; i > 0; i--) {
            if(util_sys_inb(COM1 + LSR, &status)) break;
            if(status & LSR_RECEIVER_DATA) {
                if(util_sys_inb(COM1 + RBR, &byte)) break;
                if(!(status & (LSR_OVERRUN_ERROR| LSR_PARITY_ERROR| LSR_FRAMING_ERROR))) {
                    return byte;
                }
            }
        }
    }
    return 1;
}

int send(uint8_t val) {
    if (val != 1) printf("send: %d\n", val);
    uint8_t status;
    for (int i = 10; i > 0; i--) {
      if(util_sys_inb(COM1 + LSR, &status)) return 1;
      if(status & LSR_TRANSMITTER_HOLDING_REG_EMPTY) {
        return sys_outb(COM1 + THR, val);
      }
    }
    return 1;
}

int serial_clear() {
    printf("clear\n");
    if (sys_outb(COM1 + FCR, FCR_ENABLE_BOTH_FIFO|FCR_CLEAR_RCVR_FIFO|FCR_CLEAR_XMIT_FIFO)) return 1;
    return 0;
}
