#include <lcom/lcf.h>
#include "keyboard.h"
#include "i8042.h"

int hook_id = 1;
uint8_t scancode = 0;
int (keyboard_subscribe_int)(uint8_t *bit_no) {
    if(bit_no == NULL) return 1;
    *bit_no = BIT(hook_id);

    if (sys_irqsetpolicy(1, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != 0) {
        return 1;
    }
    return 0;
}

int (keyboard_unsubscribe_int)(){
    if (sys_irqrmpolicy(&hook_id) != 0) {
        return 1;
    }
    return 0;
}

void(kbc_ih)(){
    if (read_kbc_output_buffer(KBC_OUT_CMD, &scancode, 0) != 0) {
        printf("Error: Could not read scancode!\n");
    }
}

int(keyboard_restore)(){
    uint8_t cmdbyte;

    if(write_kbc_command(KBC_IN_CMD, KBC_READ_CMD) != 0) return 1;
    if(read_kbc_output_buffer(KBC_OUT_CMD, &cmdbyte, 0) != 0) return 1;

    cmdbyte |=  ENABLE_INT;

    if(write_kbc_command(KBC_IN_CMD, KBC_WRITE_CMD) != 0) return 1;
    if(write_kbc_command(KBC_WRITE_CMD, cmdbyte) != 0) return 1;
    return 0;
}

int (wait_ESC)() {

    int ipc_status;
    message msg;
    uint8_t irq_set;

    if (keyboard_subscribe_int(&irq_set) != 0) {
        return 1;
    }
    while (scancode != BREAK_ESC) {
        if (driver_receive(ANY, &msg, &ipc_status) != 0) {
            printf("driver_receive failed\n");
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: 
                    if (msg.m_notify.interrupts & irq_set) 
                        kbc_ih();
                    break;
                default:
                    break;
            }
        }
    }

    if (keyboard_unsubscribe_int() != 0) {
        return 1;
    }
    return 0;
   
}
