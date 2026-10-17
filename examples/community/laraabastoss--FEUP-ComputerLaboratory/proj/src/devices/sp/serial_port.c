#include "serial_port.h"

int sp_hook_id = 7;
uint8_t sp_data = 0;
extern int player;

int (sp_send_data)(uint8_t data) {
    uint8_t status;
    while (true) {
        if (util_sys_inb(0x3F8 + 5, &status) != 0) return 1;
        if (status & (BIT(5) | BIT(6))){
            return (sys_outb(0x3F8,data));
        }
        tickdelay(micros_to_ticks(10));
    }
}

int (sp_process_data)() {
    uint8_t status;
    if (util_sys_inb(0x3F8 + 5, &status) != 0) return 1;
    if (!(status & BIT(0))){ // error reading data
        sp_data = 0; // reset data
        return 1;
    }
    while (status & BIT(0)) {
        if (util_sys_inb(0x3F8, &sp_data)) return 1;
        if (util_sys_inb(0x3F8 + 5, &status)) return 1;
    }
    return 0;
}

int (sp_subscribe_interrupts)() {
  return sys_irqsetpolicy(SER_IRQ,IRQ_REENABLE | IRQ_EXCLUSIVE,&sp_hook_id);
}

int (sp_unsubscribe_interrupts)() {
  return sys_irqrmpolicy(&sp_hook_id);
}
