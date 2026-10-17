#include "serial_port.h"
#include "uart_macros.h"

int uart_send_data(uint16_t com_address, uint8_t data) {
    uint8_t lsr_data;

    if (util_sys_inb(com_address + LSR, &lsr_data) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    while (!(lsr_data & TRANSMITTER_READY)) {
        tickdelay(micros_to_ticks(WAIT_UART));
        if (util_sys_inb(com_address + LSR, &lsr_data) != 0) {
            printf("ERROR: %s\n", __func__);
            return 1;
        }
    }

    if (sys_outb(com_address + THR, data) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    return 0;
}

int uart_receive_data(uint16_t com_address, uint8_t* data) {
    uint8_t lsr_data;

    if (util_sys_inb(com_address + LSR, &lsr_data) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    while (!(lsr_data & RECEIVER_READY)) {
        tickdelay(micros_to_ticks(WAIT_UART));
        if (util_sys_inb(com_address + LSR, &lsr_data) != 0) {
            printf("ERROR: %s\n", __func__);
            return 1;
        }
    }

    if (util_sys_inb(com_address + RBR, data) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    return 0;
}

int uart_config(uint16_t com_address) {
    // disable interrupts
    uint8_t ier_data, lcr_data;
    if (util_sys_inb(com_address + IER, &ier_data) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    if (util_sys_inb(com_address + LCR, &lcr_data) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    ier_data &= DISABLE_INT;

    if (sys_outb(com_address + IER, ier_data) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    // set mode to 8 bits
    lcr_data |= BPC_8;
    lcr_data &= DLAB_0;

    if (sys_outb(com_address + LCR, lcr_data) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    return 0;
}
