#include "sp.h"

int hook_id_sp = 7;

uint8_t received_byte;
bool byte_was_received;

int sp_subscribe_int(uint8_t* irq_set) {
    *irq_set = BIT(hook_id_sp);
    return sys_irqsetpolicy(COM1_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_sp);
}

int sp_unsubscribe_int() {
    return sys_irqrmpolicy(&hook_id_sp);
}

int sp_send_byte(uint8_t byte) {
    uint8_t st;
    int attempts = 5;
    while (attempts--) {
        util_sys_inb(COM1_BASE_ADDR + LSR, &st);
        if (st & BIT(5)) {
            return sys_outb(COM1_BASE_ADDR, byte);
        }
    }
    return 1;
}

void sp_ih() {
    uint32_t iir;
    sys_inb(COM1_BASE_ADDR + IIR, &iir);

    if (iir & NO_INTERRUPT_PENDING) return;

    switch (iir & 0x06) {
        // receiver line status
        case 0x06: {

            break;
        }
        // received data available
        case 0x04: {
            uint8_t lsr_content;
            util_sys_inb(COM1_BASE_ADDR + LSR, &lsr_content);
            if (lsr_content & BIT(0)) {
                util_sys_inb(COM1_BASE_ADDR, &received_byte);
                byte_was_received = true;
            }
            break;
        }
    }
}






// int sp_test_conf() {
//     printf("\n");

//     uint8_t lcr_content;
//     util_sys_inb(COM1_BASE_ADDR + LCR, &lcr_content);

//     uint8_t char_bits = lcr_content & 0x03;

//     switch (char_bits) {
//         case 0x00:
//             printf("5 bits per char");
//             break;
//         case 0x01:
//             printf("6 bits per char");
//             break;
//         case 0x02:
//             printf("7 bits per char");
//             break;
//         case 0x03:
//             printf("8 bits per char");
//             break;
//     }

//     printf("\n");

//     uint8_t stop_bits = lcr_content & 0x04;

//     switch (stop_bits) {
//         case 0x00:
//             printf("1 stop bit");
//             break;
//         case 0x04:
//             printf("2 stop bits (1 and 1/2 when 5 bits char)");
//             break;
//     }

//     printf("\n");

//     uint8_t parity_bits = lcr_content & 0x0E;
//     switch (parity_bits) {
//         case 0x08:
//             printf("Odd parity");
//             break;
//         case 0x18:
//             printf("Even parity");
//             break;
//         case 0x28:
//             printf("Parity bit is 1 (always)");
//             break;
//         case 0x38:
//             printf("Parity bit is 0 (always)");
//             break;
//         default:
//             printf("No parity");
//             break;
//     }

//     return 0;
// }

