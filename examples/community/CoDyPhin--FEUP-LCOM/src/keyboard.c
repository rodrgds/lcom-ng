#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "keyboard.h"
#include "i8042.h"

int hook_id_kbd = KEYBOARD_IRQ;
uint32_t inb_count = 0;
uint8_t scanCode = 0;

int (keyboard_subscribe_int)(uint8_t *bit_no) {
    if (bit_no == NULL) { //checks if the pointer is valid
        printf("Pointer bit_no is invalid.\n");
        return 1;
    }

    *bit_no = hook_id_kbd;
    if(sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_kbd)) {
        printf("Error subscribing kbc interrupts.\n");
        return 1;
    }
    return 0;
}

int (keyboard_unsubscribe_int)() {
    if (sys_irqrmpolicy(&hook_id_kbd) != 0) {
        printf("Error unsubscribing the interrupt of the hook.\n");
        return 1;
    }
    return 0;
}

int (count_sys_inb)(int port, uint8_t *value) {
    if (util_sys_inb(port, value)) return 1;
    inb_count++;
    return 0;
}

void (kbc_ih)() {
    uint8_t stataux;

    if(count_sys_inb(STAT_REG, &stataux)) {
        printf("Unable to read the keyboard status\n");
        return;
    }

    if(stataux & OBF) {
        if(count_sys_inb(OUT_BUF, &scanCode)) return;
        if(stataux & (TO_ERR | PAR_ERR)) scanCode = 0;
    }
    else scanCode = 0;
}


int (scanCodeReader)(bool *make, uint8_t *size, uint8_t *bytes) {
    int aux = 0;
    if(scanCode != 0) { // When scanCode = 0, no errors occured reading the OUT_BUF
        if(scanCode == ESC_BREAKCODE) aux = -1;
        if(scanCode != TWO_BYTE_LSB){
            if(*size) {
                (*size)+=1;
                bytes[1] = scanCode;
            }
            else {
                (*size)+=1;
                bytes[0] = scanCode;
            }
            *make = (!(scanCode & MSB));
            if(aux == -1) return aux;
            else return 1;
        }
        else {
            (*size)++;
            bytes[0] = scanCode;
            return 0;
        }
    }
    return 0;
}



uint8_t getScanCode() {
    return scanCode;
}
