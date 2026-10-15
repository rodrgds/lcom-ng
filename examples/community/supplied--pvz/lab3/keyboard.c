#include <lcom/lcf.h>
#include "keyboard.h"
#include "i8042.h"

static int hook_id_kbd = 1;
static uint8_t scancode = 0;
static bool kbd_error = false;

uint8_t get_scancode() { return scancode; }
bool get_kbd_error() { return kbd_error; }

int (kbd_subscribe_int)(uint8_t *bit_no) {
    *bit_no = hook_id_kbd;
    return sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_kbd);
}

int (kbd_unsubscribe_int)() {
    return sys_irqrmpolicy(&hook_id_kbd);
}

void (kbc_ih)() {
    uint8_t status;
    kbd_error = false;

    if (util_sys_inb(KBC_STAT_REG, &status) != 0) {
        kbd_error = true; 
        return;
    }

    if (status & KBC_OBF) {
        util_sys_inb(KBC_OUT_BUF, &scancode);
        if ((status & KBC_PARITY) || (status & KBC_TIMEOUT)) {
            kbd_error = true;
        }
    }
}

int (kbd_poll)() {
    uint8_t status;
    kbd_error = false;

    while (true) {
        if (util_sys_inb(KBC_STAT_REG, &status) != 0) return 1;

        if (status & KBC_OBF) {
            util_sys_inb(KBC_OUT_BUF, &scancode);
            if ((status & KBC_PARITY) || (status & KBC_TIMEOUT)) {
                kbd_error = true;
                return 1;
            }
            return 0; 
        }
        tickdelay(micros_to_ticks(DELAY_US));
    }
}

int (kbc_restore_interrupts)() {
    uint8_t cmd_byte;
    uint8_t status;

    // LER COMMAND BYTE
    sys_outb(KBC_CMD_REG, KBC_READ_CMD);
    util_sys_inb(KBC_OUT_BUF, &cmd_byte);
    
    // ATIVAR BIT DE INTERRUPÇÕES DO TECLADO
    cmd_byte |= KBC_INT_EN;
    
    // ESCREVER COMMAND BYTE (Garantindo que o Input Buffer não está cheio)
    while (true) {
        util_sys_inb(KBC_STAT_REG, &status);
        if ((status & KBC_IBF) == 0) {
            sys_outb(KBC_CMD_REG, KBC_WRITE_CMD);
            break;
        }
        tickdelay(micros_to_ticks(DELAY_US));
    }

    while (true) {
        util_sys_inb(KBC_STAT_REG, &status);
        if ((status & KBC_IBF) == 0) {
            sys_outb(KBC_IN_BUF, cmd_byte);
            break;
        }
        tickdelay(micros_to_ticks(DELAY_US));
    }

    return 0;
}
