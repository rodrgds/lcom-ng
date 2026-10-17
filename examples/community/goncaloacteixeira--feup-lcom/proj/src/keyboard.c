//
// Created by skidr on 13/10/2019.
//
#include "lcom/lcf.h"
#include "keyboard.h"

static int32_t kbd_id = 1;
uint8_t scancode;
int32_t sys_inb_counter = 0;

int sys_inb_kbd_count(port_t port, uint32_t *value) {
    sys_inb_counter++;
    return sys_inb(port, value);
}

int sys_inb_kbd(port_t port, uint8_t *value) {
    uint32_t temp;
    if (sys_inb_kbd_count(port, &temp) == OK) {
        (*value) = (uint8_t) temp;
        return 0;
    }
    printf("Error::sys_inb_kbd_count(port, &temp)\n");
    return 1;
}

void (kbc_ih)(void) {
    uint32_t status_reg;

    if (sys_inb_kbd_count(ST_REG, &status_reg) != 0) {
        printf("Error::cannot read keyboard status\n");
        return;
    }

    if (OB_FULL & status_reg) { // output buffer needs to be full so we can read data from it
        if (sys_inb_kbd(BUFF_OUT, &scancode) != 0) {
            printf("Error::cannot read scancode\n");
            return;
        }

        if ((status_reg & (PRT_ERROR | TIMEOUT_ERROR)) == 0) {
            return;
        }

        else {
            printf("Error::Parity or Time-out error\n");
            return;
        }
    }
    return;
}

int keyboard_subscribe_int(uint32_t *bit_no) {
    (*bit_no) = BIT(kbd_id);
    if(sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_id) == OK) {
        return 0;
    } else {
        printf("Error::sys_irqsetpolicy(KBD_ID, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_id)\n");
        return 1;
    }
}

int keyboard_unsubscribe_int(void) {
    if (sys_irqrmpolicy(&kbd_id) == OK) {
        return 0;
    }
    else {
        printf("Error::sys_irqrmpolicy(&kbd_id)\n");
        return 1;
    }
}

//Funções para a KBD_TEST_POLL

int (kbc_poll)(void) {
    uint32_t status_reg;

    if (sys_inb_kbd_count(ST_REG, &status_reg) != 0) {
        printf("Error::cannot read keyboard status\n");
        return 1;
    }

    if (OB_FULL & status_reg) { // output buffer needs to be full so we can read data from it
        if (sys_inb_kbd(BUFF_OUT, &scancode) != 0) {
            printf("Error::cannot read scancode\n");
            return 1;
        }

        if ((status_reg & (PRT_ERROR | TIMEOUT_ERROR)) == 0) {
            return 0;
        }

        else {
            printf("Error::Parity or Time-out error\n");
            return 1;
        }
    }
    return 1;
}
